#include "Interactables/ItemBagActor.h"
#include "Siltarn/Public/Interactables/PickupEntity.h"
#include "Siltarn/Public/Characters/SiltarnPlayerController.h"
#include "Siltarn/Public/Slate/Widgets/SInGameBagInventory.h"

DEFINE_LOG_CATEGORY(LogClass_AItemBagActor);
DEFINE_LOG_CATEGORY(LogClass_FBagTile);


int32 FBagTile::s_InstanceCount = 0;

FBagTile::FBagTile()
{
	s_bIsOccupied = false;

	s_InstanceCount++;
	UE_LOG(LogClass_FBagTile, Warning, TEXT("Number of FBagTile instances : %d"), s_InstanceCount);
}

FBagTile::FBagTile(bool p_bIsOccupied)
{
	s_bIsOccupied = p_bIsOccupied;

	s_InstanceCount++;
	UE_LOG(LogClass_FBagTile, Warning, TEXT("Number of FBagTile instances : %d"), s_InstanceCount);
}

FBagTile::~FBagTile()
{
	s_InstanceCount--;
	UE_LOG(LogClass_FBagTile, Warning, TEXT("Number of FBagTile instances : %d"), s_InstanceCount);
}




AItemBagActor::AItemBagActor()
{
	/*
		Luciole 12/02/2024 | This is a temporary fix in order to not have to deal with getting the AItemBagActor's Blueprint Class in order to spawn it 
		in ASiltarnCharacter::DROP_Items. 
	*/
	const ConstructorHelpers::FObjectFinder<UStaticMesh> _OF_Bag_StaticMesh(TEXT("StaticMesh'/Game/Geometry/ItemBag/SM_ItemBag.SM_ItemBag'"));

	check(_OF_Bag_StaticMesh.Succeeded());

	m_StaticMesh->SetStaticMesh(_OF_Bag_StaticMesh.Object);
}

AItemBagActor::~AItemBagActor()
{
	for (auto i : m_TilesMap)
	{
		UE_LOG(LogClass_AItemBagActor, Log, TEXT("Destroying FBagTile %s"), *i.Value->s_Coordinates.ToString());
		delete i.Value;
	}

	m_TilesMap.Empty();
	m_UnoccupiedTiles.Empty();
	m_FittingUnoccupiedTilesCache.Empty();
}

void AItemBagActor::BeginPlay()
{
	Super::BeginPlay();

	m_StaticMesh->AddImpulse(GetActorForwardVector() * IMPULSE_FORCE_ON_SPAWN);
}

void AItemBagActor::INTERFACE_Interact(ASiltarnPlayerController* p_SiltarnController)
{
	p_SiltarnController->OPEN_ItemBag(this, m_InventorySize, m_Items);
	DEBUG_ListAllItemEntities();
}



void AItemBagActor::RemoveItem(UPickupEntity* p_ItemActor)
{
	if (p_ItemActor)
	{
		m_Items.Remove(p_ItemActor);
	}
}



bool AItemBagActor::ComputeItemsPositionsInInventory()
{
	int32 _LargestItemInQueueIndex = FindLargestItemInQueue();
	UPickupEntity* _LargestItemInQueue = m_ItemsPlacementQueue[_LargestItemInQueueIndex];

	_LargestItemInQueue->SET_InventoryLocationTile(FIntPoint(0, 0)); // This is the first item
	m_InventorySize = _LargestItemInQueue->GET_InventorySpace();
	BuildTilesForItem(_LargestItemInQueue);

	m_Items.Emplace(_LargestItemInQueue);
	m_ItemsPlacementQueue[_LargestItemInQueueIndex] = nullptr;

	UE_LOG(LogClass_AItemBagActor, Warning, TEXT("Inventory size : %s"), *m_InventorySize.ToString());

	//while (m_ItemsPlacementQueue.Num() > 0)
	while (!AreWeDone()) // Luciole 24/03/2024 || Dirty fix for a dirty way to do things. Need to refactor it later
	{
		_LargestItemInQueueIndex = FindLargestItemInQueue();

		if (_LargestItemInQueueIndex < 0)
		{
			continue;
		}

		_LargestItemInQueue = m_ItemsPlacementQueue[_LargestItemInQueueIndex];
		ExpandInventory(_LargestItemInQueue, _LargestItemInQueueIndex);
	}


	m_ItemsPlacementQueue.Empty();

	UE_LOG(LogClass_AItemBagActor, Warning, TEXT("Inventory size : %s"), *m_InventorySize.ToString());

	DEBUG_ListAllItemEntitiesInQueue();
	DEBUG_ListAllItemEntities();
	DEBUG_ListAllTilesMap();
	DEBUG_ListAllUnoccupiedTilesMap();
	DEBUG_ListAllItemEntitiesCoordinates();

	return true;
}

