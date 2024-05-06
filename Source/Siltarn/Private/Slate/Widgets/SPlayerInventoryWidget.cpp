#include "Slate/Widgets/SPlayerInventoryWidget.h"
#include "Siltarn/Public/Slate/Widgets/SItemWidget.h"
#include "Siltarn/Public/Interactables/PickupEntity.h"
#include "Siltarn/Public/Inventory/InventoryManager.h"

DEFINE_LOG_CATEGORY(LogClass_SPlayerInventoryWidget);


SPlayerInventoryWidget::SPlayerInventoryWidget()
{
	UE_LOG(LogClass_SPlayerInventoryWidget, Error, TEXT("I was constructed !"));
}



SPlayerInventoryWidget::~SPlayerInventoryWidget()
{
	m_DroppingItemsCacheNew.Empty();
	m_InventoryItemsMapNew.Empty();

	UE_LOG(LogClass_SPlayerInventoryWidget, Error, TEXT("I was destroyed !"));
}



bool SPlayerInventoryWidget::AddItemToInventoryNew(UPickupEntity* p_ItemEntity)
{
	if (!p_ItemEntity)
	{
		UE_LOG(LogClass_SPlayerInventoryWidget, Error, TEXT("AddItemToInventoryNew() : p_ItemEntity is NULL !"));
		CleanCachedTilesIndexes();
		return false;
	}

	if (m_CachedTileIndex < 0 || m_CachedTileIndex >= m_Tiles.Num())
	{
		UE_LOG(LogClass_SPlayerInventoryWidget, Error, TEXT("AddItemToInventoryNew() : there is an issue with m_CachedTileIndex !"));
		CleanCachedTilesIndexes();
		return false;
	}

	if (m_CachedTilesIndexes.IsEmpty())
	{
		UE_LOG(LogClass_SPlayerInventoryWidget, Error, TEXT("AddItemToInventoryNew() : Could not retrieved the cached FTiles !"));
		return false;
	}


	FTile* _NewItemControlTile = &m_Tiles[m_CachedTileIndex];

	if (_NewItemControlTile && !_NewItemControlTile->IS_Occupied())
	{
		TSharedPtr<SItemWidget> _ItemWidget = ConstructItemWidget(p_ItemEntity, _NewItemControlTile, EItemWidgetLocation::PLAYER_INVENTORY);

		if (_ItemWidget.IsValid())
		{
			m_ItemsMap.Emplace(p_ItemEntity->GET_EntityId(), _ItemWidget);
			p_ItemEntity->SET_InventoryLocationTile(_NewItemControlTile->GET_TileCoordinates());

			CleanCachedTilesIndexes();
			return true;
		}
	}

	return false;
}



bool SPlayerInventoryWidget::RemoveItemCanvasSlot(int64 p_InventoryItemId)
{
	if (p_InventoryItemId >= 0 && m_Canvas)
	{
		//FInventoryItem* _InventoryItem = m_InventoryItemsMapNew.FindAndRemoveChecked(p_InventoryItemId); // old
		TSharedPtr<SItemWidget> _ItemWidget = m_ItemsMap.FindAndRemoveChecked(p_InventoryItemId); // new

		// new
		if (_ItemWidget)
		{
			_ItemWidget->FreeOccupiedTiles();
			m_Canvas->RemoveSlot(_ItemWidget.ToSharedRef());
			
			return true;
		}

		// old
		/*if (_InventoryItem)
		{
			m_Canvas->RemoveSlot(_InventoryItem->GET_ItemWidget().ToSharedRef());
			_InventoryItem->FreeOccupiedTiles();
			delete _InventoryItem;

			return true;
		}*/
	}

	return false;
}



bool SPlayerInventoryWidget::RemoveItemsCanvasSlotNew()
{
	for (int32 i = 0; i < m_DroppingItemsCacheNew.Num(); i++)
	{
		TSharedPtr<SItemWidget> _ItemWidget = m_DroppingItemsCacheNew[i];

		if (_ItemWidget)
		{
			_ItemWidget->FreeOccupiedTiles();
			int64 _Key = _ItemWidget->GET_ItemData()->GET_EntityId();
			m_Canvas->RemoveSlot(_ItemWidget.ToSharedRef());
			m_ItemsMap.FindAndRemoveChecked(_Key);
		}
		else
		{
			return false;
		}
	}

	m_DroppingItemsCacheNew.Empty();

	return true;
}



void SPlayerInventoryWidget::SetItemForGroupDropping(TSharedPtr<SItemWidget> p_ItemWidget)
{
	if (p_ItemWidget)
	{
		m_DroppingItemsCacheNew.Emplace(p_ItemWidget);
	}
}



void SPlayerInventoryWidget::RemoveItemFromGroupDropping(TSharedPtr<SItemWidget> p_ItemWidget)
{
	if (p_ItemWidget)
	{
		m_DroppingItemsCacheNew.Remove(p_ItemWidget);
	}
}



void SPlayerInventoryWidget::HideItemsSetForGroupDrop()
{
	for (auto _ItemWidget : m_DroppingItemsCacheNew)
	{
		_ItemWidget->SetVisibility(EVisibility::Hidden);
	}
}



void SPlayerInventoryWidget::SetItemsForGroupDrop()
{
	if (m_InventoryManager)
	{
		for (int32 i = 0; i < m_DroppingItemsCacheNew.Num(); i++)
		{
			UPickupEntity* _ItemEntity = m_DroppingItemsCacheNew[i]->GET_ItemData();

			if (_ItemEntity)
			{
				m_InventoryManager->SetItemForGroupDrop(_ItemEntity);
			}
		}

		m_InventoryManager->DropItems();
	}
}