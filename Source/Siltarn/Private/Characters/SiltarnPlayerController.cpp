#include "Characters/SiltarnPlayerController.h"
#include "Siltarn/Public/HUDs/GameplayHUD.h"
#include "Siltarn/Public/Interactables/InteractableEntity.h"
#include "Siltarn/Public/Inventory/Inventory.h"
#include "Siltarn/Public/Characters/SiltarnCharacter.h"

DEFINE_LOG_CATEGORY(LogClass_SiltarnPlayerController);

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
			break;

		case EWidgetVisibilityState::COLLAPSED:
			SetInputMode(FInputModeGameOnly());
			SetShowMouseCursor(false);
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
	if (m_GameplayHUD)
	{
		return m_GameplayHUD->DOES_InventoryHasRoomForItem(p_ItemSize);
	}

	return false;
}

void ASiltarnPlayerController::ADD_ItemToInventory(UPickupEntity* p_Item)
{
	if (m_Inventory && m_GameplayHUD)
	{
		bool _bWasAddingItemSuccessful = m_GameplayHUD->ADD_ItemToInventory(p_Item);

		if (_bWasAddingItemSuccessful)
		{
			m_Inventory->ADD_Item(p_Item);
		}		
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
	check(_Character);

	// Physically dropping the item
	_Character->DROP_Item(p_Item);

	// 
	m_Inventory->REMOVE_Item(p_Item);
}