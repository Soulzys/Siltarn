#include "Slate/Widgets/SInventoryWidget.h"
#include "Siltarn/Public/Slate/Widgets/SInventoryItemWidget.h"
#include "Siltarn/Public/Slate/Widgets/SDebugWidget.h"
#include "Siltarn/Public/HUDs/GameplayHUD.h"
#include "Siltarn/Public/Interactables/PickupEntity.h"
#include "Siltarn/Public/Slate/Styles/SiltarnGeneralStyleContainer.h"
#include "Siltarn/Public/Slate/Styles/SiltarnStyleController.h"

DEFINE_LOG_CATEGORY(LogClass_SInventoryWidget);
DEFINE_LOG_CATEGORY(LogStruct_FLine);
DEFINE_LOG_CATEGORY(LogStruct_FTile);
DEFINE_LOG_CATEGORY(LogStruct_FInventoryItem)
DEFINE_LOG_CATEGORY(LogStruct_FCrossAnchor)


/*****************************************************************************************
	FLine struct
******************************************************************************************/

int32 FLine::s_InstanceCount = 0;

FLine::FLine()
{
	s_InstanceCount++;
	UE_LOG(LogStruct_FLine, Log, TEXT("A new instance was created ! || Instance count : %d"), s_InstanceCount);
}

FLine::FLine(FVector2D p_StartPoint, FVector2D p_EndPoint)
{
	s_PointsArray.Add(p_StartPoint);
	s_PointsArray.Add(p_EndPoint);

	s_InstanceCount++;
	UE_LOG(LogStruct_FLine, Log, TEXT("A new instance was created ! || Instance count : %d"), s_InstanceCount);
}

FLine::~FLine()
{
	s_InstanceCount--;
	UE_LOG(LogStruct_FLine, Log, TEXT("An instance was destroyed ! || Instance count : %d"), s_InstanceCount);
}

TArray<FVector2D>& FLine::GET_PointsArray()
{
	return s_PointsArray;
}

const FVector2D& FLine::GET_StartPoint() const
{
	return s_PointsArray[0];
}



/*****************************************************************************************
	FTile struct
******************************************************************************************/

int32 FTile::s_InstanceCount = 0;

FTile::FTile()
{
	s_InstanceCount++;
	UE_LOG(LogStruct_FTile, Log, TEXT("A new instance was created ! || Instance count : %d"), s_InstanceCount);
}

FTile::FTile(FIntPoint p_Coordinates, int32 p_Id)
{
	s_TileCoordinates = p_Coordinates;
	s_TileIndex = p_Id;

	s_InstanceCount++;
	UE_LOG(LogStruct_FTile, Log, TEXT("A new instance was created ! || Instance count : %d"), s_InstanceCount);
}

FTile::~FTile()
{
	s_InstanceCount--;
	UE_LOG(LogStruct_FTile, Log, TEXT("An instance was destroyed ! || Instance count : %d"), s_InstanceCount);
}

uint32 FTile::GET_TileIndex() const
{
	return s_TileIndex;
}

FVector2D FTile::GET_RelativeCoordinates(const int32 p_TileSize) const
{
	return FVector2D((float)s_TileCoordinates.X * p_TileSize, (float)s_TileCoordinates.Y * p_TileSize);
}

bool FTile::IS_Occupied()
{
	if (s_Owner)
	{
		return true;
	}
	else
	{
		return false;
	}
}

FInventoryItem* FTile::GET_Owner() const
{
	return s_Owner;
}

void FTile::SET_Owner(FInventoryItem* p_Item)
{
	s_Owner = p_Item;
}

void FTile::DEBUG_DisplayStructDataThroughLogs()
{
	UE_LOG(LogStruct_FTile, Warning, TEXT("DEBUG_DisplayStructDataThroughLogs() : ***"));
	UE_LOG(LogStruct_FTile, Log, TEXT("DEBUG_DisplayStructDataThroughLogs() : s_TileCoordinates --> %s"), *s_TileCoordinates.ToString());
	UE_LOG(LogStruct_FTile, Warning, TEXT("DEBUG_DisplayStructDataThroughLogs() : ***"));
}



/*****************************************************************************************
	FCrossAnchor struct
******************************************************************************************/

int32 FCrossAnchor::s_InstanceCount = 0;

FCrossAnchor::FCrossAnchor()
{
	s_CrossAnchorIndex = 0;
	s_Position = FVector2D(0.0f);
	s_Coordinates = FIntPoint(0);

	s_InstanceCount++;
	UE_LOG(LogStruct_FCrossAnchor, Log, TEXT("A new instance was created ! || Instance count : %d"), s_InstanceCount);
}

FCrossAnchor::FCrossAnchor(const FIntPoint& p_Coordinates, int32 p_Id)
{
	s_Coordinates = p_Coordinates;
	s_CrossAnchorIndex = p_Id;

	s_InstanceCount++;
	UE_LOG(LogStruct_FCrossAnchor, Log, TEXT("A new instance was created ! || Instance count : %d"), s_InstanceCount);
}

FCrossAnchor::~FCrossAnchor()
{
	s_InstanceCount--;
	UE_LOG(LogStruct_FCrossAnchor, Log, TEXT("An instance was destroyed ! || Instance count : %d"), s_InstanceCount);
}

const FIntPoint FCrossAnchor::GET_TopLeftTileCoordinates(UPickupEntity* p_ItemEntity) const
{
	int32 _X = s_Coordinates.X - (p_ItemEntity->GET_InventorySpace().X / 2);
	int32 _Y = s_Coordinates.Y - (p_ItemEntity->GET_InventorySpace().Y / 2);

	return FIntPoint(_X, _Y);
}

FIntPoint FCrossAnchor::GET_Coordinates() const
{
	return s_Coordinates;
}

int32 FCrossAnchor::GET_Index() const
{
	return s_CrossAnchorIndex;
}


/*****************************************************************************************
	FInventoryItem struct
******************************************************************************************/

int32 FInventoryItem::s_InstanceCount = 0;

FInventoryItem::FInventoryItem()
{
	s_InstanceCount++;
	UE_LOG(LogStruct_FInventoryItem, Log, TEXT("A new instance was created ! || Instance count : %d"), s_InstanceCount);
}

FInventoryItem::~FInventoryItem()
{
	s_OccupyingTiles.Empty();

	s_InstanceCount--;
	UE_LOG(LogStruct_FInventoryItem, Log, TEXT("An instance was destroyed ! || Instance count : %d"), s_InstanceCount);
}

void FInventoryItem::FREE_Tiles()
{
	for (FTile* _Tile : s_OccupyingTiles)
	{
		_Tile->SET_Owner(nullptr);
		UE_LOG(LogStruct_FInventoryItem, Log, TEXT("FTile.s_StructId = %d has been freed !"), _Tile->GET_TileIndex());
	}

	if (s_OccupyingTiles.IsEmpty())
	{
		UE_LOG(LogStruct_FInventoryItem, Warning, TEXT("It shouldn't be empty."));
	}
}

FTile* FInventoryItem::GET_ControlTile() const
{
	if (s_OccupyingTiles.IsEmpty())
	{
		UE_LOG(LogStruct_FInventoryItem, Warning, TEXT("GET_ControlTile() : The FTile TArray is empty. || Returning a nullptr."));
		return nullptr;
	}

	return s_OccupyingTiles[0];
}


void FInventoryItem::UPDATE_WidgetTile()
{
	if (s_ItemWidget == nullptr)
	{
		UE_LOG(LogStruct_FInventoryItem, Warning, TEXT("UPDATE_WidgetTile() : p_ItemWidget is NULL. || Unable to proceed further."));
		return;
	}

	s_ItemWidget->UPDATE_Tile(*GET_ControlTile());
}

void FInventoryItem::SET_ParentSlot(SCanvas::FSlot* p_Slot)
{
	checkf(p_Slot != nullptr, TEXT("FInventoryItem::SET_ParentSlot() : p_Slot is NULL !"));

	s_ParentSlot = p_Slot;
}

void FInventoryItem::MOVE_Item(const FVector2D& p_NewLocation) const
{
	checkf(s_ParentSlot != nullptr, TEXT("FInventoryItem::MOVE_Item() : s_ParentSlot is NULL !"));

	s_ParentSlot->SetPosition(p_NewLocation);
}



/*****************************************************************************************
	SInventoryWidget class
******************************************************************************************/

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

int32 SInventoryWidget::m_InstanceCount = 0;

SInventoryWidget::SInventoryWidget()
{
	m_InstanceCount++;
	UE_LOG(LogClass_SInventoryWidget, Log, TEXT("A new instance was created ! || Instance count : %d"), m_InstanceCount);
}

SInventoryWidget::~SInventoryWidget()
{
	CLEAN_Everything();

	m_InstanceCount--;
	UE_LOG(LogClass_SInventoryWidget, Log, TEXT("An instance was destroyed ! || Instance count : %d"), m_InstanceCount);
}

void SInventoryWidget::Construct(const FArguments& p_InArgs)
{
	const FSiltarnGeneralStyleContainerStruct& _GeneralStyle = FSiltarnStyleController::GET_SiltarnGeneralStyleContainerStruct();

	m_NumberOfColumns = p_InArgs._a_NumberOfColumns.Get();
	m_NumberOfRows    = p_InArgs._a_NumberOfRows   .Get();
	m_TileSize        = p_InArgs._a_TileSize       .Get();
	m_HUDOwner        = p_InArgs._a_HUDOwner             ;

	//

	COMPUTE_HorizontalLines();
	COMPUTE_VerticalLines();

	m_InventoryBackgroundColor.R = 0.0f  ;
	m_InventoryBackgroundColor.G = 0.0f  ;
	m_InventoryBackgroundColor.B = 0.0f  ;
	m_InventoryBackgroundColor.A = 0.25f ;

	ChildSlot
	.HAlign(HAlign_Fill)
	.VAlign(VAlign_Fill)
	[
		SNew(SBorder)
		.BorderImage(&_GeneralStyle.m_WhiteBackgroundImage_SlateBrush)
		.BorderBackgroundColor(m_InventoryBackgroundColor)
		.Padding(0.0f)
		[
			SAssignNew(m_Canvas, SCanvas)
		]
	];

	BUILD_Tiles();
	BUILD_CrossAnchors();	
}

int32 SInventoryWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	// Painting lines
	//
	for (int32 i = 0; i < m_HorizontalLines.Num(); i++)
	{
		FSlateDrawElement::MakeLines(OutDrawElements, LayerId, AllottedGeometry.ToPaintGeometry(), m_HorizontalLines[i].s_PointsArray, ESlateDrawEffect::None, FLinearColor::Red);
	}

	for (int32 i = 0; i < m_VerticalLines.Num(); i++)
	{
		FSlateDrawElement::MakeLines(OutDrawElements, LayerId, AllottedGeometry.ToPaintGeometry(), m_VerticalLines[i].s_PointsArray, ESlateDrawEffect::None, FLinearColor::Red);
	}

	// Painting the widgets built in Construct()
	//
	return SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
}

void SInventoryWidget::COMPUTE_HorizontalLines()
{
	const float _Width = m_NumberOfColumns * m_TileSize;

	for (int32 i = 0; i <= m_NumberOfRows; i++)
	{
		FVector2D _StartPoint;
		_StartPoint.X = 0.0f;
		_StartPoint.Y = i * m_TileSize;

		FVector2D _EndPoint;
		_EndPoint.X = _Width;
		_EndPoint.Y = i * m_TileSize;

		m_HorizontalLines.Add(FLine(_StartPoint, _EndPoint));
	}
}

void SInventoryWidget::COMPUTE_VerticalLines()
{
	const float _Height = m_NumberOfRows * m_TileSize;

	for (int32 i = 0; i <= m_NumberOfColumns; i++)
	{
		FVector2D _StartPoint;
		_StartPoint.X = i * m_TileSize;
		_StartPoint.Y = 0.0f;

		FVector2D _EndPoint;
		_EndPoint.X = i * m_TileSize;
		_EndPoint.Y = _Height;

		m_VerticalLines.Add(FLine(_StartPoint, _EndPoint));
	}
}

void SInventoryWidget::BUILD_Tiles()
{
	int32 _IterationCount = 0;

	for (int32 i = 0; i < m_NumberOfRows; i++)
	{
		for (int32 j = 0; j < m_NumberOfColumns; j++)
		{
			FTile _NewTile = FTile(FIntPoint(j, i), _IterationCount);
			m_Tiles.Add(_NewTile);

			_IterationCount++;
		}
	}

	UE_LOG(LogClass_SInventoryWidget, Log, TEXT("BUILD_Tiles() : Number of tiles built : %d"), _IterationCount);
}

void SInventoryWidget::BUILD_CrossAnchors()
{
	int32 _IterationCount = 0;

	for (int32 i = 1; i < m_NumberOfRows; i++)
	{
		for (int32 j = 1; j < m_NumberOfColumns; j++)
		{
			FCrossAnchor _NewCross = FCrossAnchor(FIntPoint(j, i), _IterationCount);
			m_CrossAnchors.Add(_NewCross);

			_IterationCount++;
		}
	}

	UE_LOG(LogClass_SInventoryWidget, Log, TEXT("BUILD_CrossAnchors() : Number of FCrossAnchors built : %d"), _IterationCount);
}

void SInventoryWidget::CONSTRUCT_CanvasItemSlot(UPickupEntity* p_Item, FTile* p_ControlTilePtr, FInventoryItem* p_InventoryItem)
{
	// The widget location (0, 0) is its top left corner
	const float _SlotPosX = p_ControlTilePtr->s_TileCoordinates.X * m_TileSize;
	const float _SlotPosY = p_ControlTilePtr->s_TileCoordinates.Y * m_TileSize;

	const float _SlotSizeX = p_Item->GET_InventorySpace().X * m_TileSize;
	const float _SlotSizeY = p_Item->GET_InventorySpace().Y * m_TileSize;

	TSharedPtr<SInventoryItemWidget> _ItemWidget = nullptr             ;
	SCanvas::FSlot*                  _Slot       = nullptr             ;
	FVector2D                        _ItemSize(_SlotSizeX, _SlotSizeY) ;


	m_Canvas->AddSlot()
	.Position(FVector2D(_SlotPosX, _SlotPosY))
	.Size    (_ItemSize)
	[
		SAssignNew(_ItemWidget, SInventoryItemWidget)
		.a_Tile    (p_ControlTilePtr)
		.a_ItemData(p_Item          )
		.a_ItemSize(_ItemSize       )
	]
	.Expose(_Slot);

	p_InventoryItem->s_ItemWidget = _ItemWidget;
	p_InventoryItem->SET_ParentSlot(_Slot);
}

bool SInventoryWidget::ADD_Item(UPickupEntity* p_Item)
{
	FTile* _TilePtr = &m_Tiles[m_CachedTileIndex];

	if (_TilePtr->IS_Occupied())
	{
		// Should never happen but we may eventually need to double safe check it and make it as if it may happen (so we'd need to destroy p_item, etc)
		return false;
	}
	

	FInventoryItem* _NewInventoryItem = new FInventoryItem();

	// Set all FTiles to occupied     ||    we should eventually put this into its own function for cleaningness purposes
	for (int32 j = 0; j < m_CachedTilesIndexes.Num(); j++)
	{
		FTile* _Tile = &m_Tiles[m_CachedTilesIndexes[j]];

		if (_Tile)
		{
			_Tile->SET_Owner(_NewInventoryItem);
			_NewInventoryItem->s_OccupyingTiles.Add(_Tile);
		}
		else
		{
			UE_LOG(LogClass_SInventoryWidget, Warning, TEXT("Could not retrieve the reference out of &m_Tiles[m_CachedTilesIndexes[i]]"));
		}
	}


	CONSTRUCT_CanvasItemSlot(p_Item, _TilePtr, _NewInventoryItem);
	m_InventoryItemsMap.Add(*_TilePtr, _NewInventoryItem);

	p_Item->SET_ControlTileId(m_CachedTileIndex);

	// Clean the cached resources
	m_CachedTileIndex = -1;
	m_CachedTilesIndexes.Empty();

	return true;
}

void SInventoryWidget::REMOVE_Item(FTile* p_ItemControlTile)
{
	FInventoryItem* _InventoryItemStruct = nullptr;
	m_InventoryItemsMap.RemoveAndCopyValue(*p_ItemControlTile, _InventoryItemStruct);
	check(_InventoryItemStruct != nullptr);

	m_Canvas->RemoveSlot(_InventoryItemStruct->s_ItemWidget.ToSharedRef());
	_InventoryItemStruct->FREE_Tiles();
	delete _InventoryItemStruct;
}

bool SInventoryWidget::TRY_AddingItemToInventory(const FIntPoint& p_ItemSize)
{
	// Eventually try to switch with ensureAlways()
	if (p_ItemSize.X <= 0 || p_ItemSize.Y <= 0)
	{
		UE_LOG(LogClass_SInventoryWidget, Warning, TEXT("IS_RoomAvailableInInventory() : Item size is uncorrect. Could not proceed further."));
		return false;
	}


	for (int32 i = 0; i < m_Tiles.Num(); i++)
	{
		if (IS_RoomAvailableAtTileIndex(p_ItemSize, i))
		{
			m_CachedTileIndex = i;
			return true;
		}
	}

	return false;
}

