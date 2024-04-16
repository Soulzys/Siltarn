#include "Slate/Widgets/SInGamePlayerInventoryWidget.h"
#include "Siltarn/Public/Slate/Widgets/SItemWidget.h"
#include "Siltarn/Public/Interactables/PickupEntity.h"

DEFINE_LOG_CATEGORY(LogClass_SInGamePlayerInventoryWidget);


SInGamePlayerInventoryWidget::SInGamePlayerInventoryWidget()
{
	UE_LOG(LogClass_SInGamePlayerInventoryWidget, Error, TEXT("I was constructed !"));
}



SInGamePlayerInventoryWidget::~SInGamePlayerInventoryWidget()
{
	/*for (auto _it : m_InventoryItemsMapNew)
	{
		FInventoryItem* _InventoryItem = _it.Value;

		if (_InventoryItem)
		{
			// Not sure if this is useful. All SCanvas FSlot are probably automatically removed when we close the game anyway... 
			m_Canvas->RemoveSlot(_InventoryItem->GET_ItemWidget().ToSharedRef()); 
			delete _InventoryItem;
		}		
	}*/

	m_DroppingItemsCache.Empty();
	m_InventoryItemsMapNew.Empty();

	UE_LOG(LogClass_SInGamePlayerInventoryWidget, Error, TEXT("I was destroyed !"));
}



bool SInGamePlayerInventoryWidget::AddItemToInventoryNew(UPickupEntity* p_ItemEntity)
{
	if (!p_ItemEntity)
	{
		UE_LOG(LogClass_SInGamePlayerInventoryWidget, Error, TEXT("AddItemToInventoryNew() : p_ItemEntity is NULL !"));
		CleanCachedTilesIndexes();
		return false;
	}

	if (m_CachedTileIndex < 0 || m_CachedTileIndex >= m_Tiles.Num())
	{
		UE_LOG(LogClass_SInGamePlayerInventoryWidget, Error, TEXT("AddItemToInventoryNew() : there is an issue with m_CachedTileIndex !"));
		CleanCachedTilesIndexes();
		return false;
	}

	if (m_CachedTilesIndexes.IsEmpty())
	{
		UE_LOG(LogClass_SInGamePlayerInventoryWidget, Error, TEXT("AddItemToInventoryNew() : Could not retrieved the cached FTiles !"));
		return false;
	}


	FTile* _NewItemControlTile = &m_Tiles[m_CachedTileIndex];

	if (_NewItemControlTile && !_NewItemControlTile->IS_Occupied())
	{
		TSharedPtr<SItemWidget> _ItemWidget = ConstructItemWidget(p_ItemEntity, _NewItemControlTile, EInventoryItemWidgetLocation::PLAYER_INVENTORY);

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



bool SInGamePlayerInventoryWidget::RemoveItemCanvasSlot(int64 p_InventoryItemId)
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



bool SInGamePlayerInventoryWidget::RemoveItemsCanvasSlotNew()
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