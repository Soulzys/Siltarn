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
	UE_LOG(LogClass_APickupActor, Warning, TEXT("WTF ???"));

	checkf(p_SiltarnController != nullptr, TEXT("APickupActor::INTERFACE_Interact() : p_SiltarnController is NULL !"));

	if (m_bIsInteractible)
	{
		bool _bEnoughRoomForItem = p_SiltarnController->DOES_InventoryHasRoomForItem(m_InventorySpace);

		if (_bEnoughRoomForItem)
		{
			UPickupEntity* _NewEntity = NewObject<UPickupEntity>();
			UE_LOG(LogClass_APickupActor, Warning, TEXT("WTF ???"));
			_NewEntity->Initialize_Entity(this);
			p_SiltarnController->ADD_ItemToInventory(_NewEntity);

			Destroy();
		}
	}	
}

bool APickupActor::InitializeActorWithEntity(UPickupEntity* p_ItemEntity)
{
	if (p_ItemEntity)
	{
		m_Icon              = p_ItemEntity->GET_Icon()              ;
		m_InventorySpace    = p_ItemEntity->GET_InventorySpace()    ;
		m_ResellValue       = p_ItemEntity->GET_ResellValue()       ;
		m_Description       = p_ItemEntity->GET_Description()       ;
		m_ItemClass         = p_ItemEntity->GET_ActorClass()        ;
		m_Rarety            = p_ItemEntity->GET_Rarety()            ;
		m_DroppableBagClass = p_ItemEntity->GET_DroppableBagClass() ;
		m_bIsEquipable      = p_ItemEntity->IS_Equipable()          ;
		m_bIsDroppableAsIs  = p_ItemEntity->IS_DroppableAsIs()      ;
		m_bIsPlacedInLevel  = true                                  ;

		return true;
	}

	return false;
}