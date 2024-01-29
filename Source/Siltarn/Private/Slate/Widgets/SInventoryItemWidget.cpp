#include "Slate/Widgets/SInventoryItemWidget.h"
#include "Siltarn/Public/Interactables/PickupEntity.h"
#include "Siltarn/Public/Interactables/EquipableEntity.h"
#include "Siltarn/Public/Interactables/NonEquipableEntity.h"
#include "Siltarn/Public/Slate/Widgets/SInventoryWidget.h"
#include "Styling/CoreStyle.h"

DEFINE_LOG_CATEGORY(LogClass_SInventoryItemWidget  );
DEFINE_LOG_CATEGORY(LogClass_FInventoryItemDragDrop);
DEFINE_LOG_CATEGORY(LogClass_FItemTooltip          );

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

SInventoryItemWidget::SInventoryItemWidget()
{
	m_bIsInInventory = true;
}

SInventoryItemWidget::~SInventoryItemWidget()
{
	UE_LOG(LogClass_SInventoryItemWidget, Warning, TEXT("I was destroyed !"));
}

void SInventoryItemWidget::Construct(const FArguments& p_InArgs)
{
	m_Tile         = p_InArgs._a_Tile; // Luciole ! We are not using m_Tile anymore. However, the game crashed when dealing with two items in the inventory. Need to investigate. Once it is done, remove m_Tile from here.
	m_ItemSize     = p_InArgs._a_ItemSize;
	m_ItemData     = p_InArgs._a_ItemData;
	m_ParentWidget = p_InArgs._a_ParentWidget;

	if (m_ItemData)
	{
		m_ItemIcon.SetResourceObject(m_ItemData->GET_Icon());
	}

	m_ItemBackgroundColor.R = 1.0f;
	m_ItemBackgroundColor.G = 1.0f;
	m_ItemBackgroundColor.B = 1.0f;
	m_ItemBackgroundColor.A = 0.0f;

	ChildSlot
	.HAlign(HAlign_Fill)
	.VAlign(VAlign_Fill)
	[
		SNew(SOverlay)

		+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)		
		[
			SAssignNew(m_BackgroundBorder, SBorder)
			.BorderImage(&m_GeneralStyle.m_WhiteBackgroundImage_SlateBrush)
			.BorderBackgroundColor(m_ItemBackgroundColor)
			.Padding(0)
		]

		+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SAssignNew(m_IconImage, SImage)
			.Image(&m_ItemIcon)
		]
	];
}

void SInventoryItemWidget::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (m_BackgroundBorder == nullptr)
	{
		UE_LOG(LogClass_SInventoryItemWidget, Warning, TEXT("OnMouseEnter() : m_BackgroundBorder is NULL ! || Unable to proceed further."));
		return;
	}	

	m_ItemBackgroundColor.A = 0.25f;

	m_BackgroundBorder->SetBorderBackgroundColor(m_ItemBackgroundColor);

	m_ItemTooltip = FItemTooltip::CREATE_Tooltip(m_ItemData);
	
	if (m_ItemTooltip.IsValid())
	{
		m_ItemTooltip->MOVE_Window(MyGeometry.GetAbsolutePositionAtCoordinates(FVector2D(0)));
	}
}

void SInventoryItemWidget::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	if (m_BackgroundBorder == nullptr)
	{
		UE_LOG(LogClass_SInventoryItemWidget, Warning, TEXT("OnMouseEnter() : m_BackgroundBorder is NULL ! || Unable to proceed further."));
		return;
	}

	m_ItemBackgroundColor.A = 0.0f;


	m_BackgroundBorder->SetBorderBackgroundColor(m_ItemBackgroundColor);

	if (m_ItemTooltip.IsValid())
	{
		m_ItemTooltip->DESTROY_Tooltip();
		m_ItemTooltip.Reset(); // If we do not add this line, the FItemTooltip that is created when the item is hovered for the first time is never deleted 
	}
}

FReply SInventoryItemWidget::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	// Left click -> drag 
	if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		if (m_ItemTooltip.IsValid())
		{
			m_ItemTooltip->DESTROY_Tooltip();
			m_ItemTooltip.Reset();
		}
		
		return FReply::Handled().DetectDrag(SharedThis(this), EKeys::LeftMouseButton).CaptureMouse(SharedThis(this));
	}

	// Right click -> place in character profile widget
	if (MouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		if (m_bIsInInventory)
		{
			if (m_ItemData->IS_Equipable())
			{
				if (m_ItemTooltip.IsValid())
				{
					m_ItemTooltip->DESTROY_Tooltip();
					m_ItemTooltip.Reset();
				}

				if (m_ParentWidget)
				{
					m_ParentWidget->MOVE_ItemToCharacterProfileWidget(this);
				}

				return DeleteItemWidget();
			}

			return FReply::Handled();
		}		
	}

	return FReply::Unhandled();
}

