#include "Interactables/EquipableActor.h"
#include "Siltarn/Public/Characters/SiltarnPlayerController.h"
#include "Siltarn/Public/Interactables/EquipableEntity.h"

DEFINE_LOG_CATEGORY(LogClass_AEquipableActor);

AEquipableActor::AEquipableActor()
{
	
}

AEquipableActor::~AEquipableActor()
{

}

void AEquipableActor::BeginPlay()
{
	Super::BeginPlay();

	/*
		Luciole || Need to document this trick, or perhaps try to find a better, more sustainable solution
	*/
	if (!m_bIsPlacedInLevel)
	{
		m_StaticMesh->AddImpulse(GetActorForwardVector() * 250000.0f);
	}
}

void AEquipableActor::INTERFACE_Interact(ASiltarnPlayerController* p_SiltarnController)
{
	checkf(p_SiltarnController != nullptr, TEXT("AEquipableActor::INTERFACE_Interact() : p_SiltarnController is NULL !"));

	if (m_bIsInteractible)
	{
		bool _bEnoughRoomForItem = p_SiltarnController->DOES_InventoryHasRoomForItem(m_InventorySpace);

		if (_bEnoughRoomForItem)
		{
			UEquipableEntity* _NewEntity = NewObject<UEquipableEntity>();
			_NewEntity->Initialize_Entity(this);
			p_SiltarnController->ADD_ItemToInventory(_NewEntity, this);

			//Destroy();
		}		
	}
}
