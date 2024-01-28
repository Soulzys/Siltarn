#include "Interactables/NonEquipableActor.h"
#include "Siltarn/Public/Characters/SiltarnPlayerController.h"
#include "Siltarn/Public/Interactables/NonEquipableEntity.h"

DEFINE_LOG_CATEGORY(LogClass_ANonEquipableActor);

ANonEquipableActor::ANonEquipableActor()
{
	
}

ANonEquipableActor::~ANonEquipableActor()
{

}

void ANonEquipableActor::BeginPlay()
{
	Super::BeginPlay();

	if (!m_bIsPlacedInLevel)
	{
		m_StaticMesh->AddImpulse(GetActorForwardVector() * 250000.0f);
	}
}

void ANonEquipableActor::INTERFACE_Interact(ASiltarnPlayerController* p_SiltarnController)
{
	checkf(p_SiltarnController != nullptr, TEXT("ANonEquipableActor::INTERFACE_Interact() : p_SiltarnController is NULL !"));

	if (m_bIsInteractible)
	{
		bool _bEnoughRoomForItem = p_SiltarnController->DOES_InventoryHasRoomForItem(m_InventorySpace);

		if (_bEnoughRoomForItem)
		{
			UNonEquipableEntity* _NewEntity = NewObject<UNonEquipableEntity>();
			_NewEntity->Initialize_Entity(this);
			p_SiltarnController->ADD_ItemToInventory(_NewEntity, this);

			//Destroy();
		}
	}
}