FReply SInventoryItemWidget::OnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		const float _Ratio            = GET_ScreenToViewportRatio();
		FVector2D _MousePositionLocal = MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());

		TSharedRef<FInventoryItemDragDrop> _Operation = FInventoryItemDragDrop::New
		(
			this       , 
			_Ratio     , 
			m_ItemSize , 
			m_ItemData , 
			m_Tile
		);

		SetVisibility(EVisibility::Collapsed);

		return FReply::Handled().BeginDragDrop(_Operation);
	}

	return FReply::Unhandled();
}

float SInventoryItemWidget::GET_ScreenToViewportRatio() const
{
	FVector2D _ViewportSize;

	if (GEngine == nullptr)
	{
		UE_LOG(LogClass_SInventoryItemWidget, Warning, TEXT("GET_ScreenToViewportRatio() : GEngine is NULL !"));
	}
	else
	{
		UE_LOG(LogClass_SInventoryItemWidget, Warning, TEXT("GET_ScreenToViewportRatio() : GEngine is NOT NULL !"));
	}

	if (GEngine->GameViewport == nullptr)
	{
		UE_LOG(LogClass_SInventoryItemWidget, Warning, TEXT("GET_ScreenToViewportRatio() : GameViewport is NULL !"));
	}
	else
	{
		UE_LOG(LogClass_SInventoryItemWidget, Warning, TEXT("GET_ScreenToViewportRatio() : GameViewport is NOT NULL !"));
	}

	GEngine->GameViewport->GetViewportSize(_ViewportSize);

	const double _PercentageX = _ViewportSize.X / RESOLUTION_X;
	const double _PercentageY = _ViewportSize.Y / RESOLUTION_Y;

	int32 _PercentageIntX = FMath::FloorToInt32(_PercentageX * DECORATOR_SIZE_PRECISION);
	int32 _PercentageIntY = FMath::FloorToInt32(_PercentageY * DECORATOR_SIZE_PRECISION);

	float _Ratio = 0.0f;

	if (_PercentageIntX >= _PercentageIntY)
	{
		_Ratio = (float)_PercentageIntY;
	}
	else
	{
		_Ratio = (float)_PercentageIntX;
	}

	UE_LOG(LogClass_SInventoryItemWidget, Warning, TEXT("GET_ScreenToViewportRatio() : I arrived here ! !"));

	_Ratio /= DECORATOR_SIZE_PRECISION;

#if WITH_EDITOR

	UE_LOG(LogClass_SInventoryItemWidget, Log, TEXT("GET_ScreenToViewportRatio() : _Ratio : %f"), _Ratio);

#endif

	return _Ratio;
}

void SInventoryItemWidget::UPDATE_Tile(FTile& p_NewTile)
{
	m_Tile = &p_NewTile;
}

FReply SInventoryItemWidget::DeleteItemWidget()
{


	return FReply::Handled();
}

