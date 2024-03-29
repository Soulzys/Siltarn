#include "Slate/Widgets/SInventoryItemWidget.h"
#include "Siltarn/Public/Interactables/PickupEntity.h"
#include "Siltarn/Public/Interactables/EquipableEntity.h"
#include "Siltarn/Public/Interactables/NonEquipableEntity.h"
#include "Siltarn/Public/Slate/Widgets/SInventoryWidget.h"
#include "Styling/CoreStyle.h"

DEFINE_LOG_CATEGORY(LogClass_SInventoryItemWidget   );
DEFINE_LOG_CATEGORY(LogClass_FInventoryItemDragDrop );
DEFINE_LOG_CATEGORY(LogClass_FItemTooltip           );

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

SInventoryItemWidget::SInventoryItemWidget()
{
	m_bIsInInventory          = true  ;
	m_bIsShiftKeyDown         = false ;
	m_bIsSelectedForGroupDrop = false ;

	m_InventoryItemState    = EInventoryItemWidgetState   ::DEFAULT ;
	m_InventoryItemLocation = EInventoryItemWidgetLocation::UNKNOWN ;

	UE_LOG(LogClass_SInventoryItemWidget, Warning, TEXT("I was created !"));
}

SInventoryItemWidget::~SInventoryItemWidget()
{
	UE_LOG(LogClass_SInventoryItemWidget, Warning, TEXT("I was destroyed !"));
}

void SInventoryItemWidget::Construct(const FArguments& p_InArgs)
{
	m_Tile            = p_InArgs._a_Tile; // Luciole ! We are not using m_Tile anymore. However, the game crashed when dealing with two items in the inventory. Need to investigate. Once it is done, remove m_Tile from here.
	m_ItemSize        = p_InArgs._a_ItemSize;
	m_ItemData        = p_InArgs._a_ItemData;
	m_InventoryOwner  = p_InArgs._a_InventoryOwner;
	m_InventoryItemId = p_InArgs._a_InventoryItemId;
	m_InventoryItemLocation = p_InArgs._a_InventoryItemWidgetLocation;
	m_InventoryItemClass = p_InArgs._a_InventoryItemClass;

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


	// Allows the player to receive keyboard inputs as soon the mouse enters the widget
	FSlateApplication::Get().SetUserFocus(0, SharedThis(this), EFocusCause::SetDirectly);
}

void SInventoryItemWidget::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	if (m_BackgroundBorder == nullptr)
	{
		UE_LOG(LogClass_SInventoryItemWidget, Warning, TEXT("OnMouseEnter() : m_BackgroundBorder is NULL ! || Unable to proceed further."));
		return;
	}

	if (!m_bIsSelectedForGroupDrop)
	{
		m_ItemBackgroundColor.A = 0.0f;
		m_BackgroundBorder->SetBorderBackgroundColor(m_ItemBackgroundColor);
	}	

	if (m_ItemTooltip.IsValid())
	{
		m_ItemTooltip->DESTROY_Tooltip();
		m_ItemTooltip.Reset(); // If we do not add this line, the FItemTooltip that is created when the item is hovered for the first time is never deleted 
	}

	// Unfocuses this widget. I have no idea what default widget is focused after. In fact, is this actually supposed to be done this way ? I have no idea. 
	FSlateApplication::Get().ClearUserFocus(0); // Do we need this ? Try to remove it later
	FSlateApplication::Get().SetUserFocusToGameViewport(0);
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
		/*
			Luciole 31/01/2024 | Combining SHIFT + Right mouse button works. The issue now is that the game inputs are also consumed.
			This, for some reason that are probably related to widgets' focus, causes the player to activate the ADS feature.
			A quick fix would be to set the game mode to "UI only" when we open the inventory, but do we actually want that ? 
			Also, why, prior implementing the Keyboard + Mouse clicks, the MouseEvent.GetEffectingButton() == EKeys::RightMouseButton did not trigger
			the ADS ? We need to investigate on that. 

			Luciole 31/01/2024 | I'm too lazy to solve this out right now. Currently switching to "UI only" mode when opening the inventory.
				! Update ! --> In fact, we're switching back to "Game and UI" because "UI only" blocks the other input events, such as pressing "I" to 
				close the inventory when it's open...
		*/
		if (m_bIsShiftKeyDown)
		{
			if (!m_bIsSelectedForGroupDrop && m_InventoryOwner)
			{
				m_ItemBackgroundColor.B = 0.5f;
				m_ItemBackgroundColor.A = 0.5f;
				m_BackgroundBorder->SetBorderBackgroundColor(m_ItemBackgroundColor);

				//m_InventoryOwner->SetInventoryItemForGroupDropping(m_InventoryItemId); // new
				m_InventoryOwner->SetInventoryItemForGroupDropping(m_InventoryItemClass);
				m_InventoryItemState = EInventoryItemWidgetState::SET_FOR_GROUP_DROP;
				m_bIsSelectedForGroupDrop = true;
				return FReply::Handled();
			}
			else
			{
				m_ItemBackgroundColor.B = 1.0f;
				m_ItemBackgroundColor.A = 0.25f;
				m_BackgroundBorder->SetBorderBackgroundColor(m_ItemBackgroundColor);
				m_bIsSelectedForGroupDrop = false;
			}
		}
		else
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

					if (m_InventoryOwner)
					{
						//m_InventoryOwner->MOVE_ItemToCharacterProfileWidget(this);
					}

					// Luciole 12/02/2024 | What is the purpose of this ??
					return DeleteItemWidget();
				}

				return FReply::Handled();
			}
		}		
	}	

	return FReply::Unhandled();
}

