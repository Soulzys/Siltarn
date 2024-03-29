#include "Characters/SiltarnPlayerController.h"
#include "Siltarn/Public/HUDs/GameplayHUD.h"
#include "Siltarn/Public/Interactables/InteractableEntity.h"
#include "Siltarn/Public/Interactables/PickupActor.h"
#include "Siltarn/Public/Inventory/Inventory.h"
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

	CREATE_Inventory();
}

void ASiltarnPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Careful ! This implementation only deals with AGameplayHUD, as if the game started in the "Pyralinth". 
	// However, the game is eventually going to be starting from the "base camp" world, which should have another HUD.
	m_GameplayHUD = Cast<AGameplayHUD>(GetHUD());

	//CREATE_Inventory();	


	// Inventory
	m_InventoryManager = NewObject<UInventoryManager>();
	m_InventoryManager->SET_InventoryWidget(m_GameplayHUD->GET_InGamePlayerInventoryWidget());
	m_InventoryManager->SET_SiltarnPlayerController(this);

	/*
		We give SProfileMenu a reference to UInventoryManager in order to manage the dropping of items.
	*/
	m_GameplayHUD->SET_InventoryManager(m_InventoryManager);
}

// Should check if this has already been created as this function should be called once and ONLY once 
void ASiltarnPlayerController::CREATE_Inventory()
{
	if (m_Inventory != nullptr)
	{
		UE_LOG(LogClass_SiltarnPlayerController, Warning, TEXT("CREATE_Inventory() : Is appears that an inventory already exists. || Unable to proceed further."));
		return;
	}

	m_Inventory = NewObject<UInventory>();

	if (m_Inventory == nullptr)
	{
		UE_LOG(LogClass_SiltarnPlayerController, Warning, TEXT("CREATE_Inventory() : Could not manage to create a UInventory instance. m_Inventory is therefore still NULL. || Unable to proceed further."));
		return;
	}

	m_Inventory->SET_SiltarnPlayerController(this);
}

void ASiltarnPlayerController::TOGGLE_Inventory()
{
	if (m_GameplayHUD == nullptr)
	{
		UE_LOG(LogClass_SiltarnPlayerController, Warning, TEXT("OPEN_Inventory() : m_GameplayHUD is NULL. || Unable to proceed further."));
		return;
	}

	EWidgetVisibilityState _VisibilityState = m_GameplayHUD->TOGGLE_InventoryWidget();

	switch (_VisibilityState)
	{
		case EWidgetVisibilityState::VISIBLE:
			SetInputMode(FInputModeGameAndUI());
			SetShowMouseCursor(true);
			m_bIsInventoryOpen = true;
			break;

		case EWidgetVisibilityState::COLLAPSED:
			SetInputMode(FInputModeGameOnly());
			SetShowMouseCursor(false);
			m_bIsInventoryOpen = false;
			break;

		case EWidgetVisibilityState::BUG:
			UE_LOG(LogClass_SiltarnPlayerController, Warning, TEXT("OPEN_Inventory() : _VisibilityState returned with BUG. || Unable to proceed further."));
			return;

		default:
			break;
	}
}

void ASiltarnPlayerController::TOGGLE_EchapMenu()
{
	EWidgetVisibilityState _VisibilityState = m_GameplayHUD->TOGGLE_EchapMenu();

	switch (_VisibilityState)
	{
		case EWidgetVisibilityState::VISIBLE:
			SetInputMode(FInputModeUIOnly());
			SetShowMouseCursor(true);
			break;

		case EWidgetVisibilityState::COLLAPSED:
			SetInputMode(FInputModeGameOnly());
			SetShowMouseCursor(false);
			break;

		case EWidgetVisibilityState::BUG:
			return;

		default:
			break;
	}
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
	// Old
	/*if (m_GameplayHUD)
	{
		return m_GameplayHUD->DOES_InventoryHasRoomForItem(p_ItemSize);
	}*/

	// New (same result but more direct way)
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

void ASiltarnPlayerController::DROP_Item(UPickupEntity* p_Item)
{
	ASiltarnCharacter* _Character = Cast<ASiltarnCharacter>(GetCharacter());
	
	if (_Character)
	{
		_Character->DROP_Item(p_Item);
		m_Inventory->REMOVE_Item(p_Item);
	}
}

/*void ASiltarnPlayerController::DROP_Items(TArray<UPickupEntity*>& p_Items)
{
	ASiltarnCharacter* _Character = Cast<ASiltarnCharacter>(GetCharacter());

	if (_Character)
	{
		_Character->DROP_Items(p_Items);
	}
}*/

void ASiltarnPlayerController::OPEN_ItemBag(const FIntPoint& p_InventorySize, TArray<UPickupEntity*>& p_Items)
{
	if (m_GameplayHUD)
	{
		m_GameplayHUD->OPEN_ItemBag(p_InventorySize, p_Items);
	}
}

bool ASiltarnPlayerController::DropItem(UPickupEntity* p_ItemEntity, AItemBagActor* p_DroppingBag, APickupActor* p_ItemActor)
{
	if (p_ItemEntity)
	{
		ASiltarnCharacter* _Character = Cast<ASiltarnCharacter>(GetCharacter());

		if (_Character)
		{
			return _Character->DropItem(p_ItemEntity, p_DroppingBag, p_ItemActor);
		}
	}

	return false;
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



bool ASiltarnPlayerController::DropItems(TArray<UPickupEntity*>& p_Items)
{
	ASiltarnCharacter* _Character = Cast<ASiltarnCharacter>(GetCharacter());

	if (_Character)
	{
		return _Character->DropItems(p_Items);
	}

	return false;
}