bool AItemBagActor::AreWeDone()
{
	int32 _NullCount = 0;

	for (int32 i = 0; i < m_ItemsPlacementQueue.Num(); i++)
	{
		if (m_ItemsPlacementQueue[i] == nullptr)
		{
			_NullCount++;
		}
	}

	if (_NullCount == m_ItemsPlacementQueue.Num())
	{
		return true;
	}
	else
	{
		return false;
	}
}

void AItemBagActor::ExpandInventory(UPickupEntity* p_ItemEntity, int32 p_ItemIndexInQueue)
{
	/*
		If width > height --> expand height
		If width <= height --> expand width
	*/
	if (m_InventorySize.X > m_InventorySize.Y)
	{
		p_ItemEntity->SET_InventoryLocationTile(FIntPoint(0, m_InventorySize.Y));
		BuildTilesForItem(p_ItemEntity);
		m_Items.Emplace(p_ItemEntity);
		m_ItemsPlacementQueue[p_ItemIndexInQueue] = nullptr;
		m_InventorySize.Y += p_ItemEntity->GET_InventorySpace().Y;
		BuildLeftOverTiles(p_ItemEntity, EXPAND_ON_HEIGHT);

		UE_LOG(LogClass_AItemBagActor, Error, TEXT("EnlargeInventory() : The inventory size is now : %s"), *m_InventorySize.ToString());
	}
	else
	{
		p_ItemEntity->SET_InventoryLocationTile(FIntPoint(m_InventorySize.X, 0));
		BuildTilesForItem(p_ItemEntity);
		m_Items.Emplace(p_ItemEntity);
		m_ItemsPlacementQueue[p_ItemIndexInQueue] = nullptr;
		m_InventorySize.X += p_ItemEntity->GET_InventorySpace().X;
		BuildLeftOverTiles(p_ItemEntity, EXPAND_ON_WIDTH);

		UE_LOG(LogClass_AItemBagActor, Error, TEXT("EnlargeInventory() : The inventory size is now : %s"), *m_InventorySize.ToString());
	}

	UE_LOG(LogClass_AItemBagActor, Error, TEXT("There are still %d in queue !"), m_ItemsPlacementQueue.Num());

	// Filling unoccupied tiles
	for (int32 i = 0; i < m_ItemsPlacementQueue.Num(); i++)
	{
		UPickupEntity* _ItemEntity = m_ItemsPlacementQueue[i];

		if (!_ItemEntity)
		{
			continue;
		}

		FillUnoccupiedTiles(_ItemEntity, i);
	}
}



void AItemBagActor::BuildTilesForItem(UPickupEntity* p_ItemEntity)
{
	if (p_ItemEntity)
	{
		FIntPoint _ItemLocation = p_ItemEntity->GET_InventoryLocationTile() ;
		FIntPoint _ItemSize     = p_ItemEntity->GET_InventorySpace()        ;

		for (int32 y = _ItemLocation.Y; y < _ItemLocation.Y + _ItemSize.Y; y++)
		{
			for (int32 x = _ItemLocation.X; x < _ItemLocation.X + _ItemSize.X; x++)
			{
				FBagTile* _NewMapTile = new FBagTile(true);
				_NewMapTile->s_Coordinates = FIntPoint(x, y);
				m_TilesMap.Emplace(FIntPoint(x, y), _NewMapTile);
			}
		}
	}
}

void AItemBagActor::BuildLeftOverTiles(UPickupEntity* p_ItemEntity, EExpansionDir p_ExpansionDir)
{
	if (p_ItemEntity)
	{
		const FIntPoint _ItemLocation = p_ItemEntity->GET_InventoryLocationTile() ;
		const FIntPoint _ItemSize     = p_ItemEntity->GET_InventorySpace()        ;

		int32 _InitialX = 0;
		int32 _InitialY = 0;

		if (p_ExpansionDir == EXPAND_ON_WIDTH)
		{
			_InitialX = _ItemLocation.X               ;
			_InitialY = _ItemLocation.Y + _ItemSize.Y ;
		}
		else
		{
			_InitialX = _ItemLocation.X + _ItemSize.X ;
			_InitialY = _ItemLocation.Y               ;
		}


		for (int32 y = _InitialY; y < m_InventorySize.Y; y++)
		{
			for (int32 x = _InitialX; x < m_InventorySize.X; x++)
			{
				FBagTile* _NewMapTile = new FBagTile(false);
				_NewMapTile->s_Coordinates = FIntPoint(x, y);
				m_TilesMap.Emplace(FIntPoint(x, y), _NewMapTile);
				m_UnoccupiedTiles.Emplace(_NewMapTile);
			}
		}
	}
}

void AItemBagActor::FillUnoccupiedTiles(UPickupEntity* p_ItemEntity, int32 p_ItemIndexInQueue)
{
	for (int32 i = 0; i < m_UnoccupiedTiles.Num(); i++)
	{
		if (m_UnoccupiedTiles[i])
		{
			if (DoesItemFit(i, p_ItemEntity))
			{
				InsertItemIntoUnoccupiedTiles(p_ItemEntity, p_ItemIndexInQueue);
				return;
			}
		}
	}
}

bool AItemBagActor::DoesItemFit(int32 p_UnoccupiedTileIndex, UPickupEntity* p_ItemEntity)
{
	FBagTile* _TileMap = m_UnoccupiedTiles[p_UnoccupiedTileIndex];
	m_FittingUnoccupiedTilesCache.Emplace(_TileMap->s_Coordinates);

	if (_TileMap && p_ItemEntity)
	{
		const FIntPoint _ItemSize = p_ItemEntity->GET_InventorySpace();

		// Checking for "out of inventory" case
		if (_TileMap->s_Coordinates.X + _ItemSize.X > m_InventorySize.X ||
			_TileMap->s_Coordinates.Y + _ItemSize.Y > m_InventorySize.Y)
		{
			m_FittingUnoccupiedTilesCache.Empty();
			return false;
		}

		for (int32 y = _TileMap->s_Coordinates.Y; y < _TileMap->s_Coordinates.Y + _ItemSize.Y; y++)
		{
			for (int32 x = _TileMap->s_Coordinates.X; x < _TileMap->s_Coordinates.X + _ItemSize.X; x++)
			{
				FIntPoint _CurrentCheckingTile = FIntPoint(x, y);

				// There is no tile at the coordinates we are checking. 
				// Luciole 21/03/2024 || This appears redondant with the check above. Do we actually need this one ?
				if (!m_TilesMap.Contains(_CurrentCheckingTile))
				{
					m_FittingUnoccupiedTilesCache.Empty();
					return false;
				}

				// The tile we are checking is already occupied
				if (m_TilesMap.Contains(_CurrentCheckingTile))
				{
					FBagTile* _TileMap = *m_TilesMap.Find(_CurrentCheckingTile);

					if (_TileMap->s_bIsOccupied)
					{
						m_FittingUnoccupiedTilesCache.Empty();
						return false;
					}
				}

				m_FittingUnoccupiedTilesCache.Emplace(_CurrentCheckingTile);
			}
		}
	}
	else
	{
		return false;
	}

	return true;
}

