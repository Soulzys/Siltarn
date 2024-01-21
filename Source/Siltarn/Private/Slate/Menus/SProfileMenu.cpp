#include "Slate/Menus/SProfileMenu.h"
#include "Siltarn/Public/Slate/Widgets/SInventoryWidget.h"
#include "Siltarn/Public/Slate/Widgets/SInventoryItemWidget.h"
#include "Siltarn/Public/Slate/Widgets/SCharacterProfileWidget.h"
#include "Siltarn/Public/Interactables/PickupEntity.h"
#include "Siltarn/Public/Characters/SiltarnPlayerController.h"

DEFINE_LOG_CATEGORY(LogClass_SProfileMenu);

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SProfileMenu::Construct(const FArguments& p_InArgs)
{
	m_NumberOfColumns = p_InArgs._a_NumberOfColumns.Get();
	m_NumberOfRows    = p_InArgs._a_NumberOfRows   .Get();
	m_TileSize        = p_InArgs._a_TileSize       .Get();

	m_HUDOwner = p_InArgs._a_HUDOwner;

	
	ChildSlot
	.HAlign(HAlign_Fill)
	.VAlign(VAlign_Fill)
	[
		SNew(SHorizontalBox)

		+ SHorizontalBox::Slot()
		[
			SNew(SBorder)
			.BorderImage(FCoreStyle::Get().GetBrush("NoBrush"))
			.Padding(0)
		]

		// Character profile (equipment + inventory)
		//
		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SBorder)
			.BorderImage(&m_GeneralStyle.m_DebuggingYellow_SlateBrush)
			.Padding(FMargin(10.0f))
			[
				SNew(SVerticalBox)
					
				// Character equipment
				//
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SBorder)
					.BorderImage(&m_GeneralStyle.m_DebuggingBlue_SlateBrush)
					.Padding(0)
					[
						SNew(SBox)
						.WidthOverride(m_NumberOfColumns * m_TileSize)
						.HeightOverride(500.0f)
						[
							SNew(SBorder)
							.BorderImage(FCoreStyle::Get().GetBrush("NoBrush"))
							//.BorderImage(&m_GeneralStyle.m_DebuggingBlue_SlateBrush)
							.Padding(0)
						]
					]
				]
					
				// Inventory
				//
				+ SVerticalBox::Slot()
				[
					SNew(SBorder)
					.BorderImage(&m_GeneralStyle.m_DebuggingRed_SlateBrush)
					.Padding(0)
					[
						SNew(SBox)
						.WidthOverride (m_NumberOfColumns * m_TileSize)
						.HeightOverride(m_NumberOfRows    * m_TileSize)
						[
							SAssignNew(m_InventoryWidget, SInventoryWidget)
							.a_NumberOfColumns(m_NumberOfColumns)
							.a_NumberOfRows   (m_NumberOfRows   )
							.a_TileSize       (m_TileSize       )
							.a_HUDOwner       (m_HUDOwner       )
						]
					]
				]
			]				
			
		]

		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SBox)
			.WidthOverride(50.0f)
			[
				SNew(SBorder)
				.BorderImage(FCoreStyle::Get().GetBrush("NoBrush"))
				//.BorderImage(&m_GeneralStyle.m_DebuggingGreen_SlateBrush)
				.Padding(0)
			]			
		]
		

		/*SNew(SCanvas)
		
		+ SCanvas::Slot()
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Center)
		[
			SNew(SBorder)
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Center)
			.Padding(0)
			[
				SNew(SBox)
				.WidthOverride (m_NumberOfColumns * m_TileSize)
				.HeightOverride(m_NumberOfRows    * m_TileSize)
				[
					SAssignNew(m_InventoryWidget, SInventoryWidget)
					.a_NumberOfColumns(m_NumberOfColumns)
					.a_NumberOfRows   (m_NumberOfRows   )
					.a_TileSize       (m_TileSize       )
					.a_HUDOwner       (m_HUDOwner       )
				]
			]
		]*/
		

		/*SNew(SBorder)
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Center)
		.Padding(0)
		[
			SNew(SBox)
			.WidthOverride (m_NumberOfColumns * m_TileSize)
			.HeightOverride(m_NumberOfRows    * m_TileSize)
			[
				SAssignNew(m_InventoryWidget, SInventoryWidget)
				.a_NumberOfColumns(m_NumberOfColumns)
				.a_NumberOfRows   (m_NumberOfRows   )
				.a_TileSize       (m_TileSize       )
				.a_HUDOwner       (m_HUDOwner       )
			]
		]	*/	
	];
}

FReply SProfileMenu::OnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent)
{
	TSharedPtr<FInventoryItemDragDrop> _Operation = InDragDropEvent.GetOperationAs<FInventoryItemDragDrop>();

	if (_Operation.IsValid())
	{
		UE_LOG(LogClass_SProfileMenu, Warning, TEXT("We've sensed a drop !"));
	}

	ASiltarnPlayerController* _SPC = Cast<ASiltarnPlayerController>(m_HUDOwner->GetOwningPlayerController());
	check(_SPC);

	_SPC->DROP_Item(_Operation->GET_ItemEntity());

	m_InventoryWidget->REMOVE_Item(_Operation->GET_Tile());

	return FReply::Handled();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

bool SProfileMenu::DOES_InventoryHasRoomForItem(const FIntPoint& p_ItemSize)
{
	return m_InventoryWidget->TRY_AddingItemToInventory(p_ItemSize);
}

bool SProfileMenu::ADD_ItemToInventory(UPickupEntity* p_Item)
{
	if (p_Item)
	{
		return m_InventoryWidget->ADD_Item(p_Item);
	}

	return false;
}