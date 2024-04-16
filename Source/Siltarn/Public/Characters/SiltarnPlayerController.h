#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SiltarnPlayerController.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogClass_SiltarnPlayerController, Log, All);

class AGameplayHUD;
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

	void SHUTDOWN_PickupItemWidget();
	void OPEN_ItemBag(AItemBagActor* p_BagActor, const FIntPoint& p_InventorySize, TArray<UPickupEntity*>& p_Items);

	// new
	class APickupActor* DropItemAsItIs(UPickupEntity* p_ItemEntity); // Need to think of a better name
	class AItemBagActor* DropItemAsBag(UClass* p_BagClass);
	
	// New inventory stuff
	//
	bool DOES_InventoryHasRoomForItem(const FIntPoint& p_ItemSize);
	FORCEINLINE bool IS_InventoryOpen() const { return m_bIsInventoryOpen; }
	FORCEINLINE AGameplayHUD* GET_GameplayHUD() const { return m_GameplayHUD; }
	void ADD_ItemToInventory(UPickupEntity* p_Item);


	void DISPLAY_InteractableEntityTag(const FString& p_ItemName);


	// new actions
	void OpenEscapeMenu();
	void OpenCharacterProfileWidget();


protected:

	virtual void BeginPlay() override;

private:

	AGameplayHUD* m_GameplayHUD = nullptr;
	bool m_bIsInventoryOpen;

	UPROPERTY()
	UInventoryManager* m_InventoryManager = nullptr;
};
