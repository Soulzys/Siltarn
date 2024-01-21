#include "Interactables/InteractableEntity.h"
#include "Siltarn/Public/Interactables/InteractableActor.h"
#include "Siltarn/Public/Inventory/Inventory.h"

DEFINE_LOG_CATEGORY(LogClass_UInteractableEntity);

uint32 UInteractableEntity::m_InstanceCount = 0;

UInteractableEntity::UInteractableEntity()
{
	// Variable initialization
	//
	m_Name= "";

	m_InstanceCount++;
	UE_LOG(LogClass_UInteractableEntity, Log, TEXT("A new instance was created ! || Instance count : %d"), m_InstanceCount);
}

UInteractableEntity::~UInteractableEntity()
{
	m_InstanceCount--;
	UE_LOG(LogClass_UInteractableEntity, Log, TEXT("An instance was destroyed ! || Instance count : %d"), m_InstanceCount);
}

void UInteractableEntity::Initialize_Entity(AInteractableActor* p_IA)
{
	check(p_IA != nullptr);

	m_Name= p_IA->GET_Name();
}



void UInteractableEntity::DEBUG_DisplayDataThroughUELogs()
{
	UE_LOG(LogClass_UInteractableEntity, Log, TEXT("********************")         );
	UE_LOG(LogClass_UInteractableEntity, Log, TEXT("Name            : %s"), *m_Name);
	UE_LOG(LogClass_UInteractableEntity, Log, TEXT("********************")         );
}