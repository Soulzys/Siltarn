#include "Slate/Widgets/SInGamePlayerInventoryWidget.h"
#include "Siltarn/Public/Slate/Widgets/SInventoryItemWidget.h"
#include "Siltarn/Public/Interactables/PickupEntity.h"

DEFINE_LOG_CATEGORY(LogClass_SInGamePlayerInventoryWidget);


SInGamePlayerInventoryWidget::SInGamePlayerInventoryWidget()
{
	UE_LOG(LogClass_SInGamePlayerInventoryWidget, Error, TEXT("I was constructed !"));
}

SInGamePlayerInventoryWidget::~SInGamePlayerInventoryWidget()
{
	for (auto _it : m_InventoryItemsMapNew)
	{
		FInventoryItem* _InventoryItem = _it.Value;

		if (_InventoryItem)
		{
			// Not sure if this is useful. All SCanvas FSlot are probably automatically removed when we close the game anyway... 
			m_Canvas->RemoveSlot(_InventoryItem->GET_ItemWidget().ToSharedRef()); 
			delete _InventoryItem;
		}		
	}

	m_DroppingItemsCache.Empty();
	m_InventoryItemsMapNew.Empty();

	UE_LOG(LogClass_SInGamePlayerInventoryWidget, Error, TEXT("I was destroyed !"));
}

bool SInGamePlayerInventoryWidget::AddItemToInventory(UPickupEntity* p_ItemEntity)
{
	if (!p_ItemEntity)
	{
		UE_LOG(LogClass_SInGamePlayerInventoryWidget, Error, TEXT("AddItemToInventoryNew() : p_ItemEntity is NULL !"));
		CleanCachedTilesIndexes();
		return false;
	}
	// Old
	if (m_CachedTileIndex < 0 || m_CachedTileIndex >= m_Tiles.Num())
	{
		UE_LOG(LogClass_SInGamePlayerInventoryWidget, Error, TEXT("AddItemToInventoryNew() : there is an issue with m_CachedTileIndex !"));
		CleanCachedTilesIndexes();
		return false;
	}
	/*if (m_CachedTileIndex < 0 || m_CachedTileIndex >= m_TilesNew.Num())
	{
		UE_LOG(LogClass_SInGamePlayerInventoryWidget, Error, TEXT("AddItemToInventoryNew() : there is an issue with m_CachedTileIndex !"));
		CleanCachedTilesIndexes();
		return false;
	}*/

	// Old
	FTile* _NewItemControlTile = &m_Tiles[m_CachedTileIndex];

	// New
	//FTile* _NewItemControlTile = m_TilesNew[m_CachedTileIndex];

	if (_NewItemControlTile && !_NewItemControlTile->IS_Occupied())
	{
		// Old
		FInventoryItem* _NewInventoryItem = new FInventoryItem(m_CachedTilesIndexes, m_Tiles, p_ItemEntity->GET_EntityId());

		// New
		//FInventoryItem* _NewInventoryItem = new FInventoryItem(m_CachedTilesIndexes, m_TilesNew, p_ItemId);

		if (_NewInventoryItem)
		{
			// Luciole 11/03/2024 || Should probably check whether the Canvas' slot construction went well before adding the inventory item to the TMap
			ConstructCanvasItemSlot(p_ItemEntity, _NewItemControlTile, _NewInventoryItem, EInventoryItemWidgetLocation::PLAYER_INVENTORY);
			//m_InventoryItemsMapNew.Emplace(p_ItemId, _NewInventoryItem); // old
			m_InventoryItemsMapNew.Emplace(p_ItemEntity->GET_EntityId(), _NewInventoryItem); // New
			CleanCachedTilesIndexes();
			return true;
		}
	}

	UE_LOG(LogClass_SInGamePlayerInventoryWidget, Error, TEXT("AddItemToInventoryNew() : _TilePtr is occupied, which should never happen..."));
	CleanCachedTilesIndexes();
	return false;
}

bool SInGamePlayerInventoryWidget::RemoveItemCanvasSlot(int64 p_InventoryItemId)
{
	if (p_InventoryItemId >= 0 && m_Canvas)
	{
		FInventoryItem* _InventoryItem = m_InventoryItemsMapNew.FindAndRemoveChecked(p_InventoryItemId);

		if (_InventoryItem)
		{
			m_Canvas->RemoveSlot(_InventoryItem->GET_ItemWidget().ToSharedRef());
			_InventoryItem->FreeOccupiedTiles();
			delete _InventoryItem;

			//UE_LOG(LogClass_SInGamePlayerInventoryWidget, Warning, TEXT("There are %d slots left in SCanvas !"), m_Canvas->Numb)

			return true;
		}
	}

	return false;
}

bool SInGamePlayerInventoryWidget::RemoveItemsCanvasSlot()
{
	UE_LOG(LogClass_SInGamePlayerInventoryWidget, Error, TEXT("Checking checking in inventory... (1)"));

	UE_LOG(LogClass_SInGamePlayerInventoryWidget, Warning, TEXT("There are %d items that are about to get dropped !"), m_DroppingItemsCache.Num());

	for (int32 i = 0; i < m_DroppingItemsCache.Num(); i++)
	{
		FInventoryItem* _InventoryItem = m_DroppingItemsCache[i];

		if (_InventoryItem)
		{
			UE_LOG(LogClass_SInGamePlayerInventoryWidget, Error, TEXT("Checking checking in inventory loop (TRUE)... (%d)"), i);
			_InventoryItem->FreeOccupiedTiles();
			//m_DroppingItemsCache[i] = nullptr;

			int64 _Key = _InventoryItem->GET_UniqueId();

			//m_DroppingItemsCache.RemoveAt(i);
			//_InventoryItem->GET_ItemWidget()->Dest
			// Luciole 24/03/2024 || We sometimes get a TSharedPtr error coming from the line below... Need to investigate ASAP
			m_Canvas->RemoveSlot(_InventoryItem->GET_ItemWidget().ToSharedRef());
			delete _InventoryItem;
			m_InventoryItemsMapNew.FindAndRemoveChecked(_Key);

			UE_LOG(LogClass_SInGamePlayerInventoryWidget, Warning, TEXT("All inventory items count : %d"), m_InventoryItemsMapNew.Num());
		}
		else
		{
			UE_LOG(LogClass_SInGamePlayerInventoryWidget, Error, TEXT("Checking checking in inventory loop (FALSE)... (%d)"), i);
			return false;
		}
	}

	/*for (int32 i = 0; i < m_DroppingItemsCache.Num(); i++)
	{
		UE_LOG(LogClass_SInGamePlayerInventoryWidget, Error, TEXT("Checking checking in inventory... (%d)"), i);
		delete m_DroppingItemsCache[i];
	}*/

	// Luciole 15/03/2024 || Probably should put this into its own function.
	m_DroppingItemsCache.Empty();

	return true;
}