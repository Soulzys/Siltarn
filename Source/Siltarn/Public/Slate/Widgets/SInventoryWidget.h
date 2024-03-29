#pragma once

#include "Siltarn/Public/HUDS/GameplayHUD.h"
#include "Siltarn/Public/Inventory/InventoryEnumsLib.h"
#include "SlateBasics.h"

DECLARE_LOG_CATEGORY_EXTERN(LogClass_SInventoryWidget, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogStruct_FLine          , Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogStruct_FTile          , Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogStruct_FInventoryItem , Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogStruct_FCrossAnchor   , Log, All);

class SCanvas              ;
class SBorder              ;
class SInventoryItemWidget ;
class SDebugWidget         ;
class SProfileMenu         ;
class UPickupEntity        ;
class UInventoryManager    ;

/*
	The sole purpose of this struct is to draw the horizontal and vertical lines of the inventory widget in Paint()
*/
struct SILTARN_API FLine
{
	FLine();
	FLine(FVector2D p_StartPoint, FVector2D p_EndPoint);
	~FLine();

	const FVector2D& GET_StartPoint() const;
	TArray<FVector2D>& GET_PointsArray();

public:

	TArray<FVector2D> s_PointsArray;

private:

	static int32 s_InstanceCount;
};



/*
	This struct is the representation of the "squares" of the inventory. It is used to move the items around and dock them to precise locations. 

	+----+
	|    |
	+----+
*/
struct SILTARN_API FTile
{
	FTile();
	FTile(FIntPoint p_Coordinates, int32 p_Id);
	~FTile();

	void DEBUG_DisplayStructDataThroughLogs();
	uint32 GET_TileIndex() const; 
	FVector2D GET_RelativeCoordinates(const int32 p_TileSize) const;

	/*
		If p_item is a nullptr, it means the FTile has been freed.
	*/
	void SET_Owner(class FInventoryItem* p_Item);

	/*
		Returns null if unoccupied. 
		If occupied, the return value can be used to compare whether the FInventoryItem that called the function is also this FTile owner or not.
	*/
	bool IS_Occupied();

	/*
		Used to check whether the FTile is occupied or not. If s_Owner is not NULL, it means the FTile is occupied. 
	*/
	class FInventoryItem* GET_Owner() const;

public:

	FIntPoint s_TileCoordinates = FIntPoint(-1);
	bool s_bIsEmpty = true;
	
private:

	uint32 s_TileIndex; // Can also be seen as the FTile struct's id. It serves as the index to retrieve an FTile from the m_Tiles TArray.
	FVector2D s_RelativeCoordinates = FVector2D(-100.0f);
	class FInventoryItem* s_Owner = nullptr;

	static int32 s_InstanceCount;


	// This part is so that we can hash FTile and therefore use it in a TMap
	//
	friend bool operator==(const FTile& p_First, const FTile& p_Second)
	{
		return (p_First.s_TileIndex == p_Second.s_TileIndex);
	}

	friend uint32 GetTypeHash(const FTile& p_Other)
	{
		return GetTypeHash(p_Other.s_TileIndex);
	}
};



/*
	This struct is the representation of the "cross" of the inventory. It is used to move the items around and dock them to precise locations.

	   |	
	---+---
	   |
*/
struct SILTARN_API FCrossAnchor
{
	FCrossAnchor();
	FCrossAnchor(const FIntPoint& p_Coordinates, int32 p_Id);
	~FCrossAnchor();

	const FIntPoint GET_TopLeftTileCoordinates(UPickupEntity* p_ItemEntity) const;
	FIntPoint GET_Coordinates() const;
	int32 GET_Index() const;

private:

	int32 s_CrossAnchorIndex;
	FVector2D s_Position;
	FIntPoint s_Coordinates;

	static int32 s_InstanceCount;
};


// New FInventoryItem
// Luciole 10/03/2024 || We should use this class to conduct some test and improve our understanding about move semantics and such

/*
	Luciole 29/03/2024 || Consider having a UPickupEntity* variable, which would replace s_UniqueId and wouldn't need such a heavy constructor as most of the necessary
	data are already inside our UPickupEntity object.
*/
class SILTARN_API FInventoryItem
{
public:

	FInventoryItem();
	// Old
	FInventoryItem(TArray<int32>& p_CachedTilesIndex, TArray<FTile>& p_Tiles, int64 p_UniqueId);
	FInventoryItem(const FIntPoint& p_OriginTile, const FIntPoint& p_ItemSize, int64 p_UniqueId, int32 p_NumberOfColumns, TArray<FTile>& p_Tiles);
	// New
	//FInventoryItem(TArray<int32>& p_CachedTilesIndex, TArray<FTile*>& p_Tiles, int32 p_UniqueId);
	~FInventoryItem();

