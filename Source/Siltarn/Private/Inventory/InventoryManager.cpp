#include "Inventory/InventoryManager.h"
#include "Siltarn/Public/Slate/Widgets/SPlayerInventoryWidget.h"
#include "Siltarn/Public/Characters/SiltarnPlayerController.h"
#include "Siltarn/Public/Interactables/PickupEntity.h"
#include "Siltarn/Public/Interactables/ItemBagActor.h"
#include "Siltarn/Public/Interactables/PickupActor.h"

DEFINE_LOG_CATEGORY(LogClass_UInventoryManager);

UInventoryManager::UInventoryManager()
{
	const ConstructorHelpers::FClassFinder<AItemBagActor>_ItemBagActorBPClass(TEXT("/Game/Blueprints/Interactables/BP_ItemBagActor"));

	// Check if this works 
	m_BagClass = _ItemBagActorBPClass.Class;
}

void UInventoryManager::SET_InventoryWidget(TSharedPtr<SPlayerInventoryWidget> p_InventoryWidget)
{
	if (!p_InventoryWidget.IsValid())
	{
		UE_LOG(LogClass_UInventoryManager, Error, TEXT("SET_InventoryWidget() : p_InventoryWidget is NULL !"));
		return;
	}

	m_PlayerInventoryWidget = p_InventoryWidget;

	// Luciole 15/03/2024 || This should not be here. This function should be called from elsewhere but I have a serious headache right now and I can't think 
	m_PlayerInventoryWidget->SET_InventoryManager(this);
}



void UInventoryManager::SET_SiltarnPlayerController(ASiltarnPlayerController* p_Controller)
{
	if (!p_Controller)
	{
		UE_LOG(LogClass_UInventoryManager, Error, TEXT("SET_SiltarnPlayerController() : p_Controller is NULL !"));
		return;
	}

	m_SiltarnController = p_Controller;
}



void UInventoryManager::AddItemToPlayerInventory(UPickupEntity* p_ItemEntity)
{
	if (p_ItemEntity && m_PlayerInventoryWidget)
	{
		//bool _bWasAddingItemWidgetSuccessful = m_PlayerInventoryWidget->AddItemToInventory(p_ItemEntity);
		bool _bWasAddingItemWidgetSuccessful = m_PlayerInventoryWidget->AddItemToInventoryNew(p_ItemEntity);

		if (_bWasAddingItemWidgetSuccessful)
		{
			m_ItemEntities.Emplace(p_ItemEntity);
		}
	}
}



bool UInventoryManager::DoesPlayerInventoryHasRoomForNewItem(const FIntPoint& p_ItemSize)
{
	if (m_PlayerInventoryWidget.IsValid())
	{
		return m_PlayerInventoryWidget->IsThereRoomInInventory(p_ItemSize);
	}

	return false;
}

void UInventoryManager::DropItem(UPickupEntity* p_ItemEntity)
{
	// Need to find a way to either initialize the dropped actor with p_ItemEntity if it is dropped as is,
	// or load the bag if p_ItemEntity was not droppable as is

	if (p_ItemEntity)
	{
		if (p_ItemEntity->IS_DroppableAsIs())
		{
			DropItemAsItIs(p_ItemEntity);
		}
		else
		{
			DropItemAsBag(m_BagClass, p_ItemEntity);
		}
	}
}

void UInventoryManager::DropItemAsItIs(UPickupEntity* p_ItemEntity)
{
	if (m_SiltarnController && m_PlayerInventoryWidget.IsValid() && p_ItemEntity)
	{
		APickupActor* _ItemActor = m_SiltarnController->DropItemAsItIs(p_ItemEntity);

		if (_ItemActor)
		{
			bool _bWasItemWidgetRemoved = m_PlayerInventoryWidget->RemoveItemCanvasSlot(p_ItemEntity->GET_EntityId());

			if (_bWasItemWidgetRemoved)
			{
				m_ItemEntities.Remove(p_ItemEntity);
				_ItemActor->InitializeActorWithEntity(p_ItemEntity);

				/* Luciole 13/03/2024
				   This takes some time to destroy the UObject, but I don't think we need it to be destroyed immediately so it's fine for now
				   We use the Conditional version because calling "BeginDestroy" ends up in a crash for some reason, and I'm too lazy to investigate atm.

				   /!\ After thinking, this may later cause an issue as if the object is picked up by the player right after it was dropped, therefore before
				   the UPickupEntity UObject was deleted, there will be an overlap. This may in turn result in the TArray where we stock all inventory items reference
				   to blow up in size over time. Consequently, the UPickupEntity UObject should ideally be destroyed immediately. /!\

				   26/03/2024 -> check if after being set for BeginDestroy, the UObject is still accessible or if it is automatically set to NULL. 
				   If that is the case, then we don't need to care so much about it.
				*/
				p_ItemEntity->ConditionalBeginDestroy(); 
			}
		}		
	}
}

void UInventoryManager::DropItemAsBag(UClass* p_BagClass, UPickupEntity* p_ItemEntity)
{
	if (m_SiltarnController && m_PlayerInventoryWidget.IsValid() && p_BagClass)
	{
		AItemBagActor* _ItemBag = m_SiltarnController->DropItemAsBag(p_BagClass);

		if (_ItemBag)
		{
			bool _bWasItemWidgetRemoved = m_PlayerInventoryWidget->RemoveItemCanvasSlot(p_ItemEntity->GET_EntityId());

			if (_bWasItemWidgetRemoved)
			{
				bool _bWasBagSuccesfullyLoaded = _ItemBag->LoadBag(p_ItemEntity);

				if (_bWasBagSuccesfullyLoaded)
				{
					m_ItemEntities.Remove(p_ItemEntity);
				}
			}
		}		
	}
}

void UInventoryManager::DropItems()
{
	if (m_SiltarnController && m_PlayerInventoryWidget.IsValid())
	{
		AItemBagActor* _ItemBag = m_SiltarnController->DropItemAsBag(m_BagClass); // Luciole 15/03/2024 dirty temporary fix. See .h for what to do

		if (_ItemBag)
		{
			// bool _bWereItemWidgetsRemoved = m_PlayerInventoryWidget->RemoveItemsCanvasSlot();old
			bool _bWereItemWidgetsRemoved = m_PlayerInventoryWidget->RemoveItemsCanvasSlotNew();

			if (_bWereItemWidgetsRemoved)
			{
				bool _bWereItemsWereLoaded = _ItemBag->LoadBag(m_ItemsToDrop);

				if (_bWereItemsWereLoaded)
				{
					// Luciole 30/03/2024 || Too tired to come up with anything else. There's probably plenty of room for improvement 
					for (int32 i = 0; i < m_ItemsToDrop.Num(); i++)
					{
						for (int32 j = 0; j < m_ItemEntities.Num(); j++)
						{
							if (*m_ItemsToDrop[i] == *m_ItemEntities[j])
							{
								m_ItemEntities.RemoveAt(j);
								break;
							}
						}
					}

					m_ItemsToDrop.Empty();

					DEBUG_DisplayAllItemsName();
				}
			}
		}
	}
}



void UInventoryManager::SetItemForGroupDrop(UPickupEntity* p_ItemEntity)
{
	if (p_ItemEntity)
	{
		m_ItemsToDrop.Emplace(p_ItemEntity);
	}
}



bool UInventoryManager::MoveItemToPlayerInventory(UPickupEntity* p_ItemEntity)
{
	if (m_PlayerInventoryWidget && p_ItemEntity)
	{
		AddItemToPlayerInventory(p_ItemEntity);
		return true;
	}

	return false;
}



void UInventoryManager::DEBUG_DisplayAllItemsName() const
{
	UE_LOG(LogClass_UInventoryManager, Warning, TEXT("DEBUG_DisplayAllItemsName() : START -----"));

	for (int32 i = 0; i < m_ItemEntities.Num(); i++)
	{
		UE_LOG(LogClass_UInventoryManager, Log, TEXT("m_ItemEntities[%d] : %s"), i, *m_ItemEntities[i]->GET_Name());
	}

	UE_LOG(LogClass_UInventoryManager, Warning, TEXT("DEBUG_DisplayAllItemsName() : FINISH -----"));
}