void SInventoryItemWidget::UPDATE_Widget(SInventoryItemWidget* p_ItemWidget)
{
	m_IconImage = p_ItemWidget->m_IconImage ;
	m_ItemData  = p_ItemWidget->m_ItemData  ;

	m_ItemIcon.SetResourceObject(m_ItemData->GET_Icon());
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION





/**************************************************************************************
	Drag & drop operation class
/**************************************************************************************/

int32 FInventoryItemDragDrop::m_InstanceCount = 0;

FInventoryItemDragDrop::FInventoryItemDragDrop()
{
	m_InstanceCount++;
	UE_LOG(LogClass_FInventoryItemDragDrop, Log, TEXT("A new instance was created ! || Instance count : %d"), m_InstanceCount);
}

FInventoryItemDragDrop::~FInventoryItemDragDrop()
{
	m_DraggedItem->SetVisibility(EVisibility::Visible);
	
	m_InstanceCount--;
	UE_LOG(LogClass_FInventoryItemDragDrop, Log, TEXT("An instance was destroyed ! || Instance count : %d"), m_InstanceCount);
}

void FInventoryItemDragDrop::OnDragged(const class FDragDropEvent& DragDropEvent)
{
	if (CursorDecoratorWindow.IsValid())
	{
		CursorDecoratorWindow->MoveWindowTo(DragDropEvent.GetScreenSpacePosition() - GET_WidgetMiddlePosition());
	}
}

TSharedRef<FInventoryItemDragDrop> FInventoryItemDragDrop::New
(
	SInventoryItemWidget* p_DraggedItem           , 
	const float           p_ScreenToViewportRatio , 
	const FVector2D&      p_WidgetSize            , 
	UPickupEntity*        p_ItemEntity            ,
	FTile*                p_Tile
)
{
	TSharedRef<FInventoryItemDragDrop> _Operation = MakeShareable(new FInventoryItemDragDrop);

	_Operation->m_IconBrush.SetResourceObject(p_ItemEntity->GET_Icon());

	_Operation->m_DraggedItem           = p_DraggedItem           ;
	_Operation->m_DecoratorSize         = p_WidgetSize            ;
	_Operation->m_ScreenToViewportRatio = p_ScreenToViewportRatio ;
	_Operation->m_ItemEntity            = p_ItemEntity            ;
	_Operation->m_Tile                  = p_Tile                  ;

	_Operation->Construct();

	return _Operation;
}

TSharedPtr<SWidget> FInventoryItemDragDrop::GetDefaultDecorator() const
{
	return
		SNew(SBox)
		.WidthOverride(m_DecoratorSize.X * m_ScreenToViewportRatio)
		.HeightOverride(m_DecoratorSize.Y * m_ScreenToViewportRatio)
		[
			SNew(SBorder)
			.Cursor(EMouseCursor::GrabHandClosed)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SImage)
				.Image(&m_IconBrush)
			]
		];
}





/**************************************************************************************
	Item tooltip class
/**************************************************************************************/

int32 FItemTooltip::m_InstanceCount = 0;

FItemTooltip::FItemTooltip()
{
	m_InstanceCount++;
	UE_LOG(LogClass_FItemTooltip, Log, TEXT("A new instance was created ! || Instance count : %d"), m_InstanceCount);
}

FItemTooltip::~FItemTooltip()
{
	DESTROY_Tooltip();
	m_InstanceCount--;
	UE_LOG(LogClass_FItemTooltip, Log, TEXT("An instance was destroyed ! || Instance count : %d"), m_InstanceCount);
}

TSharedRef<FItemTooltip> FItemTooltip::CREATE_Tooltip(UPickupEntity* p_Item)
{
	TSharedRef<FItemTooltip> _Tooltip = MakeShareable(new FItemTooltip);

	_Tooltip->m_ItemEntity = p_Item;

	_Tooltip->CREATE_TooltipWindow();

	return _Tooltip;
}

void FItemTooltip::DESTROY_Tooltip()
{
	if (m_TooltipWindow.IsValid())
	{
		m_TooltipWindow->RequestDestroyWindow();
		m_TooltipWindow.Reset();
	}
}

void FItemTooltip::MOVE_Window(const FVector2D& p_Position)
{
	if (m_TooltipWindow.IsValid())
	{
		FVector2D _WindowSize = m_TooltipWindow->GetSizeInScreen();
		m_TooltipWindow->MoveWindowTo(p_Position + FVector2D(-_WindowSize.X, 0.0f));
	}
}

void FItemTooltip::CREATE_TooltipWindow()
{
	EPickupEntityType _ItemEntityType = m_ItemEntity->GET_ItemType();

	TSharedPtr<SWidget> _TooltipWidget = nullptr;

	if (_ItemEntityType == EPickupEntityType::Pistol)
	{
		_TooltipWidget = BUILD_Tooltip_Pistol();
	}
	if (_ItemEntityType == EPickupEntityType::Ammunition)
	{
		_TooltipWidget = BUILD_Tooltip_Ammunition();
	}



	if (_TooltipWidget.IsValid())
	{
		m_TooltipWindow = SWindow::MakeStyledCursorDecorator(m_GeneralStyle.m_ItemTooltip_WindowStyle); // See above
		m_TooltipWindow->SetContent(_TooltipWidget.ToSharedRef());

		FSlateApplication::Get().AddWindow(m_TooltipWindow.ToSharedRef(), true);
	}
}

TSharedPtr<SWidget> FItemTooltip::BUILD_TooltipWidget() const
{
	FSlateFontInfo _ItemCategoryFont(m_GeneralStyle.m_Monorita_Font);
	_ItemCategoryFont.Size = 10;

	check(m_ItemEntity != nullptr);

	if (m_ItemEntity == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("m_ItemEntity is NULL !"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("m_ItemEntity is NOT NULL !"));
	}

	return

		SNew(SBorder)
		.BorderImage(&m_GeneralStyle.m_ItemTooltipBorder_SlateBrush)
		.Padding(20.0f)
		[
			SNew(SVerticalBox)
			// Item name + item color
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SBorder)
				.BorderImage(FCoreStyle::Get().GetBrush("NoBrush"))
				.Padding(0.0f)
				[
					SNew(STextBlock)
					.Text(FText::FromString(m_ItemEntity->GET_Name()))
					.Font(m_GeneralStyle.m_Monorita_Font)
					.Justification(ETextJustify::Center)
					.ColorAndOpacity(FLinearColor::White)
					
				]
			]

			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SBorder)
				.BorderImage(FCoreStyle::Get().GetBrush("NoBrush"))
				.Padding(0.0f)
				[					
					SNew(STextBlock)
					.Text(FText::FromString(FString("Toto")))
					.Font(_ItemCategoryFont)
					.Justification(ETextJustify::Center)
					.ColorAndOpacity(FLinearColor::White)
				]
			]

			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SBox)
				.WidthOverride(50.0f)
				.HeightOverride(100.0f)
				[
					SNew(SBorder)
					//.BorderImage(&m_GeneralStyle.m_DebuggingBlue_SlateBrush)
					.BorderImage(FCoreStyle::Get().GetBrush("NoBrush"))
					.Padding(0.0f)
				]
			]
		];
}

TSharedPtr<SWidget> FItemTooltip::BUILD_Tooltip_Pistol() const
{
	FSlateFontInfo _FontRegular(m_GeneralStyle.m_Roboto_Regular_Font);
	_FontRegular.Size = 12;

	FSlateFontInfo _FontBold(m_GeneralStyle.m_Roboto_Bold_Font);
	_FontBold.Size = 20;

	FSlateFontInfo _FontItalic(m_GeneralStyle.m_Roboto_Italic_Font);
	_FontItalic.Size = 10;


	UEquipableEntity* _Entity = Cast<UEquipableEntity>(m_ItemEntity);
	check(_Entity != nullptr);


	return

		SNew(SBorder)
		.BorderImage(&m_GeneralStyle.m_ItemTooltipBorder_SlateBrush)
		.Padding(20.0f)
		[
			SNew(SVerticalBox)

			// Item name + item color
			//
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(FMargin(0.0f, 0.0f, 0.0f, 15.0f))
			[
				SNew(SBorder)
				.BorderImage(FCoreStyle::Get().GetBrush("NoBrush"))
				.Padding(0.0f)
				[
					SNew(STextBlock)
					.Text(FText::FromString(m_ItemEntity->GET_Name()))
					.Font(_FontBold)
					.Justification(ETextJustify::Center)
					.ColorAndOpacity(_Entity->GET_RaretyColor())					
				]
			]
			
			// Damage
			//
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(FMargin(0.0f, 0.0f, 0.0f, 8.0f))
			[
				SNew(SBorder)
				.BorderImage(FCoreStyle::Get().GetBrush("NoBrush"))
				.Padding(0.0f)
				[					
					SNew(STextBlock)
					.Text(FText::FromString(FString("Damage : ") + FString::FromInt(_Entity->GET_Damage()))) // maybe do it preemptively in UPickupEntity, have one int32 and one FString for "damage amount"
					.Font(_FontRegular)
					.Justification(ETextJustify::Left)
					.ColorAndOpacity(FLinearColor::White)
				]
			]

			// Magazine capacity
			//
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(FMargin(0.0f, 0.0f, 0.0f, 8.0f))
			[

				SNew(SBorder)
				.BorderImage(FCoreStyle::Get().GetBrush("NoBrush"))
				.Padding(0.0f)
				[
					SNew(STextBlock)
					.Text(FText::FromString(FString("Magazine capacity : ") + FString::FromInt(_Entity->GET_MagazineCapacity())))
					.Font(_FontRegular)
					.Justification(ETextJustify::Left)
					.ColorAndOpacity(FLinearColor::White)
				]
			]

			// Rate of fire
			//
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(FMargin(0.0f, 0.0f, 0.0f, 8.0f))
			[

				SNew(SBorder)
				.BorderImage(FCoreStyle::Get().GetBrush("NoBrush"))
				.Padding(0.0f)
				[
					SNew(STextBlock)
					.Text(FText::FromString(FString("Rate of fire : ") + FString::SanitizeFloat(_Entity->GET_RateOfFire())))
					.Font(_FontRegular)
					.Justification(ETextJustify::Left)
					.ColorAndOpacity(FLinearColor::White)
				]
			]

			// Resell value
			//
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(FMargin(0.0f, 15.0f, 0.0f, 8.0f))
			[

				SNew(SBorder)
				.BorderImage(FCoreStyle::Get().GetBrush("NoBrush"))
				.Padding(0.0f)
				[
					SNew(STextBlock)
					.Text(FText::FromString(FString("Resell value : ") + FString::FromInt(_Entity->GET_ResellValue())))
					.Font(_FontRegular)
					.Justification(ETextJustify::Right)
					.ColorAndOpacity(FLinearColor::White)
				]
			]

			// Item description
			//
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(FMargin(0.0f, 15.0f, 0.0f, 0.0f))
			[

				SNew(SBorder)
				.BorderImage(FCoreStyle::Get().GetBrush("NoBrush"))
				.Padding(0.0f)
				[
					SNew(STextBlock)
					.Text(FText::FromString(FString(_Entity->GET_Description())))
					.Font(_FontItalic)
					.Justification(ETextJustify::Center)
					.ColorAndOpacity(FLinearColor::White)
				]
			]
		];
}

