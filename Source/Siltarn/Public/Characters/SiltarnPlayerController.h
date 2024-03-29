#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SiltarnPlayerController.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogClass_SiltarnPlayerController, Log, All);

class AGameplayHUD;
class UInventory;
class UInteractableEntity;
class UPickupEntity;
class UInventoryManager;
class AItemBagActor;

UCLASS()
class SILTARN_API ASiltarnPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	ASiltarnPlayerController();

	virtual void PostInitializeComponents() override;

	void TOGGLE_Inventory();
	void TOGGLE_EchapMenu();
	void SHUTDOWN_PickupItemWidget();
	void DROP_Item(UPickupEntity* p_Item);
	//void DROP_Items(TArray<UPickupEntity*>& p_Items);
	void OPEN_ItemBag(const FIntPoint& p_InventorySize, TArray<UPickupEntity*>& p_Items);

	// new
	bool DropItem(UPickupEntity* p_Item, AItemBagActor* p_DroppingBag, class APickupActor* p_ItemActor);
	bool DropItems(TArray<UPickupEntity*>& p_Items);
	class APickupActor* DropItemAsItIs(UPickupEntity* p_ItemEntity); // Need to think of a better name
	class AItemBagActor* DropItemAsBag(UClass* p_BagClass);
	
	// New inventory stuff
	//
	bool DOES_InventoryHasRoomForItem(const FIntPoint& p_ItemSize);
	FORCEINLINE UInventory* GET_Inventory() const { return m_Inventory; }
	FORCEINLINE bool IS_InventoryOpen() const { return m_bIsInventoryOpen; }
	FORCEINLINE AGameplayHUD* GET_GameplayHUD() const { return m_GameplayHUD; }
	void ADD_ItemToInventory(UPickupEntity* p_Item);
	void DISPLAY_InteractableEntityTag(const FString& p_ItemName);


protected:

	virtual void BeginPlay() override;

private:

	// This function should run as early as possible, and only once. 
	// It both instantiates the UInventory class and initializes its SiltarnPlayerController reference
	void CREATE_Inventory();

private:

	AGameplayHUD* m_GameplayHUD = nullptr;

	// Luciole ! Test when destroying the AActor

	UPROPERTY()
	UInventory* m_Inventory = nullptr;

	bool m_bIsInventoryOpen;

	UPROPERTY()
	UInventoryManager* m_InventoryManager = nullptr;
};
