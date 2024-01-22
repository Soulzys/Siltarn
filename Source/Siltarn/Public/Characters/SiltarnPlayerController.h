#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SiltarnPlayerController.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogClass_SiltarnPlayerController, Log, All);

class AGameplayHUD;
class UInventory;
class UInteractableEntity;

struct FPickupItemData;

UCLASS()
class SILTARN_API ASiltarnPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	virtual void PostInitializeComponents() override;

	void TOGGLE_Inventory();
	void TOGGLE_EchapMenu();
	void SHUTDOWN_PickupItemWidget();
	void DROP_Item(UPickupEntity* p_Item);
	
	// New inventory stuff
	//
	bool DOES_InventoryHasRoomForItem(const FIntPoint& p_ItemSize);
	FORCEINLINE UInventory* GET_Inventory() const { return m_Inventory; }
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
};