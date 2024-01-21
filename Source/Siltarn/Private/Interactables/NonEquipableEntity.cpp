#include "Interactables/NonEquipableEntity.h"
#include "Siltarn/Public/Interactables/NonEquipableActor.h"

DEFINE_LOG_CATEGORY(LogClass_UNonEquipableEntity);

UNonEquipableEntity::UNonEquipableEntity()
{

}

UNonEquipableEntity::~UNonEquipableEntity()
{

}

void UNonEquipableEntity::Initialize_Entity(ANonEquipableActor* p_IA)
{
	m_Name           = p_IA->GET_Name()           ;
	m_Icon           = p_IA->GET_Icon()           ;
	m_InventorySpace = p_IA->GET_InventorySpace() ;
	m_ActorClass     = p_IA->GET_ItemClass()      ;
	m_DamageBonus    = p_IA->GET_DamageBonus()    ;
	m_ResellValue    = p_IA->GET_ResellValue()    ;
	m_Description    = p_IA->GET_Description()    ;
	m_Rarety         = p_IA->GET_Rarety()         ;
}

EPickupEntityType UNonEquipableEntity::GET_ItemType() const
{
	return EPickupEntityType::Ammunition;
}