void AItemBagActor::InsertItemIntoUnoccupiedTiles(UPickupEntity* p_ItemEntity, int32 p_ItemIndexInQueue)
{
	p_ItemEntity->SET_InventoryLocationTile(m_FittingUnoccupiedTilesCache[0]); // See the second line of DoesItemFit()

	for (int32 i = 0; i < m_FittingUnoccupiedTilesCache.Num(); i++)
	{
		FIntPoint _TileCoordinates = m_FittingUnoccupiedTilesCache[i];

		if (m_TilesMap.Contains(_TileCoordinates))
		{
			FBagTile* _MapTile = *m_TilesMap.Find(_TileCoordinates);

			if (_MapTile)
			{
				_MapTile->s_bIsOccupied = true;
			}
		}

		int32 _C = 0; // Counter

		while (_C < m_UnoccupiedTiles.Num())
		{
			FBagTile* _Tile = m_UnoccupiedTiles[_C];

			if (_Tile)
			{
				if (_TileCoordinates == _Tile->s_Coordinates)
				{
					m_UnoccupiedTiles.RemoveAt(_C);
				}
				else
				{
					_C++; // Almost
				}
			}
		}
	}

	m_FittingUnoccupiedTilesCache.Empty();
	m_Items.Emplace(p_ItemEntity);
	m_ItemsPlacementQueue[p_ItemIndexInQueue] = nullptr; // new
}



int32 AItemBagActor::FindLargestItemInQueue()
{
	int32 _LargestX       =  0 ;
	int32 _LargestY       =  0 ;
	int32 _ItemPosInArray = -1 ;

	for (int32 i = 0; i < m_ItemsPlacementQueue.Num(); i++)
	{
		UPickupEntity* _ItemEntity = m_ItemsPlacementQueue[i];
		
		/*
			Instead of using RemoveAt() and shrink m_ItemsPlacementQueue TArray every time, we set its elements to nullptr when we are done with it. 
			Consequently, we need to check for nullptr elements in the array in order to skip them to the next existing item.
		*/
		if (!_ItemEntity)
		{
			continue;
		}

		const int32 _TempX = _ItemEntity->GET_InventorySpace().X;
		const int32 _TempY = _ItemEntity->GET_InventorySpace().Y;


		// Compare X
		if (_TempX == _LargestX)
		{
			if (_TempY > _LargestY)
			{
				_LargestX       = _TempX ;
				_LargestY       = _TempY ;
				_ItemPosInArray = i      ;
				continue                 ;
			}
		}
		if (_TempX > _LargestX)
		{
			if (_TempY >= _LargestY)
			{
				_LargestX       = _TempX ;
				_LargestY       = _TempY ;
				_ItemPosInArray = i      ;
				continue                 ;
			}
		}

		// Compare Y
		if (_TempY == _LargestY)
		{
			if (_TempX > _LargestX)
			{
				_LargestY       = _TempY ;
				_LargestX       = _TempX ;
				_ItemPosInArray = i      ;
				continue                 ;
			}
		}
		if (_TempY > _LargestY)
		{
			if (_TempX >= _LargestX)
			{
				_LargestY       = _TempY ;
				_LargestX       = _TempX ;
				_ItemPosInArray = i      ;
				continue                 ;
			}
		}		
	}

	return _ItemPosInArray;
}



void AItemBagActor::DEBUG_ListAllItemEntities()
{
	UE_LOG(LogClass_AItemBagActor, Warning, TEXT("This bag contains the following %d items -----"), m_Items.Num());

	for (UPickupEntity* _Entity : m_Items)
	{
		if (_Entity)
		{
			UE_LOG(LogClass_AItemBagActor, Warning, TEXT("%s -> position : %s"), *_Entity->GET_Name(), *_Entity->GET_InventoryLocationTile().ToString());
		}
	}
}

void AItemBagActor::DEBUG_ListAllItemEntitiesInQueue()
{
	UE_LOG(LogClass_AItemBagActor, Warning, TEXT("This bag placement queue contains the following %d items -----"), m_ItemsPlacementQueue.Num());

	for (UPickupEntity* _Entity : m_ItemsPlacementQueue)
	{
		if (_Entity)
		{
			UE_LOG(LogClass_AItemBagActor, Warning, TEXT("%s"), *_Entity->GET_Name());
		}
	}
}


void AItemBagActor::DEBUG_ListAllTilesMap()
{
	UE_LOG(LogClass_AItemBagActor, Warning, TEXT("All Bag Tiles In TMap -----"));

	for (auto i : m_TilesMap)
	{
		if (i.Value->s_bIsOccupied)
		{
			UE_LOG(LogClass_AItemBagActor, Log, TEXT("Tile[%d;%d] (coordinates : %d;%d) --> OCCUPIED"), i.Key.X, i.Key.Y, i.Value->s_Coordinates.X, i.Value->s_Coordinates.Y);
		}
		else
		{
			UE_LOG(LogClass_AItemBagActor, Log, TEXT("Tile[%d;%d] (coordinates : %d;%d) --> UNOCCUPIED"), i.Key.X, i.Key.Y, i.Value->s_Coordinates.X, i.Value->s_Coordinates.Y);
		}
	}
}

void AItemBagActor::DEBUG_ListAllUnoccupiedTilesMap()
{
	UE_LOG(LogClass_AItemBagActor, Warning, TEXT("All Bag Unoccupied Tiles In TArray -----"));

	for (int32 i = 0; i < m_UnoccupiedTiles.Num(); i++)
	{
		if (m_UnoccupiedTiles[i])
		{
			UE_LOG(LogClass_AItemBagActor, Log, TEXT("Unoccupied tile coordinate : (%d ; %d)"), m_UnoccupiedTiles[i]->s_Coordinates.X, m_UnoccupiedTiles[i]->s_Coordinates.Y);
		}
	}
}

void AItemBagActor::DEBUG_ListAllItemEntitiesCoordinates()
{
	UE_LOG(LogClass_AItemBagActor, Warning, TEXT("All UPickupEntity items' coordinates -----"));

	for (int32 i = 0; i < m_Items.Num(); i++)
	{
		if (m_Items[i])
		{
			UE_LOG(LogClass_AItemBagActor, Log, TEXT("%s has inventory coordinates : %s"), *m_Items[i]->GET_Name(), *m_Items[i]->GET_InventoryLocationTile().ToString());
		}
	}
}

void AItemBagActor::SET_BagWidgetReference(TSharedPtr<SInGameBagInventory> p_BagWidget)
{
	if (!p_BagWidget)
	{
		UE_LOG(LogClass_AItemBagActor, Error, TEXT("SetBagWidgetReference() : p_BagWidget is NULL !"));
		return;
	}

	m_BagInventory = p_BagWidget;
}

bool AItemBagActor::LoadBag(UPickupEntity* p_ItemEntity)
{
	if (p_ItemEntity)
	{
		m_Items.Emplace(p_ItemEntity);

		DEBUG_ListAllItemEntities();
		DEBUG_ListAllItemEntitiesCoordinates();
		DEBUG_ListAllItemEntitiesInQueue();
		

		return ComputeBagInventoryWidgetData(p_ItemEntity);
	}

	return false;
}

bool AItemBagActor::LoadBag(TArray<UPickupEntity*>& p_ItemEntities)
{
	m_ItemsPlacementQueue.Append(p_ItemEntities);

	if (p_ItemEntities.Num() > 0)
	{
		return ComputeItemsPositionsInInventory();
	}

	UE_LOG(LogClass_AItemBagActor, Error, TEXT("LoadBag() : p_ItemEntities seems to be empty !"));
	return false;
}



bool AItemBagActor::ComputeBagInventoryWidgetData(UPickupEntity* p_ItemEntity)
{
	if (p_ItemEntity)
	{
		p_ItemEntity->SET_InventoryLocationTile(FIntPoint(0, 0));
		m_InventorySize = p_ItemEntity->GET_InventorySpace();
		return true;
	}

	return false;
}