	void MoveItem(const FVector2D& p_NewLocation) const;
	void FreeOccupiedTiles();
	void EmptyTilesArray();
	bool EmplaceNewTileInArray(FTile* p_Tile);
	void UPDATE_Tile();

	FTile* GET_ControlTile() const;
	__forceinline TSharedPtr<SInventoryItemWidget> GET_ItemWidget() const { return s_ItemWidget ; }
	__forceinline int64                            GET_UniqueId()   const { return s_UniqueId   ; }

	void SET_ItemWidget(TSharedPtr<SInventoryItemWidget> p_ItemWidget);
	void SET_CanvasSlot(SCanvas::FSlot* p_Slot);

private:

	TSharedPtr<SInventoryItemWidget> s_ItemWidget = nullptr;

	/*
		Used to modify the slot's position when dragging the item around
	*/
	SCanvas::FSlot* s_CanvasSlot = nullptr;

	/*
		All the FTile that are occupied by the SInventoryItemWidget
	*/
	TArray<FTile*> s_OccupyingTiles;

	int64 s_UniqueId;

	static int32 s_InstanceNumber;

private:

	friend bool operator==(const FInventoryItem& p_A, const FInventoryItem& p_B)
	{
		return (p_A.s_OccupyingTiles[0] == p_B.s_OccupyingTiles[0]);
	}

	friend bool operator!=(const FInventoryItem& p_A, const FInventoryItem& p_B)
	{
		return (p_A.s_OccupyingTiles[0] != p_B.s_OccupyingTiles[0]);
	}
};


/*
	Luciole

	1/ Check if when removing an item from the inventory, its SCanvas's slot is also removed or if we need to manually delete it.
	2/ Investigate whether it would be better to make the item widget move itself by setting its slot position in SInventoryItemWidget, 
	   or if we should keep its slot reference in FInventoryItem and move it from here only. When I think about it atm, it appears to make more sense. 
*/


class SILTARN_API SInventoryWidget : public SCompoundWidget
{

public:
	
	SLATE_BEGIN_ARGS(SInventoryWidget) {}

	SLATE_ATTRIBUTE(int32        , a_NumberOfColumns)
	SLATE_ATTRIBUTE(int32        , a_NumberOfRows   )
	SLATE_ATTRIBUTE(int32        , a_TileSize       )
	SLATE_ATTRIBUTE(SProfileMenu*, a_ParentWidget   )

	SLATE_ARGUMENT(TWeakObjectPtr<AGameplayHUD>, a_HUDOwner)

	SLATE_END_ARGS()

public:

	SInventoryWidget();
	~SInventoryWidget();

	virtual int32  OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual bool   SupportsKeyboardFocus() const override { return true; }
	virtual FReply OnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) override;

	void Construct(const FArguments& p_InArgs);
	bool IsThereRoomInInventory(const FIntPoint& p_ItemSize);

	void DEBUG_DisplayTilesStatusThroughUELogs();

	TArray<FInventoryItem*>& GET_FInventoryItemsCache(); // new

	void HideAllItemsSetForGroupDrop();

	// Group dropping new
	void SetInventoryItemForGroupDropping(int32 p_ItemId);
	void SetInventoryItemForGroupDropping(FInventoryItem* p_InventoryItem);
	void SetItemsForGroupDrop();

	void SET_InventoryManager(UInventoryManager* p_InventoryManager);

	static int32 TileCoordinatesToTileIndexStatic(const FIntPoint& p_Coordinates, int32 p_NumberOfColumns);

protected:

	void COMPUTE_HorizontalLines();
	void COMPUTE_VerticalLines();
	void BUILD_Tiles(); // old
	void BuildTilesNew(); // new
	void BUILD_CrossAnchors(); // old
	void BuildCrossAnchorsNew(); // new
	void ConstructCanvasItemSlot(UPickupEntity* p_ItemEntity, FTile* p_ControlTile, FInventoryItem* p_InventoryItem, EInventoryItemWidgetLocation p_ItemWidgetLocation); // new 

	/*
		Used when dragging & dropping item within the inventory. Called in OnDrop().
	*/
	bool TryDroppingItemToTile(FInventoryItem* p_InventoryItem, UPickupEntity* p_ItemEntity, FTile* p_OriginalTile, FTile* p_TargetTile); // new

	/*
		Paired with TryDroppingItemToTile()
	*/
	bool IsTileAvailable(FInventoryItem* p_InventoryItem, UPickupEntity* p_ItemEntity, FTile* p_TargetTile);
	
	/*
		Gets an item widget's relative location and returns it centered according to the item size and the inventory FTile size.
		If we do not centers it, the relative location used when dropping the dragged widget will be the top-left corner of the widget's location.
	*/
	FVector2D CENTER_Location(UPickupEntity* p_ItemEntity, const FVector2D& p_Location); // New way

	/*
		Move SInventoryItemWidget to a new location at the end of a drag and drop operation.
		The location is relative to the canvas as it is actually the SCanvas::FSlot that is being moved.
	*/
	void MoveItemCanvasSlot(FInventoryItem* p_ItemEntity, const FVector2D& p_Location);

	/*
		Paired with TRY_AddingItemToInventory()
		Go through every tile that would be required to fill the Item into the inventory, from the very top left tile. 
		Returns true if the following checks are clear : 
			- the item would not get out of the grid
			- there are no other items that is in the way
	*/
	bool IS_RoomAvailableAtTileIndex(const FIntPoint& p_ItemSize, int32 p_TileIndex);
	



	int32 ComputeUniqueItemId();	


protected:

	void CleanCachedTilesIndexes();

	// Utilitary FTile functions
	//
	FIntPoint IndexToTileCoordinates(int32            p_Index);
	int32     TileCoordinatesToTileIndex(const FIntPoint& p_Coordinates );
	

	/*
		Input a FCrossAnchor coordinates and output its id, which serves as its index in m_CrossAnchors TArray
	*/
	int32         CrossAnchorCoordinatesToCrossAnchorIndex(const FIntPoint& p_Coordinates                      );
	FVector2D     TileToRelativeLocation                  (FTile*           p_Tile                             );
	FTile*        RelativeLocationToTile                  (const FVector2D& p_Loc, const FIntPoint& p_ItemSize );
	FCrossAnchor* RelativeLocationToCrossAnchor           (const FVector2D& p_Loc                              );

protected:

	int32 m_NumberOfColumns;
	int32 m_NumberOfRows;
	int32 m_TileSize;

	TArray<FLine> m_HorizontalLines;
	TArray<FLine> m_VerticalLines;

	/*
		Every FTile in the SInventory is stored here.
		They are created and added to the TArray in BUILD_Tiles()

		Luciole 10/03/2024 || We should probably use * or &
	*/
	TArray<FTile> m_Tiles;
	TArray<FTile*> m_TilesNew;

	/*
		Every FCrossAnchor in the SInventory is stored here.
		They are created and added to the TArray in BUILD_CrossAnchors()
	*/
	TArray<FCrossAnchor> m_CrossAnchors;
	TArray<FCrossAnchor*> m_CrossAnchorsNew;

	/*
		Contains all the items in the inventory and their respective occupying FTile
	*/
	TMap<int32, FInventoryItem*> m_InventoryItemsMap;
	TArray<FInventoryItem*> m_DroppingItemsCache;

	// Slate stuff
	//
	TWeakObjectPtr<AGameplayHUD> m_HUDOwner     = nullptr   ;
	TSharedPtr<SCanvas>          m_Canvas       = nullptr   ;
	TSharedPtr<SBorder>          m_BorderTest   = nullptr   ;		
	SProfileMenu*                m_ParentWidget = nullptr   ;
	FLinearColor                 m_InventoryBackgroundColor ;


	/*
		Use when SHIFT + clicking one or several items in order to drop them together in an AItemBag
	*/
	TArray<FTile*> m_DroppingItemsGroup;
	//TArray<UPickupEntity*> m_DroppingItemDataGroup; // There must be another way to do this. We shouldn't need two arrays for this task.



	/*
		Cached data. They are used when an item is trying to be added to the inventory.
		Since we first check if there is enough space in the inventory for the item before actually adding the item, 
		we need to retain some data to complete the adding process.
	*/
	int32 m_CachedTileIndex; // Refers to the soon-to-be-added item's Control Tile
	// Luciole 10/03/2024 || We should probably switch this to a TArray<uint8> or a TArray<uint16>
	TArray<int32> m_CachedTilesIndexes; // Refers to all the FTile that will be occupied by the item once it is added to the inventory


	/*
		Used to compute a new unique id for every item added to the inventory
	*/
	TArray<int32> m_ItemsId;



	TArray<int32> m_ItemsSelectedForGroupDrop;



	UInventoryManager* m_InventoryManager = nullptr;


	

	static int32 m_InstanceCount;
};
