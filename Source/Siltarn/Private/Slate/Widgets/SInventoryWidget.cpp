#include "Slate/Widgets/SInventoryWidget.h"
#include "Siltarn/Public/Slate/Widgets/SInventoryItemWidget.h"
#include "Siltarn/Public/Slate/Widgets/SDebugWidget.h"
#include "Siltarn/Public/Slate/Menus/SProfileMenu.h"
#include "Siltarn/Public/Slate/Styles/SiltarnGeneralStyleContainer.h"
#include "Siltarn/Public/Slate/Styles/SiltarnStyleController.h"
#include "Siltarn/Public/HUDs/GameplayHUD.h"
#include "Siltarn/Public/Interactables/PickupEntity.h"
#include "Siltarn/Public/Inventory/InventoryManager.h"

DEFINE_LOG_CATEGORY(LogClass_SInventoryWidget );
DEFINE_LOG_CATEGORY(LogStruct_FLine           );
DEFINE_LOG_CATEGORY(LogStruct_FTile           );
DEFINE_LOG_CATEGORY(LogStruct_FInventoryItem  );
DEFINE_LOG_CATEGORY(LogStruct_FCrossAnchor    );


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
	// New
	if (s_ItemOwner)
	{
		return true;
	}
	else
	{
		return false;
	}



	// Old
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



TSharedPtr<SInventoryItemWidget> FTile::GET_OwnerNew() const
{
	return s_ItemOwner;
}



void FTile::SET_Owner(FInventoryItem* p_Item)
{
	s_Owner = p_Item;
}



