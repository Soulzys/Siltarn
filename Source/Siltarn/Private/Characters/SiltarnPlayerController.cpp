#include "Characters/SiltarnPlayerController.h"
#include "Siltarn/Public/HUDs/GameplayHUD.h"
#include "Siltarn/Public/Interactables/InteractableEntity.h"
#include "Siltarn/Public/Interactables/PickupActor.h"
#include "Siltarn/Public/Inventory/InventoryManager.h"
#include "Siltarn/Public/Characters/SiltarnCharacter.h"

DEFINE_LOG_CATEGORY(LogClass_SiltarnPlayerController);

ASiltarnPlayerController::ASiltarnPlayerController()
{
	m_bIsInventoryOpen = false;
}

void ASiltarnPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ASiltarnPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Careful ! This implementation only deals with AGameplayHUD, as if the game started in the "Pyralinth". 
	// However, the game is eventually going to be starting from the "base camp" world, which should have another HUD.
	m_GameplayHUD = Cast<AGameplayHUD>(GetHUD());

	// Inventory
	m_InventoryManager = NewObject<UInventoryManager>();
	m_InventoryManager->SET_InventoryWidget(m_GameplayHUD->GET_InGamePlayerInventoryWidget());
	m_InventoryManager->SET_SiltarnPlayerController(this);

	/*
		We give SProfileMenu a reference to UInventoryManager in order to manage the dropping of items.
	*/
	m_GameplayHUD->SET_InventoryManager(m_InventoryManager);
}



void ASiltarnPlayerController::SHUTDOWN_PickupItemWidget()
{
	if (m_GameplayHUD)
	{
		m_GameplayHUD->SHUTDOWN_PickupItemWidget();
	}
}



bool ASiltarnPlayerController::DOES_InventoryHasRoomForItem(const FIntPoint& p_ItemSize)
{
	if (m_InventoryManager)
	{
		return m_InventoryManager->DoesPlayerInventoryHasRoomForNewItem(p_ItemSize);
	}

	return false;
}



void ASiltarnPlayerController::ADD_ItemToInventory(UPickupEntity* p_Item)
{
	if (m_InventoryManager && p_Item)
	{
		m_InventoryManager->AddItemToPlayerInventory(p_Item);
	}
}



void ASiltarnPlayerController::DISPLAY_InteractableEntityTag(const FString& p_ItemName)
{
	if (m_GameplayHUD)
	{
		m_GameplayHUD->DISPLAY_InteractableEntityTag(p_ItemName);
	}
}



void ASiltarnPlayerController::OPEN_ItemBag(AItemBagActor* p_BagActor, const FIntPoint& p_InventorySize, TArray<UPickupEntity*>& p_Items)
{
	if (m_GameplayHUD)
	{
		m_GameplayHUD->OPEN_ItemBag(p_BagActor, p_InventorySize, p_Items);
	}
}



APickupActor* ASiltarnPlayerController::DropItemAsItIs(UPickupEntity* p_ItemEntity)
{
	if (p_ItemEntity)
	{
		ASiltarnCharacter* _Character = Cast<ASiltarnCharacter>(GetCharacter());

		if (_Character)
		{
			return _Character->DropItemAsItIs(p_ItemEntity);
		}
	}

	return nullptr;
}



AItemBagActor* ASiltarnPlayerController::DropItemAsBag(UClass* p_BagClass)
{
	if (p_BagClass)
	{
		ASiltarnCharacter* _Character = Cast<ASiltarnCharacter>(GetCharacter());

		if (_Character)
		{
			return _Character->DropItemAsBag(p_BagClass);
		}
	}

	return nullptr;
}



void ASiltarnPlayerController::OpenEscapeMenu()
{
	if (m_GameplayHUD)
	{
		m_GameplayHUD->OpenEscapeMenu();
	}
	else
	{
		UE_LOG(LogClass_AGameplayHUD, Error, TEXT("EchapKeyPressed() : m_GameplayHUD is NULL !"));
	}
}



void ASiltarnPlayerController::OpenCharacterProfileWidget()
{
	if (m_GameplayHUD)
	{
		m_GameplayHUD->OpenCharacterProfileWidget();
	}
	else
	{
		UE_LOG(LogClass_AGameplayHUD, Error, TEXT("OpenCharacterProfileWidget() : m_GameplayHUD is NULL !"));
	}
}