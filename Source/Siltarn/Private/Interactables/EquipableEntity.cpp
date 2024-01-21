#include "Interactables/EquipableEntity.h"
#include "Siltarn/Public/Interactables/EquipableActor.h"

UEquipableEntity::UEquipableEntity()
{

}

UEquipableEntity::~UEquipableEntity()
{

}

void UEquipableEntity::Initialize_Entity(AEquipableActor* p_IA)
{
	m_Name             = p_IA->GET_Name()             ;
	m_Icon             = p_IA->GET_Icon()             ;
	m_InventorySpace   = p_IA->GET_InventorySpace()   ;
	m_ActorClass       = p_IA->GET_ItemClass()        ;
	m_ResellValue      = p_IA->GET_ResellValue()      ;
	m_Description      = p_IA->GET_Description()      ;
	m_Damage           = p_IA->GET_Damage()           ;
	m_MagazineCapacity = p_IA->GET_MagazineCapacity() ;
	m_RateOfFire       = p_IA->GET_RateOfFire()       ;
	m_Rarety           = p_IA->GET_Rarety()           ;
}

EPickupEntityType UEquipableEntity::GET_ItemType() const
{
	return EPickupEntityType::Pistol;
}