void FTile::SET_OwnerNew(TSharedPtr<SInventoryItemWidget> p_ItemWidget)
{
	s_ItemOwner = p_ItemWidget;
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
	for (auto _it : m_TilesNew)
	{
		delete _it;
	}

	for (auto _it : m_CrossAnchorsNew)
	{
		delete _it;
	}

	m_TilesNew.Empty();
	m_CrossAnchorsNew.Empty();

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
	m_ParentWidget    = p_InArgs._a_ParentWidget.Get()   ;

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

	// New
	BUILD_Tiles();
	BUILD_CrossAnchors();	

	// Old
	//BuildTilesNew();
	//BuildCrossAnchorsNew();
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



void SInventoryWidget::ClearInventoryVisual()
{
	ClearVerticalAndHorizontalLines();
	DestroyTiles();
	DestroyCrossAnchors();
}



void SInventoryWidget::ClearVerticalAndHorizontalLines()
{
	m_VerticalLines.Empty();
	m_HorizontalLines.Empty();
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



void SInventoryWidget::DestroyTiles()
{
	UE_LOG(LogClass_SInventoryWidget, Warning, TEXT("Destroying all FTiles !"));
	m_Tiles.Empty();
}



void SInventoryWidget::BuildTilesNew()
{
	int32 _IterationCount = 0;

	for (int32 i = 0; i < m_NumberOfRows; i++)
	{
		for (int32 j = 0; j < m_NumberOfColumns; j++)
		{
			FTile* _NewTile = new FTile(FIntPoint(j, i), _IterationCount);
			m_TilesNew.Emplace (_NewTile);

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



void SInventoryWidget::DestroyCrossAnchors()
{
	m_CrossAnchors.Empty();
}


void SInventoryWidget::BuildCrossAnchorsNew()
{
	int32 _IterationCount = 0;

	for (int32 i = 1; i < m_NumberOfRows; i++)
	{
		for (int32 j = 1; j < m_NumberOfColumns; j++)
		{
			FCrossAnchor* _NewCross = new FCrossAnchor(FIntPoint(j, i), _IterationCount);
			m_CrossAnchorsNew.Emplace(_NewCross);

			_IterationCount++;
		}
	}

	UE_LOG(LogClass_SInventoryWidget, Log, TEXT("BUILD_CrossAnchors() : Number of FCrossAnchors built : %d"), _IterationCount);
}



TSharedPtr<SInventoryItemWidget> SInventoryWidget::ConstructItemWidget(UPickupEntity* p_ItemEntity, FTile* p_ControlTile, EInventoryItemWidgetLocation p_ItemWidgetLocation)
{
	if (!p_ItemEntity)
	{
		UE_LOG(LogClass_SInventoryWidget, Error, TEXT("ConstructItemWidget() : p_ItemEntity is NULL !"));
		return nullptr;
	}

	if (!p_ControlTile)
	{
		UE_LOG(LogClass_SInventoryWidget, Error, TEXT("ContructItemWidget() : p_ControlTile is NULL !"));
		return nullptr;
	}


	// The widget location (0, 0) is its top left corner
	const float _SlotPosX = p_ControlTile->s_TileCoordinates.X * m_TileSize;
	const float _SlotPosY = p_ControlTile->s_TileCoordinates.Y * m_TileSize;

	const float _SlotSizeX = p_ItemEntity->GET_InventorySpace().X * m_TileSize;
	const float _SlotSizeY = p_ItemEntity->GET_InventorySpace().Y * m_TileSize;

	TSharedPtr<SInventoryItemWidget> _ItemWidget = nullptr             ;
	SCanvas::FSlot*                  _Slot       = nullptr             ;
	FVector2D                        _ItemSize(_SlotSizeX, _SlotSizeY) ;
	FVector2D                        _ItemPos(_SlotPosX, _SlotPosY)    ;


	m_Canvas->AddSlot()
	.Position(_ItemPos)
	.Size(_ItemSize)
	[
		SAssignNew(_ItemWidget, SInventoryItemWidget)
		.a_ItemData(p_ItemEntity)
		.a_ItemSize(_ItemSize)
		.a_InventoryOwner(this)
		.a_InventoryItemWidgetLocation(p_ItemWidgetLocation)
	]
	.Expose(_Slot);


	if (_ItemWidget.IsValid() && _Slot)
	{
		// Luciole 08/04/2024 || Still unsure whether we should deal with FTile in m_Tiles indexes or with FTile* pointers directly
		// Luciole 10/04/2024 || UPDATE : it was decided to use FTile* in order to avoid going a long way to free the occupied FTiles when dropping the item
		// Note to myself : eventually need to remove these comments and move this explanation and this "history" to a clean documentation.
		//_ItemWidget->AssignTiles(m_CachedTilesIndexes);
		_ItemWidget->AssignTiles(m_Tiles, m_CachedTilesIndexes);
		_ItemWidget->SET_ItemCanvasSlot(_Slot);

		return _ItemWidget;
	}

	return nullptr;	
}



TSharedPtr<SInventoryItemWidget> SInventoryWidget::ConstructItemWidget(UPickupEntity* p_ItemEntity, EInventoryItemWidgetLocation p_ItemWidgetLocation)
{
	int32 _CTileIndex = TileCoordinatesToTileIndex(p_ItemEntity->GET_InventoryLocationTile());
	FTile* _CTile = &m_Tiles[_CTileIndex];

	if (_CTile)
	{
		return ConstructItemWidget(p_ItemEntity, _CTile, p_ItemWidgetLocation);
	}

	return nullptr;
}



bool SInventoryWidget::IsThereRoomInInventory(const FIntPoint& p_ItemSize)
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
	const FIntPoint _TopLeftTile = IndexToTileCoordinates(p_TileIndex);

	// Checking for "item out of grid" case
	if (_TopLeftTile.Y + p_ItemSize.Y > m_NumberOfRows || _TopLeftTile.X + p_ItemSize.X > m_NumberOfColumns)
	{
		UE_LOG(LogClass_SInventoryWidget, Warning, TEXT("IS_RoomAvailableAtTileIndex() : FTile %d -> Out of grid case."), p_TileIndex);
		return false;
	}

	// Old
	for (int32 y = _TopLeftTile.Y; y < p_ItemSize.Y + _TopLeftTile.Y; y++)
	{
		for (int32 x = _TopLeftTile.X; x < p_ItemSize.X + _TopLeftTile.X; x++)
		{
			const FIntPoint _TileCoordinates = FIntPoint(x, y)                              ;
			const int32     _TileIndex       = TileCoordinatesToTileIndex(_TileCoordinates) ;

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



int32 SInventoryWidget::TileCoordinatesToTileIndexStatic(const FIntPoint& p_Coordinates, int32 p_NumberOfColumns)
{
	return p_Coordinates.X + (p_Coordinates.Y * p_NumberOfColumns);
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

	// Old
	FTile* _TilePtr = &m_Tiles[_Index];

	// New
	//FTile* _TilePtr = m_TilesNew[_Index];

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

	// Old
	FCrossAnchor* _CrossAnchorPtr = &m_CrossAnchors[_Index];

	// New
	//FCrossAnchor* _CrossAnchorPtr = m_CrossAnchorsNew[_Index];

	return _CrossAnchorPtr;
}



FReply SInventoryWidget::OnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent)
{
	TSharedPtr<FInventoryItemDragDrop> _Operation = DragDropEvent.GetOperationAs<FInventoryItemDragDrop>();

	if (_Operation.IsValid())
	{
		FVector2D _MouseRelativePos = MyGeometry.AbsoluteToLocal(DragDropEvent.GetScreenSpacePosition());
		FVector2D _TargetPos = _MouseRelativePos - _Operation->GET_WidgetMiddlePosition();

		FTile* _TargetTile = nullptr;
		UPickupEntity* _ItemEntity = _Operation->GET_ItemEntity();

		if (_ItemEntity)
		{
			EItemAnchorType _AnchorType = _ItemEntity->GET_AnchorType();

			if (_AnchorType == EItemAnchorType::Cross)
			{
				FCrossAnchor* _Cross = RelativeLocationToCrossAnchor(CENTER_Location(_ItemEntity, _TargetPos));
				int32 _TileIndex = TileCoordinatesToTileIndex(_Cross->GET_TopLeftTileCoordinates(_ItemEntity));
				// Old
				_TargetTile = &m_Tiles[_TileIndex];

				// New
				//_TargetTile = m_TilesNew[_TileIndex];
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


			// Luciole 12/03/2024 || Investigate on the use of GET_Tile(). Do we really need to have a reference to the control FTile within SInventoryItemWidget ??
			//TryDroppingItemToTile(_Operation->GET_InventoryItemClass(), _ItemEntity, _Operation->GET_Tile(), _TargetTile);



			// Luciole 10/04/2024 || Finish this + what is the use of _Operation->GET_Tile() ? We do not use the original FTile anyway.
			TryDroppingItemToTileNew(_Operation->GET_ItemWidget(), _TargetTile);





			FSlateApplication::Get().SetUserFocus(0, SharedThis(this), EFocusCause::SetDirectly);

			return FReply::Handled();
		}
	}

	return FReply::Unhandled();
}



bool SInventoryWidget::TryDroppingItemToTileNew(TSharedPtr<SInventoryItemWidget> p_ItemWidget, FTile* p_TargetTile)
{
	bool _bIsTileAvailable = IsTileAvailableNew(p_ItemWidget, p_TargetTile);

	if (_bIsTileAvailable)
	{
		p_ItemWidget->FreeOccupiedTiles();
		//p_InventoryItem->EmptyTilesArray();

		p_ItemWidget->AssignTiles(m_CachedTilesIndexes);

		/*for (int32 i = 0; i < m_CachedTilesIndexes.Num(); i++)
		{
			// Old
			FTile* _Tile = &m_Tiles[m_CachedTilesIndexes[i]];

			// New
			//FTile* _Tile = m_TilesNew[m_CachedTilesIndexes[i]];
			_Tile->SET_OwnerNew(p_ItemWidget);

			if (!p_InventoryItem->EmplaceNewTileInArray(_Tile))
			{
				return false;
			}

			UE_LOG(LogTemp, Log, TEXT("FTile %d has been occupied by the item !"), _Tile->GET_TileIndex());
		}*/

		//p_InventoryItem->UPDATE_Tile();
		//MoveItemCanvasSlot(p_InventoryItem, p_TargetTile->GET_RelativeCoordinates(m_TileSize));
		MoveItemCanvasSlotNew(p_ItemWidget, p_TargetTile->GET_RelativeCoordinates(m_TileSize));
	}
	else
	{
		p_ItemWidget->SetVisibility(EVisibility::Visible);
		//p_InventoryItem->GET_ItemWidget()->SetVisibility(EVisibility::Visible);
	}

	CleanCachedTilesIndexes();
	return false;

	return false;
}

bool SInventoryWidget::IsTileAvailableNew(TSharedPtr<SInventoryItemWidget> p_ItemWidget, FTile* p_TargetTile)
{
	if (!p_ItemWidget.IsValid())
	{
		UE_LOG(LogClass_SInventoryWidget, Error, TEXT("IsTileAvailableNew() : p_ItemWidget is INVALID !"));
		return false;
	}

	if (!p_TargetTile)
	{
		UE_LOG(LogClass_SInventoryWidget, Error, TEXT("IsTileAvailableNew() : p_TargetTile is NULL !"));
		return false;
	}

	const FIntPoint _TileCoordinates = p_TargetTile->s_TileCoordinates;
	const FIntPoint _ItemDimensions = p_ItemWidget->GET_ItemData()->GET_InventorySpace();

	if (_TileCoordinates.X + _ItemDimensions.X > m_NumberOfColumns || _TileCoordinates.Y + _ItemDimensions.Y > m_NumberOfRows)
	{
		UE_LOG(LogClass_SInventoryWidget, Warning, TEXT("IS_TileAvailable_a() : Out of grid case."));
		return false;
	}

	for (int32 y = _TileCoordinates.Y; y < _TileCoordinates.Y + _ItemDimensions.Y; y++)
	{
		for (int32 x = _TileCoordinates.X; x < _TileCoordinates.X + _ItemDimensions.X; x++)
		{
			const FIntPoint _TileCoordinates = FIntPoint(x, y);
			const int32 _TileIndex = TileCoordinatesToTileIndex(_TileCoordinates);

			// Old
			if (_TileIndex >= m_Tiles.Num())
			{
				UE_LOG(LogClass_SInventoryWidget, Warning, TEXT("IS_TileAvailable() : Last FTile case. (Checking for FTile %d"), _TileIndex);

				CleanCachedTilesIndexes();
				return false;
			}

			if (m_Tiles[_TileIndex].IS_Occupied())
			{
				if (*m_Tiles[_TileIndex].GET_OwnerNew() != *p_ItemWidget.Get())
				{
					UE_LOG(LogClass_SInventoryWidget, Warning, TEXT("IS_TileAvailable() : FTile occupied case. (Checking for FTile %d"), _TileIndex);
					CleanCachedTilesIndexes();
					return false;
				}
			}

			// New
			/*if (_TileIndex >= m_TilesNew.Num())
			{
				UE_LOG(LogClass_SInventoryWidget, Warning, TEXT("IS_TileAvailable() : Last FTile case. (Checking for FTile %d"), _TileIndex);

				CleanCachedTilesIndexes();
				return false;
			}

			if (m_TilesNew[_TileIndex]->IS_Occupied())
			{
				if (*m_TilesNew[_TileIndex]->GET_Owner() != *p_InventoryItem)
				{
					UE_LOG(LogClass_SInventoryWidget, Warning, TEXT("IS_TileAvailable() : FTile occupied case. (Checking for FTile %d"), _TileIndex);
					CleanCachedTilesIndexes();
					return false;
				}
			}*/

			m_CachedTilesIndexes.Emplace(_TileIndex);
		}
	}

	return true;
}



void SInventoryWidget::MoveItemCanvasSlotNew(TSharedPtr<SInventoryItemWidget> p_ItemWidget, const FVector2D& p_Location)
{
	if (!p_ItemWidget.IsValid())
	{
		UE_LOG(LogClass_SInventoryWidget, Error, TEXT("MoveItemCanvasSlotNew() : p_ItemWidget is INVALID !"));
		return;
	}


	// Luciole 10/04/2024 || Should we do this here or create a function in SInventoryItemWidget to move its own FSlot ? 
	p_ItemWidget->GET_ItemCanvasSlot()->SetPosition(p_Location);
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

	// Old
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

	/*for (FTile* _Tile : m_TilesNew)
	{
		if (_Tile->IS_Occupied() == false)
		{
			UE_LOG(LogTemp, Log, TEXT("Tile %d (position : %s) is empty"), _Tile->GET_TileIndex(), *_Tile->s_TileCoordinates.ToString());
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Tile %d (position : %s) is NOT empty"), _Tile->GET_TileIndex(), *_Tile->s_TileCoordinates.ToString());
		}
	}*/

	UE_LOG(LogTemp, Warning, TEXT("***"));
}



void SInventoryWidget::CleanCachedTilesIndexes()
{
	m_CachedTileIndex = -1;
	m_CachedTilesIndexes.Empty();
}



void SInventoryWidget::HideAllItemsSetForGroupDrop()
{
	/*for (int32 i = 0; i < m_ItemsSelectedForGroupDrop.Num(); i++)
	{
		FInventoryItem* _ItemPtr = *m_InventoryItemsMap.Find(m_ItemsSelectedForGroupDrop[i]);

		if (_ItemPtr)
		{
			_ItemPtr->GET_ItemWidget()->SetVisibility(EVisibility::Hidden);
		}
	}*/

	/* old
	for (auto _InventoryItem : m_DroppingItemsCache)
	{
		_InventoryItem->GET_ItemWidget()->SetVisibility(EVisibility::Hidden);
	}
	*/

	// new
	for (auto _ItemWidget : m_DroppingItemsCacheNew)
	{
		_ItemWidget->SetVisibility(EVisibility::Hidden);
	}
}



void SInventoryWidget::SetInventoryItemForGroupDropping(TSharedPtr<SInventoryItemWidget> p_ItemWidget)
{
	if (p_ItemWidget)
	{
		m_DroppingItemsCacheNew.Emplace(p_ItemWidget);
	}
}



void SInventoryWidget::RemoveInventoryItemFromGroupDropping(TSharedPtr<SInventoryItemWidget> p_ItemWidget)
{
	if (p_ItemWidget)
	{
		m_DroppingItemsCacheNew.Remove(p_ItemWidget);
	}
}



void SInventoryWidget::SetItemsForGroupDrop()
{
	// old
	/*
	if (m_InventoryManager)
	{
		for (int32 i = 0; i < m_DroppingItemsCache.Num(); i++)
		{
			//int32 _ItemEntityId = m_DroppingItemsCache[i]->GET_UniqueId(); // old
			UPickupEntity* _ItemEntity = m_DroppingItemsCache[i]->GET_ItemWidget()->GET_ItemData();

			//m_InventoryManager->SetItemForGroupDrop(_ItemEntityId); // old
			m_InventoryManager->SetItemForGroupDrop(_ItemEntity);
		}

		m_InventoryManager->DropItems();
	}*/

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



void SInventoryWidget::SET_InventoryManager(UInventoryManager* p_InventoryManager)
{
	m_InventoryManager = p_InventoryManager;
}



FReply SInventoryWidget::OnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::I || InKeyEvent.GetKey() == EKeys::Escape)
	{
		if (m_HUDOwner.IsValid())
		{
			m_HUDOwner->CloseCharacterProfileWidget();
		}
	}

	return FReply::Handled();
}



void SInventoryWidget::ResetFocus()
{
	FSlateApplication::Get().SetUserFocus(0, SharedThis(this));
}



void SInventoryWidget::MoveItemToPlayerInventory(TSharedPtr<SInventoryItemWidget> p_ItemWidget)
{
	/*if (m_InventoryManager && p_ItemEntity)
	{
		bool _bWasItemMoved = m_InventoryManager->MoveItemToPlayerInventory(p_ItemEntity);

		if (_bWasItemMoved)
		{

		}
	}*/


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

			}
		}
	}
	else
	{
		UE_LOG(LogClass_SInventoryWidget, Warning, TEXT("I'm NULL !"));
	}
}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION