#include "Slate/Menus/SProfileMenu.h"
#include "Siltarn/Public/Slate/Widgets/SInventoryWidget.h"
#include "Siltarn/Public/Slate/Widgets/SInventoryItemWidget.h"
#include "Siltarn/Public/Slate/Widgets/SCharacterProfileWidget.h"
#include "Siltarn/Public/Slate/Widgets/SInGamePlayerInventoryWidget.h"
#include "Siltarn/Public/Slate/Widgets/SInGameBagInventory.h"
#include "Siltarn/Public/Interactables/PickupEntity.h"
#include "Siltarn/Public/Characters/SiltarnPlayerController.h"
#include "Siltarn/Public/Inventory/InventoryManager.h"

DEFINE_LOG_CATEGORY(LogClass_SProfileMenu);

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SProfileMenu::Construct(const FArguments& p_InArgs)
{
	m_PlayerInventoryNumberOfColumns = p_InArgs._a_NumberOfColumns.Get();
	m_PlayerInventoryNumberOfRows    = p_InArgs._a_NumberOfRows   .Get();
	m_TileSize                       = p_InArgs._a_TileSize       .Get();
	m_HUDOwner                       = p_InArgs._a_HUDOwner             ;

	ChildSlot
	.HAlign(HAlign_Fill)
	.VAlign(VAlign_Fill)
	[
		SNew(SHorizontalBox)

		// External inventory (bag, chest, lootbox...
		//
		+ SHorizontalBox::Slot()
		[
			SNew(SBorder)
			//.HAlign(HAlign_Center)
			//.VAlign(VAlign_Center)
			//.BorderImage(FCoreStyle::Get().GetBrush("NoBrush"))
			.BorderImage(&m_GeneralStyle.m_DebuggingBlue_SlateBrush)
			.Padding(0)
			[
				SNew(SBox)
				.WidthOverride(m_ExternalInventoryNumberOfColumns * m_TileSize)
				.HeightOverride(m_ExternalInventoryNumberOfRows   * m_TileSize)
				[
					/*SNew(SBorder)
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					.BorderImage(&m_GeneralStyle.m_DebuggingYellow_SlateBrush)
					.Padding(0)
					[*/
						SAssignNew(m_ExternalInventoryWidget, SInGameBagInventory)
						.a_HUDOwner(m_HUDOwner)
						.a_NumberOfColumns(m_ExternalInventoryNumberOfColumns)
						.a_NumberOfRows(m_ExternalInventoryNumberOfRows)
						.a_TileSize(m_TileSize)
						.a_ParentWidget(this)
					//]					
				]
			]
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
						.WidthOverride(m_PlayerInventoryNumberOfColumns * m_TileSize)
						.HeightOverride(650.0f) // Luciole 28/01/2024 | Need to turn this into a variable. Do not forget to change a_CanvasSize below
						[
							/*SNew(SBorder)
							.BorderImage(FCoreStyle::Get().GetBrush("NoBrush"))
							//.BorderImage(&m_GeneralStyle.m_DebuggingBlue_SlateBrush)
							.Padding(0)*/

							SAssignNew(m_CharacterProfileWidget, SCharacterProfileWidget)
							.a_CanvasSize(FVector2D(m_PlayerInventoryNumberOfColumns * m_TileSize, 650.0f))
							.a_ItemSlotSize(FVector2D(75.0f)) // Luciole 28/01/2024 | Need to turn this into a variable, perhaps set in the HUD ? 
							.a_TileSize((float)m_TileSize)
						]
					]					
				]
					
				// Character inventory
				//
				+ SVerticalBox::Slot()
				[
					SNew(SBorder)
					.BorderImage(&m_GeneralStyle.m_DebuggingRed_SlateBrush)
					.Padding(0)
					[
						SNew(SBox)
						.WidthOverride (m_PlayerInventoryNumberOfColumns * m_TileSize)
						.HeightOverride(m_PlayerInventoryNumberOfRows    * m_TileSize)
						[
							//SAssignNew(m_InventoryWidget, SInventoryWidget)
							SAssignNew(m_InventoryWidget, SInGamePlayerInventoryWidget)
							.a_NumberOfColumns(m_PlayerInventoryNumberOfColumns)
							.a_NumberOfRows   (m_PlayerInventoryNumberOfRows   )
							.a_TileSize       (m_TileSize       )
							.a_HUDOwner       (m_HUDOwner       )
							.a_ParentWidget   (this             )
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
				//.BorderImage(FCoreStyle::Get().GetBrush("NoBrush"))
				.BorderImage(&m_GeneralStyle.m_DebuggingGreen_SlateBrush)
				.Padding(0)
			]			
		]
	];
}

FReply SProfileMenu::OnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent)
{
	TSharedPtr<FInventoryItemDragDrop> _DragOperation = InDragDropEvent.GetOperationAs<FInventoryItemDragDrop>();

	if (_DragOperation.IsValid())
	{
		if (_DragOperation->IS_SingleItemDragOperation())
		{
			if (m_InventoryManager)
			{
				m_InventoryManager->DropItem(_DragOperation->GET_ItemEntity());
				return FReply::Handled();
			}
		}
		else
		{
			if (m_InventoryWidget)
			{
				m_InventoryWidget->SetItemsForGroupDrop();
				return FReply::Handled();
			}
		}
	}	

	return FReply::Unhandled();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION



bool SProfileMenu::DOES_InventoryHasRoomForItem(const FIntPoint& p_ItemSize)
{
	return m_InventoryWidget->IsThereRoomInInventory(p_ItemSize);
}



bool SProfileMenu::MOVE_ItemToCharacterProfileMenu(SInventoryItemWidget* p_ItemWidget)
{
	if (m_CharacterProfileWidget && p_ItemWidget)
	{
		m_CharacterProfileWidget->PLACE_ItemInSlot(p_ItemWidget);
		return true;
	}

	return false;
}



void SProfileMenu::SET_InventoryManager(UInventoryManager* p_InventoryManager)
{
	m_InventoryManager = p_InventoryManager;
}



void SProfileMenu::OpenExternalInventoryWidget(const FIntPoint& p_InventorySize, int32 p_TileSize, TArray<UPickupEntity*>& p_Items)
{
	if (m_ExternalInventoryWidget.IsValid())
	{
		// Reconstruct inventory
		SInventoryWidget::FArguments _Args         ;
		_Args.a_NumberOfColumns(p_InventorySize.X) ;
		_Args.a_NumberOfRows   (p_InventorySize.Y) ;
		_Args.a_TileSize       (p_TileSize)        ;
		_Args.a_HUDOwner       (m_HUDOwner)        ;
		_Args.a_ParentWidget   (this)              ;

		m_ExternalInventoryWidget->Reconstruct(_Args)                  ;
		m_ExternalInventoryWidget->SetVisibility(EVisibility::Visible) ;
		m_ExternalInventoryWidget->LoadItemsWidgets(p_Items)           ;
	}
	else
	{
		UE_LOG(LogClass_SProfileMenu, Error, TEXT("OpenExternalInventoryWidget() : m_ExternalInventoryWidget is invalid !"));
	}
}



void SProfileMenu::CloseExternalInventoryWidget()
{
	if (m_ExternalInventoryWidget.IsValid() && m_ExternalInventoryWidget->GetVisibility() == EVisibility::Visible)
	{
		m_ExternalInventoryWidget->ClearInventoryVisual();
		m_ExternalInventoryWidget->SetVisibility(EVisibility::Collapsed);
	}
}



FReply SProfileMenu::OnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::I || InKeyEvent.GetKey() == EKeys::Escape)
	{
		if (m_HUDOwner.IsValid())
		{
			m_HUDOwner->CloseCharacterProfileWidget();
		}
	}

	if (InKeyEvent.GetKey() == EKeys::T)
	{
		UE_LOG(LogClass_SProfileMenu, Warning, TEXT("T was pressed !"));
	}

	return FReply::Handled();
}