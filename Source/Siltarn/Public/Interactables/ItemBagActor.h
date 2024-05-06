#pragma once

#include "CoreMinimal.h"
#include "Interactables/InteractableActor.h"
#include "ItemBagActor.generated.h"

#define IMPULSE_FORCE_ON_SPAWN 250000.0f

DECLARE_LOG_CATEGORY_EXTERN(LogClass_AItemBagActor, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogClass_FBagTile, Log, All);


class UPickupEntity;
class SExternalInventoryWidget;

/*
	Luciole 25/03/2024
	We probably only need FBagTile for the computation of items' positions process. Thus, we could probably get rid of them all once the process is done.
	This also means we may not need to create instances of FBagTile on the heap. 
*/
class SILTARN_API FBagTile
{
public:

	FBagTile();
	FBagTile(bool p_bIsOccupied);
	~FBagTile();

	bool s_bIsOccupied;
	FIntPoint s_Coordinates;

	static int32 s_InstanceCount;
};



UCLASS()
class SILTARN_API AItemBagActor : public AInteractableActor
{
	GENERATED_BODY()

public:

	AItemBagActor();
	~AItemBagActor();

	void DEBUG_ListAllItemEntities();
	void DEBUG_ListAllItemEntitiesInQueue();
	void DEBUG_ListAllTilesMap();
	void DEBUG_ListAllUnoccupiedTilesMap();
	void DEBUG_ListAllItemEntitiesCoordinates();
	void SET_BagWidgetReference(TSharedPtr<SExternalInventoryWidget> p_BagWidget);
	bool LoadBag(UPickupEntity* p_ItemEntity);
	bool LoadBag(TArray<UPickupEntity*>& p_ItemEntities);
	void RemoveItem(UPickupEntity* p_ItemEntity);

protected:

	virtual void BeginPlay() override;

	void INTERFACE_Interact(ASiltarnPlayerController* p_SiltarnController) override;

private:

	bool ComputeItemsPositionsInInventory();
	bool ComputeBagInventoryWidgetData(UPickupEntity* p_ItemEntity);
	void FillUnoccupiedTiles(UPickupEntity* p_ItemEntity, int32 p_ItemIndexInQueue);
	bool DoesItemFit(int32 p_UnoccupiedTileIndex, UPickupEntity* p_ItemEntity);
	void InsertItemIntoUnoccupiedTiles(UPickupEntity* p_ItemEntity, int32 p_ItemIndexInQueue);

	bool AreWeDone();

	int32 FindLargestItemInQueue();


	enum EExpansionDir
	{
		EXPAND_ON_WIDTH  = 0,
		EXPAND_ON_HEIGHT = 1
	};

	// About inventory building
	void BuildTilesForItem(UPickupEntity* p_ItemEntity);
	void BuildLeftOverTiles(UPickupEntity* p_ItemEntity, EExpansionDir p_ExpansionDir);
	void ExpandInventory(UPickupEntity* p_ItemEntity, int32 p_ItemIndexInQueue);
	

private:

	UPROPERTY()
	TArray<UPickupEntity*> m_Items;

	UPROPERTY()
	TArray<UPickupEntity*> m_ItemsPlacementQueue;

	TSharedPtr<SExternalInventoryWidget> m_BagInventory = nullptr;




	TMap<FIntPoint, FBagTile*> m_TilesMap; // Need to check whether it'll be removed by the garbage collector or not 
	TArray<FBagTile*> m_UnoccupiedTiles; // Same here obviously

	FIntPoint m_InventorySize;

	TArray<FIntPoint> m_FittingUnoccupiedTilesCache;



	TArray<FIntPoint> m_AllOccupiedCoordinates;
};