bool SInventoryWidget::IS_RoomAvailableAtTileIndex(const FIntPoint& p_ItemSize, int32 p_TileIndex)
{
	FIntPoint _TopLeftTile = IndexToTileCoordinates(p_TileIndex);

	// Checking for "item out of grid" case
	if (_TopLeftTile.Y + p_ItemSize.Y > m_NumberOfRows || _TopLeftTile.X + p_ItemSize.X > m_NumberOfColumns)
	{
		UE_LOG(LogClass_SInventoryWidget, Warning, TEXT("IS_RoomAvailableAtTileIndex() : FTile %d -> Out of grid case."), p_TileIndex);
		return false;
	}

	for (int32 y = _TopLeftTile.Y; y < p_ItemSize.Y + _TopLeftTile.Y; y++)
	{
		for (int32 x = _TopLeftTile.X; x < p_ItemSize.X + _TopLeftTile.X; x++)
		{
			FIntPoint _TileCoordinates = FIntPoint(x, y)                              ;
			int32     _TileIndex       = TileCoordinatesToTileIndex(_TileCoordinates) ;

			// Checking for "out of bounds" case (happens when checking for the last FTile)
			if (_TileIndex >= m_Tiles.Num())
			{
				UE_LOG(LogClass_SInventoryWidget, Warning, TEXT("IS_RoomAvailableAtTileIndex() : FTile %d -> Out of bound case."), p_TileIndex);
				return false;
			}

			// Checking for "already occupied" case
			if (m_Tiles[_TileIndex].IS_Occupied())
			{
				UE_LOG(LogClass_SInventoryWidget, Warning, TEXT("IS_RoomAvailableAtTileIndex() : FTile %d -> Already occupied case."), p_TileIndex);
				return false;
			}

			// It is supposed to be cleaned in ADD_item()
			m_CachedTilesIndexes.Add(_TileIndex);
		}
	}

	UE_LOG(LogClass_SInventoryWidget, Log, TEXT("We have found a room at FTile %d !"), p_TileIndex);
	return true;
}

bool SInventoryWidget::IS_TileAvailable(FInventoryItem* p_Item, UPickupEntity* p_ItemEntity, FTile* p_TargetTile)
{
	FIntPoint _TileCoordinates = p_TargetTile->s_TileCoordinates    ;
	FIntPoint _ItemDimensions  = p_ItemEntity->GET_InventorySpace() ;

	if (_TileCoordinates.X + _ItemDimensions.X > m_NumberOfColumns || _TileCoordinates.Y + _ItemDimensions.Y > m_NumberOfRows)
	{
		UE_LOG(LogClass_SInventoryWidget, Warning, TEXT("IS_TileAvailable_a() : Out of grid case."));
		return false;
	}

	// If target FTile is occupied
	if (p_TargetTile->IS_Occupied())
	{
		// If the FTile is occupied by our dragged item, we can still drop it
		if (*p_TargetTile->GET_Owner() != *p_Item)
		{
			UE_LOG(LogClass_SInventoryWidget, Warning, TEXT("IS_TileAvailable() : p_TargetTile occupied case."));
			return false;
		}
	}

	TArray<int32> _TilesIndex;

	for (int32 y = _TileCoordinates.Y; y < _TileCoordinates.Y + _ItemDimensions.Y; y++)
	{
		for (int32 x = _TileCoordinates.X; x < _TileCoordinates.X + _ItemDimensions.X; x++)
		{
			FIntPoint _TileCoordinates = FIntPoint(x, y);
			int32 _TileIndex = TileCoordinatesToTileIndex(_TileCoordinates);

			if (_TileIndex >= m_Tiles.Num())
			{
				UE_LOG(LogClass_SInventoryWidget, Warning, TEXT("IS_TileAvailable() : Last FTile case. (Checking for FTile %d"), _TileIndex);
				return false;
			}

			if (m_Tiles[_TileIndex].IS_Occupied())
			{
				UE_LOG(LogTemp, Warning, TEXT("m_Tiles[_TileIndex].GET_Owner()->GET_ControlTile() : %d"), m_Tiles[_TileIndex].GET_Owner()->GET_ControlTile()->GET_TileIndex());
				UE_LOG(LogTemp, Warning, TEXT("p_Item->GET_ControlTile() : %d"), p_Item->GET_ControlTile()->GET_TileIndex());

				if (*m_Tiles[_TileIndex].GET_Owner() != *p_Item)
				{					
					UE_LOG(LogClass_SInventoryWidget, Warning, TEXT("IS_TileAvailable() : FTile occupied case. (Checking for FTile %d"), _TileIndex);
					return false;
				}				
			}

			_TilesIndex.Add(_TileIndex);
		}
	}

	p_Item->FREE_Tiles();
	p_Item->s_OccupyingTiles.Empty();

	if (p_Item == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Wait.... what ????"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Nevermind"));
	}

	// If we reach this point, it means the target FTile (p_TargetTile) is validated
	for (int32 i = 0; i < _TilesIndex.Num(); i++)
	{
		FTile* _Tile = &m_Tiles[_TilesIndex[i]];
		_Tile->SET_Owner(p_Item);
		p_Item->s_OccupyingTiles.Add(_Tile);

		UE_LOG(LogTemp, Log, TEXT("FTile %d has been occupied by the item !"), _Tile->GET_TileIndex());
	}

	return true;
}

FIntPoint SInventoryWidget::IndexToTileCoordinates(int32 p_Index)
{
	FIntPoint _TileCoordinates;
	_TileCoordinates.X = p_Index % m_NumberOfColumns;
	_TileCoordinates.Y = p_Index / m_NumberOfColumns;

	return _TileCoordinates;
}

int32 SInventoryWidget::TileCoordinatesToTileIndex(const FIntPoint& p_Coordinates)
{
	int32 _Index = p_Coordinates.X + (p_Coordinates.Y * m_NumberOfColumns);

	return _Index;
}

int32 SInventoryWidget::CrossAnchorCoordinatesToCrossAnchorIndex(const FIntPoint& p_Coordinates)
{
	// (x - 1) + ((y - 1) * nb of cross per row)
	int32 _Index = p_Coordinates.X - 1 + (p_Coordinates.Y - 1) * (m_NumberOfColumns - 1);

	return _Index;
}

FVector2D SInventoryWidget::TileToRelativeLocation(FTile* p_Tile)
{
	if (p_Tile == nullptr)
	{
		UE_LOG(LogClass_SInventoryWidget, Warning, TEXT("TileToRelativeLocation() : p_Tile is NULL. || Unable to proceed further."));
		return FVector2D(-100.0f); // Returning a position out of the SInventory widget
	}

	return FVector2D((float)p_Tile->s_TileCoordinates.X * m_TileSize, (float)p_Tile->s_TileCoordinates.Y * m_TileSize);
}

FTile* SInventoryWidget::RelativeLocationToTile(const FVector2D& p_Loc, const FIntPoint& p_ItemSize)
{
	int32 _X = FMath::FloorToInt32(p_Loc.X);
	int32 _Y = FMath::FloorToInt32(p_Loc.Y);

	UE_LOG(LogTemp, Warning, TEXT("RelativeLocationToTile() : _X : %d"), _X);
	UE_LOG(LogTemp, Warning, TEXT("RelativeLocationToTile() : _Y : %d"), _Y);

	_X /= m_TileSize;
	_Y /= m_TileSize;	

	// if item isn't of size (1,1), we need an offset so the item is always positioned with its center at the FTile's mouse location
	if (p_ItemSize.X != 1)
	{
		const int32 _OffsetX = (p_ItemSize.X - 1) / 2;
		const int32 _OffsetY = (p_ItemSize.Y - 1) / 2;

		_X -= _OffsetX;
		_Y -= _OffsetY;

		if (_X < 0 || _Y < 0)
		{
			return nullptr;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Aiming for FTile(%d, %d)"), _X, _Y);

	const int32 _Index = TileCoordinatesToTileIndex(FIntPoint(_X, _Y));

	FTile* _TilePtr = &m_Tiles[_Index];

	return _TilePtr;
}

FCrossAnchor* SInventoryWidget::RelativeLocationToCrossAnchor(const FVector2D& p_Loc)
{
	int32 _X = FMath::FloorToInt32(p_Loc.X);
	int32 _Y = FMath::FloorToInt32(p_Loc.Y);

	UE_LOG(LogTemp, Warning, TEXT("Trying to match location : %s"), *p_Loc.ToString());
	UE_LOG(LogTemp, Warning, TEXT("After floored to int32 : _X -> %d"), _X);
	UE_LOG(LogTemp, Warning, TEXT("After floored to int32 : _Y -> %d"), _Y);

	int32 _TargetX = 0;
	int32 _TargetY = 0;

	int32 _NumberOfCrossAnchorsPerRow = m_NumberOfColumns - 1;
	int32 _NumberOfCrossAnchorsPerColumn = m_NumberOfRows - 1;

	// Checking far west
	if (_X <= m_TileSize)
	{
		UE_LOG(LogTemp, Warning, TEXT("Far West"));
		_TargetX = 1; // The western FCrossAnchor coordinate is X = 1
	}

	// Checking far east
	if (_X >= _NumberOfCrossAnchorsPerRow * m_TileSize)
	{
		UE_LOG(LogTemp, Warning, TEXT("Far East"));
		_TargetX = _NumberOfCrossAnchorsPerRow;
	}

	// Checking far north
	if (_Y <= m_TileSize)
	{
		UE_LOG(LogTemp, Warning, TEXT("Far North"));
		_TargetY = 1;
	}

	// Checking far south
	if (_Y >= _NumberOfCrossAnchorsPerColumn * m_TileSize)
	{
		UE_LOG(LogTemp, Warning, TEXT("Far South"));
		_TargetY = _NumberOfCrossAnchorsPerColumn;
	}


	/*
		Checking for X
	*/
	int32 _CrossAnchor = 1;

	// As we are already checking the far east, we do not need to check for the last FCrossAnchor, therefore no need for <=
	while (_CrossAnchor < _NumberOfCrossAnchorsPerRow)
	{
		int32 _CrossAnchorPos = _CrossAnchor * m_TileSize;

		if (_X < _CrossAnchorPos + m_TileSize / 2)
		{
			_TargetX = _CrossAnchor;
			UE_LOG(LogTemp, Warning, TEXT("%d < %d (_CrossAnchorPos + m_TileSize / 2)"), _X, _CrossAnchorPos + m_TileSize / 2);
			break;
		}

		if (_X < _CrossAnchorPos + m_TileSize)
		{
			_TargetX = _CrossAnchor + 1;
			UE_LOG(LogTemp, Warning, TEXT("%d < %d (_CrossAnchorPos + m_TileSize)"), _X, _CrossAnchorPos + m_TileSize);
			break;
		}

		_CrossAnchor++;
	}

	/*
		Checking for Y
	*/
	_CrossAnchor = 1;

	while (_CrossAnchor < _NumberOfCrossAnchorsPerColumn)
	{
		int32 _CrossAnchorPos = _CrossAnchor * m_TileSize;

		if (_Y < _CrossAnchorPos + m_TileSize / 2)
		{
			_TargetY = _CrossAnchor;
			UE_LOG(LogTemp, Warning, TEXT("%d < %d (_CrossAnchorPos + m_TileSize / 2)"), _Y, _CrossAnchorPos + m_TileSize / 2);
			break;
		}

		if (_Y < _CrossAnchorPos + m_TileSize)
		{
			_TargetY = _CrossAnchor + 1;
			UE_LOG(LogTemp, Warning, TEXT("%d < %d (_CrossAnchorPos + m_TileSize)"), _Y, _CrossAnchorPos + m_TileSize);
			break;
		}

		_CrossAnchor++;
	}

	UE_LOG(LogTemp, Warning, TEXT("_Target Pos : %d ; %d"), _TargetX, _TargetY);

	const int32 _Index = CrossAnchorCoordinatesToCrossAnchorIndex(FIntPoint(_TargetX, _TargetY));

	FCrossAnchor* _CrossAnchorPtr = &m_CrossAnchors[_Index];

	return _CrossAnchorPtr;
}

FReply SInventoryWidget::OnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent)
{
	TSharedPtr<FInventoryItemDragDrop> _Operation = DragDropEvent.GetOperationAs<FInventoryItemDragDrop>();

	if (_Operation.IsValid() == false)
	{
		UE_LOG(LogClass_SInventoryWidget, Warning, TEXT("OnDrop() : _Operation is not valid. || Unable to proceed further."));
		return FReply::Unhandled();
	}


	FVector2D _MouseRelativePos = MyGeometry.AbsoluteToLocal(DragDropEvent.GetScreenSpacePosition()) ;
	FVector2D _TargetPos        = _MouseRelativePos - _Operation->GET_WidgetMiddlePosition()         ;

	FTile* _TargetTile = nullptr;





	UPickupEntity* _ItemEntity = _Operation->GET_ItemEntity();

	if (_ItemEntity == nullptr)
	{
		UE_LOG(LogClass_SInventoryWidget, Warning, TEXT("OnDrop() : _ItemEntity is NULL. || Cannot proceed further."));
	}




	EItemAnchorType _AnchorType = _ItemEntity->GET_AnchorType();

	if (_AnchorType == EItemAnchorType::Cross)
	{
		FCrossAnchor* _Cross = RelativeLocationToCrossAnchor(CENTER_Location(_ItemEntity, _TargetPos));
		int32 _TileIndex = TileCoordinatesToTileIndex(_Cross->GET_TopLeftTileCoordinates(_ItemEntity));
		_TargetTile = &m_Tiles[_TileIndex];
	}
	else if (_AnchorType == EItemAnchorType::Tile)
	{
		_TargetTile = RelativeLocationToTile(CENTER_Location(_ItemEntity, _TargetPos), _ItemEntity->GET_InventorySpace());
	}
	else if (_AnchorType == EItemAnchorType::Line)
	{

	}
	else if (_AnchorType == EItemAnchorType::Error)
	{
		UE_LOG(LogClass_SInventoryWidget, Warning, TEXT("OnDrop() : EItemAnchorType::Error."));
	}
	else
	{
		UE_LOG(LogClass_SInventoryWidget, Warning, TEXT("OnDrop() : Error. Not even EItemAnchorType::Error was catched. Very weird."));
	}





	FTile* _OriginalTilee = _Operation->GET_Tile();

	check(_OriginalTilee != nullptr);

	FInventoryItem* _Item = *m_InventoryItemsMap.Find(*_OriginalTilee);

	check(_Item != nullptr);

	TRY_DroppingItemToTile(_Item, _ItemEntity, _OriginalTilee, _TargetTile);

	check(_Item->GET_ControlTile() != nullptr);

	// To use whenever we move the item to another FTile
	m_InventoryItemsMap.Remove(*_OriginalTilee);
	m_InventoryItemsMap.Add(*_Item->GET_ControlTile(), _Item);

	return FReply::Handled();	
}

bool SInventoryWidget::TRY_DroppingItemToTile(FInventoryItem* p_Item, UPickupEntity* p_ItemEntity, FTile* p_OriginalTile, FTile* p_TargetTile)
{
	bool _bIsTileAvailable = IS_TileAvailable(p_Item, p_ItemEntity, p_TargetTile);

	if (_bIsTileAvailable)
	{
		p_Item->UPDATE_WidgetTile();
		p_ItemEntity->SET_ControlTileId(p_TargetTile->GET_TileIndex());

		MOVE_Item(p_Item, p_TargetTile->GET_RelativeCoordinates(m_TileSize));
	}
	else
	{
		MOVE_Item(p_Item, p_OriginalTile->GET_RelativeCoordinates(m_TileSize));
	}

	return _bIsTileAvailable;
}

void SInventoryWidget::MOVE_Item(FInventoryItem* p_Item, const FVector2D& p_Location)
{
	checkf(p_Item != nullptr, TEXT("SInventoryWidget::MOVE_Item() : p_Item is NULL !"));

	p_Item->MOVE_Item(p_Location);
}

FVector2D SInventoryWidget::CENTER_Location(UPickupEntity* p_ItemEntity, const FVector2D& p_Location)
{
	const float _fX = (float)p_ItemEntity->GET_InventorySpace().X * m_TileSize / 2.0f;
	const float _fY = (float)p_ItemEntity->GET_InventorySpace().Y * m_TileSize / 2.0f;

	return FVector2D(p_Location + FVector2D(_fX, _fY));
}

void SInventoryWidget::DEBUG_DisplayTilesStatusThroughUELogs()
{
	UE_LOG(LogTemp, Warning, TEXT("***"));

	for (FTile _Tile : m_Tiles)
	{
		if (_Tile.IS_Occupied() == false)
		{
			UE_LOG(LogTemp, Log, TEXT("Tile %d (position : %s) is empty"), _Tile.GET_TileIndex(), *_Tile.s_TileCoordinates.ToString());
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Tile %d (position : %s) is NOT empty"), _Tile.GET_TileIndex(), *_Tile.s_TileCoordinates.ToString());
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("***"));
}

void SInventoryWidget::CLEAN_Everything()
{
	m_HorizontalLines   .Empty();
	m_VerticalLines     .Empty();
	m_Tiles             .Empty();
	m_CrossAnchors      .Empty();

	for (const auto& _Pair : m_InventoryItemsMap)
	{
		delete _Pair.Value;
	}

	m_InventoryItemsMap .Empty();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION