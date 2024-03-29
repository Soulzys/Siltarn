#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InventoryManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogClass_UInventoryManager, Log, All);

class UPickupEntity;
class SInGamePlayerInventoryWidget;
class ASiltarnPlayerController;

/*
	Luciole 15/03/2024

	Think of loading the AItemBagActor's blueprint class right here, from the Constructor(). 
	This way, we wouldn't need to pass a UPickupEntity* reference in DropItemAsBag(). The function would then be reusable for when dropping several
	items at once (group dropping)
*/

UCLASS()
class SILTARN_API UInventoryManager : public UObject
{
	GENERATED_BODY()

public:

	UInventoryManager();

	void AddItemToPlayerInventory(UPickupEntity* p_ItemEntity);
	bool DoesPlayerInventoryHasRoomForNewItem(const FIntPoint& p_ItemSize);
	void DropItem(UPickupEntity* p_ItemEntity);
	void DropItems();
	void SetItemForGroupDrop(int32 p_ItemEntityId); // Old
	void SetItemForGroupDrop(UPickupEntity* p_ItemEntity); // New

	void SET_InventoryWidget(TSharedPtr<SInGamePlayerInventoryWidget> p_InventoryWidget);
	void SET_SiltarnPlayerController(ASiltarnPlayerController* p_Controller);

	void DEBUG_DisplayAllItemsName() const;

private:

	int32 CreateUniqueInventoryId();
	void PlaceItemEntityInArray(UPickupEntity* p_ItemEntity, int32 p_ItemInventoryId);

	void DropItemAsItIs(UPickupEntity* p_ItemEntity);
	void DropItemAsBag(UClass* p_BagClass, UPickupEntity* p_ItemEntit);

private:

	UPROPERTY()
	TArray<UPickupEntity*> m_ItemEntities;

	UPROPERTY()
	TArray<UPickupEntity*> m_ItemsToDrop;

	UPROPERTY()
	TSubclassOf<class AItemBagActor> m_BagClass;

	// I don't think we need UPROPERTY() for these two but if something gets funny, we may need to investigate the issue from here
	TSharedPtr<SInGamePlayerInventoryWidget> m_PlayerInventoryWidget = nullptr;
	ASiltarnPlayerController* m_SiltarnController = nullptr;
};