TSharedPtr<SWidget> FItemTooltip::BUILD_Tooltip_Ammunition() const
{
	FSlateFontInfo _FontRegular(m_GeneralStyle.m_Roboto_Regular_Font);
	_FontRegular.Size = 12;

	FSlateFontInfo _FontBold(m_GeneralStyle.m_Roboto_Bold_Font);
	_FontBold.Size = 20;

	FSlateFontInfo _FontItalic(m_GeneralStyle.m_Roboto_Italic_Font);
	_FontItalic.Size = 10;


	UNonEquipableEntity* _Entity = Cast<UNonEquipableEntity>(m_ItemEntity);
	check(_Entity != nullptr);

	return

		SNew(SBorder)
		.BorderImage(&m_GeneralStyle.m_ItemTooltipBorder_SlateBrush)
		.Padding(20.0f)
		[
			SNew(SVerticalBox)

			// Item name + item color
			//
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(FMargin(0.0f, 0.0f, 0.0f, 15.0f))
			[
				SNew(SBorder)
				.BorderImage(FCoreStyle::Get().GetBrush("NoBrush"))
				.Padding(0.0f)
				[
					SNew(STextBlock)
					.Text(FText::FromString(m_ItemEntity->GET_Name()))
					.Font(_FontBold)
					.Justification(ETextJustify::Center)
					.ColorAndOpacity(_Entity->GET_RaretyColor())
					
				]
			]
			
			// Damage bonus
			//
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(FMargin(0.0f, 0.0f, 0.0f, 8.0f))
			[
				SNew(SBorder)
				.BorderImage(FCoreStyle::Get().GetBrush("NoBrush"))
				.Padding(0.0f)
				[					
					SNew(STextBlock)
					.Text(FText::FromString(FString("Damage bonus : ") + FString::FromInt(_Entity->GET_DamageBonus()))) // maybe do it preemptively in UPickupEntity, have one int32 and one FString for "damage amount"
					.Font(_FontRegular)
					.Justification(ETextJustify::Left)
					.ColorAndOpacity(FLinearColor::White)
				]
			]

			// Resell value
			//
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(FMargin(0.0f, 15.0f, 0.0f, 8.0f))
			[
				SNew(SBorder)
				.BorderImage(FCoreStyle::Get().GetBrush("NoBrush"))
				.Padding(0.0f)
				[					
					SNew(STextBlock)
					.Text(FText::FromString(FString("Resell value : ") + FString::FromInt(_Entity->GET_ResellValue()))) // maybe do it preemptively in UPickupEntity, have one int32 and one FString for "damage amount"
					.Font(_FontRegular)
					.Justification(ETextJustify::Right)
					.ColorAndOpacity(FLinearColor::White)
				]
			]

			// Item description
			//
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(FMargin(0.0f, 15.0f, 0.0f, 0.0f))
			[
				SNew(SBorder)
				.BorderImage(FCoreStyle::Get().GetBrush("NoBrush"))
				.Padding(0.0f)
				[					
					SNew(STextBlock)
					.Text(FText::FromString(FString(_Entity->GET_Description()))) // maybe do it preemptively in UPickupEntity, have one int32 and one FString for "damage amount"
					.Font(_FontItalic)
					.Justification(ETextJustify::Center)
					.ColorAndOpacity(FLinearColor::White)
				]
			]
		];
}

TSharedPtr<SWindow> FItemTooltip::GET_TooltipWindow() const
{
	return m_TooltipWindow;
}