FReply SInventoryItemWidget::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::LeftShift)
	{
		if (m_bIsShiftKeyDown)
		{
			return FReply::Handled();
		}

		UE_LOG(LogClass_SInventoryItemWidget, Warning, TEXT("SHIFT key is down !"));
		m_bIsShiftKeyDown = true;
	}

	return FReply::Handled();
}

FReply SInventoryItemWidget::OnKeyUp(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::LeftShift)
	{
		UE_LOG(LogClass_SInventoryItemWidget, Warning, TEXT("SHIFT key is up !"));
		m_bIsShiftKeyDown = false;
	}
	return FReply::Handled();
}

FReply SInventoryItemWidget::OnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		m_InventoryItemState = EInventoryItemWidgetState::DRAGGED;

		const float _Ratio = GET_ScreenToViewportRatio();
		FVector2D _MousePositionLocal = MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());

		// Normal single item drag
		if (!m_bIsSelectedForGroupDrop)
		{
			TSharedRef<FInventoryItemDragDrop> _Operation = FInventoryItemDragDrop::CREATE_SingleItemDragOperation
			(
				this,
				_Ratio,
				m_ItemSize,
				m_ItemData,
				m_Tile, 
				m_InventoryItemClass, 
				m_InventoryItemId
			);

			SetVisibility(EVisibility::Collapsed);

			return FReply::Handled().BeginDragDrop(_Operation);
		}
		else
		{
			if (m_InventoryOwner)
			{
				TSharedRef<FInventoryItemDragDrop> _Operation = FInventoryItemDragDrop::CREATE_MultipleItemsDragOperation
				(
					//m_InventoryOwner->BeginDraggingGroupItemsForDropping(), // old
					//m_InventoryOwner->GET_FInventoryItemsCache(), // new 
					_Ratio, 
					m_ItemSize, 
					&m_GeneralStyle.m_ItemBagIcon_SlateBrush
				);

				//SetVisibility(EVisibility::Collapsed); // old
				m_InventoryOwner->HideAllItemsSetForGroupDrop(); // new

				return FReply::Handled().BeginDragDrop(_Operation);
			}
		}		
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

	if (GEngine->GameViewport.Get() == nullptr)
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
	Drag & drop Item Operation Class
/**************************************************************************************/

int32 FInventoryItemDragDrop::m_InstanceCount = 0;

FInventoryItemDragDrop::FInventoryItemDragDrop()
{
	m_bIsSingleItemDrag = true;

	m_InstanceCount++;
	UE_LOG(LogClass_FInventoryItemDragDrop, Log, TEXT("A new instance was created ! || Instance count : %d"), m_InstanceCount);
}

FInventoryItemDragDrop::~FInventoryItemDragDrop()
{
	if (m_bIsSingleItemDrag)
	{
		if (m_DraggedItem)
		{
			m_DraggedItem->SetVisibility(EVisibility::Visible);
		}
	}
	
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

TSharedRef<FInventoryItemDragDrop> FInventoryItemDragDrop::CREATE_SingleItemDragOperation
(
	SInventoryItemWidget* p_DraggedItem           , 
	const float           p_ScreenToViewportRatio , 
	const FVector2D&      p_WidgetSize            , 
	UPickupEntity*        p_ItemEntity            ,
	FTile*                p_Tile, 
	FInventoryItem* p_InventoryItemClass,
	int32 p_InventoryItemId
)
{
	TSharedRef<FInventoryItemDragDrop> _Operation = MakeShareable(new FInventoryItemDragDrop);

	_Operation->m_IconBrush.SetResourceObject(p_ItemEntity->GET_Icon());

	_Operation->m_DraggedItem           = p_DraggedItem           ;
	_Operation->m_DecoratorSize         = p_WidgetSize            ;
	_Operation->m_ScreenToViewportRatio = p_ScreenToViewportRatio ;
	_Operation->m_ItemEntity            = p_ItemEntity            ;
	_Operation->m_Tile                  = p_Tile                  ;
	_Operation->m_InventoryItemid       = p_InventoryItemId       ;
	_Operation->m_InventoryItemClass = p_InventoryItemClass;

	_Operation->m_bIsSingleItemDrag = true;
	_Operation->Construct();

	return _Operation;
}

TSharedRef<FInventoryItemDragDrop> FInventoryItemDragDrop::CREATE_MultipleItemsDragOperation
(
	//TArray<UPickupEntity*>& p_ItemEntities          ,
	//TArray<FInventoryItem*>& p_InventoryItems,
	const float             p_ScreenToViewportRatio ,
	const FVector2D&        p_WidgetSize            ,
	const FSlateBrush*      p_DecoratorIcon
)
{
	TSharedRef<FInventoryItemDragDrop> _Operation = MakeShareable(new FInventoryItemDragDrop);

	_Operation->m_IconBrush             = *p_DecoratorIcon        ; // old
	//_Operation->m_InventoryItems = &p_InventoryItems; // new 
	_Operation->m_ScreenToViewportRatio = p_ScreenToViewportRatio ;
	//_Operation->m_ItemEntities          = &p_ItemEntities         ;
	_Operation->m_DecoratorSize         = p_WidgetSize            ;

	_Operation->m_bIsSingleItemDrag = false;
	_Operation->Construct();

	// Create ItemBagEntity ! 
	// Wait... do we actually need one ? 


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