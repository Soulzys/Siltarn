#include "Interactables/PickupActor.h"
#include "Siltarn/Public/Characters/SiltarnPlayerController.h"
#include "Siltarn/Public/Interactables/PickupEntity.h"

DEFINE_LOG_CATEGORY(LogClass_APickupActor);

APickupActor::APickupActor()
{
	m_bIsPlacedInLevel = false;
	m_bIsEquipable = false;
}

APickupActor::~APickupActor()
{

}

void APickupActor::INTERFACE_Interact(ASiltarnPlayerController* p_SiltarnController)
{
	checkf(p_SiltarnController != nullptr, TEXT("APickupActor::INTERFACE_Interact() : p_SiltarnController is NULL !"));

	if (m_bIsInteractible)
	{
		bool _bEnoughRoomForItem = p_SiltarnController->DOES_InventoryHasRoomForItem(m_InventorySpace);

		if (_bEnoughRoomForItem)
		{
			UPickupEntity* _NewEntity = NewObject<UPickupEntity>();
			_NewEntity->Initialize_Entity(this);
			p_SiltarnController->ADD_ItemToInventory(_NewEntity);

			Destroy();
		}
	}	
}