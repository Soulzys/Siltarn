#include "Slate/Widgets/SExternalInventoryWidget.h"
#include "Siltarn/Public/Interactables/PickupEntity.h"
#include "Siltarn/Public/Slate/Widgets/SItemWidget.h"
#include "Siltarn/Public/Inventory/InventoryManager.h"
#include "Siltarn/Public/Interactables/ItemBagActor.h"

DEFINE_LOG_CATEGORY(LogClass_SInGameBagInventory);

SExternalInventoryWidget::~SExternalInventoryWidget()
{
	/*for (auto _it : m_InventoryItemsMap)
	{
		FInventoryItem* _InventoryItem = _it.Value;

		if (_InventoryItem)
		{
			// Not sure if this is useful. All SCanvas FSlot are probably automatically removed when we close the game anyway... 
			m_Canvas->RemoveSlot(_InventoryItem->GET_ItemWidget().ToSharedRef());
			delete _InventoryItem;
		}
	}*/
}



void SExternalInventoryWidget::Construct(const FArguments p_InArgs)
{
	SInventoryWidget::Construct(p_InArgs);
}



void SExternalInventoryWidget::LoadItemsWidgetsNew(TArray<UPickupEntity*>& p_Items)
{
	for (int32 i = 0; i < p_Items.Num(); i++)
	{
		UPickupEntity* _ItemEntity = p_Items[i];

		if (_ItemEntity)
		{
			TSharedPtr<SItemWidget> _ItemWidget = ConstructItemWidget(_ItemEntity, EItemWidgetLocation::EXTERNAL_INVENTORY);

			if (_ItemWidget)
			{
				m_ItemsMap.Emplace(_ItemEntity->GET_EntityId(), _ItemWidget);
			}
		}
	}
}



void SExternalInventoryWidget::MoveItemToPlayerInventory(TSharedPtr<SItemWidget> p_ItemWidget)
{
	if (m_InventoryManager)
	{
		UPickupEntity* _ItemEntity = p_ItemWidget->GET_ItemData();

		bool _bDoesPlayerInventoryHasEnoughRoom = m_InventoryManager->DoesPlayerInventoryHasRoomForNewItem(_ItemEntity->GET_InventorySpace());

		if (_bDoesPlayerInventoryHasEnoughRoom)
		{
			/* Removing SInventoryItemWidget */
			p_ItemWidget->FreeOccupiedTiles();
			m_ItemsMap.FindAndRemoveChecked(_ItemEntity->GET_EntityId()); // Removes from TMap
			m_Canvas->RemoveSlot(p_ItemWidget.ToSharedRef());             // Destroy SInventoryItemWidget


			bool _bWasMovedToPlayerInventory = m_InventoryManager->MoveItemToPlayerInventory(_ItemEntity);

			if (_bWasMovedToPlayerInventory)
			{
				if (m_BagActor)
				{
					m_BagActor->RemoveItem(_ItemEntity);
				}
			}
		}
	}
}



void SExternalInventoryWidget::RemoveItemCanvasSlot(int64 p_ItemKey)
{
	if (p_ItemKey >= 0 && m_Canvas)
	{
		//TSharedPtr<
	}
}



void SExternalInventoryWidget::SET_BagActor(AItemBagActor* p_BagActor)
{
	m_BagActor = p_BagActor;
}