#include "Slate/Widgets/SInGameBagInventory.h"
#include "Siltarn/Public/Interactables/PickupEntity.h"
#include "Siltarn/Public/Slate/Widgets/SInventoryItemWidget.h"

DEFINE_LOG_CATEGORY(LogClass_SInGameBagInventory);

SInGameBagInventory::~SInGameBagInventory()
{
	for (auto _it : m_InventoryItemsMap)
	{
		FInventoryItem* _InventoryItem = _it.Value;

		if (_InventoryItem)
		{
			// Not sure if this is useful. All SCanvas FSlot are probably automatically removed when we close the game anyway... 
			m_Canvas->RemoveSlot(_InventoryItem->GET_ItemWidget().ToSharedRef());
			delete _InventoryItem;
		}
	}
}

void SInGameBagInventory::Reconstruct(const SInventoryWidget::FArguments& p_Args)
{
	Construct(p_Args);
}

void SInGameBagInventory::LoadItemsWidgets(TArray<UPickupEntity*>& p_Items)
{
	for (int32 i = 0; i < p_Items.Num(); i++)
	{
		UPickupEntity* _ItemEntity = p_Items[i];

		if (_ItemEntity)
		{
			FInventoryItem* _NewInventoryItem = new FInventoryItem(_ItemEntity->GET_InventoryLocationTile(), _ItemEntity->GET_InventorySpace(), _ItemEntity->GET_EntityId(), m_NumberOfColumns, m_Tiles);
			
			if (_NewInventoryItem)
			{
				ConstructCanvasItemSlot(_ItemEntity, _NewInventoryItem->GET_ControlTile(), _NewInventoryItem, EInventoryItemWidgetLocation::EXTERNAL_INVENTORY);
				m_InventoryItemsMap.Emplace(_ItemEntity->GET_EntityId(), _NewInventoryItem);

				// Debug
				FChildren* _Children = m_Canvas->GetAllChildren();
				UE_LOG(LogClass_SInGameBagInventory, Error, TEXT("Children names : %s"), *_Children->GetName().ToString());
				//_NewInventoryItem->GET_ItemWidget()->GetNa
			}
		}		
	}
}