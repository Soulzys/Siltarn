#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Inventory.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogClass_UInventory, Log, All);

class ASiltarnPlayerController;
class UPickupEntity;

/*
	This class deals with the player's inventory, no matter whether the player is in the Pyralinth or in Salnaxar.
	Both the Pyralinth HUD (currently still named GameplayHUD) and the Salnaxar HUD get their item information from this class.
	Eventually, this class will also be dealing with putting the inventory data into an external database for persistency purposes.

	One instance of class is created when the player first launch the game, in the SiltarnGameMode class. 
	Its references are then shared across the other classes such as ASiltarnCharacter.
*/
UCLASS()
class SILTARN_API UInventory : public UObject
{
	GENERATED_BODY()

public:

	UInventory();
	~UInventory();

	void SET_SiltarnPlayerController(ASiltarnPlayerController* p_Ref);

	void ADD_Item    (UPickupEntity* p_Item, bool p_bDisplayLogs = false) ;
	void REMOVE_Item (UPickupEntity* p_Item, bool p_bDisplayLogs = false) ;

private:

	/*
		Need to find a way to do it more efficiently.
		It is currently done this way because I need to advance fast but it must be rewritten eventually.
	*/
	int32 CREATE_NewItemId();

	void DEBUG_DisplayItemDataThroughUELogs(int32 p_ItemId);
	void DEBUG_DisplayAllItemsDataThroughUELogs();

private:

	ASiltarnPlayerController* m_PlayerController = nullptr;

	/*
		If UPROPERTY() is not added, our TArray will eventually get destroyed by the unreal garbage manager
	*/
	UPROPERTY()
	TArray<UPickupEntity*> m_Items;

	static uint32 m_InstanceCount;
};
