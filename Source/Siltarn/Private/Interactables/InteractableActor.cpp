#include "Interactables/InteractableActor.h"
#include "Siltarn/Public/Characters/SiltarnCharacter.h"
#include "Siltarn/Public/Characters/SiltarnPlayerController.h"
#include "Siltarn/Public/Interactables/InteractableEntity.h"

DEFINE_LOG_CATEGORY(LogClass_AInteractableActor);

int32 AInteractableActor::m_InstanceCount = 0;

AInteractableActor::AInteractableActor()
{
	// Var initialization
	//
	PrimaryActorTick.bCanEverTick = false;
	this->bReplicates = true;

	m_Name            = "None"          ;
	m_bIsInteractible = true            ;


	
	// Components initialization
	//
	m_RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(m_RootComponent);
	
	m_StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh")) ;
	m_StaticMesh->SetupAttachment(RootComponent)                                     ;
	m_StaticMesh->SetSimulatePhysics(true)                                           ; // These lines' purpose
	m_StaticMesh->SetEnableGravity(true)                                             ; // is to enable the player to 
	//m_StaticMesh->SetNotifyRigidBodyCollision(true)                                  ; // drop the item in a realistic way.
	// The mass is related to the impulse


	m_InstanceCount++;
	UE_LOG(LogClass_AInteractableActor, Log, TEXT("A new instance was created ! || Instance count : %d"), m_InstanceCount);
}

AInteractableActor::~AInteractableActor()
{
	m_InstanceCount--;
	UE_LOG(LogClass_AInteractableActor, Log, TEXT("An instance was destroyed ! || Instance count : %d"), m_InstanceCount);
}

void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();

	m_StaticMesh->SetMassOverrideInKg(FName("Static Mesh"), 1000.0f);
}

void AInteractableActor::INTERFACE_Interact(ASiltarnPlayerController* p_SiltarnController)
{
	/*checkf(p_SiltarnController != nullptr, TEXT("AInteractableActor::INTERFACE_Interact() : p_SiltarnController is NULL !"));

	if (m_bIsInteractible)
	{
		bool _bEnoughRoomForItem = p_SiltarnController->DOES_InventoryHasRoomForItem(m_InventorySpace);

		if (_bEnoughRoomForItem)
		{
			UInteractableEntity* _NewEntity = NewObject<UInteractableEntity>();
			_NewEntity->Initialize_Entity(this);
			p_SiltarnController->ADD_ItemToInventory(_NewEntity);

			Destroy();
		}
	}*/
}

void AInteractableActor::INTERFACE_OnStartFocus(ASiltarnPlayerController* p_SiltarnController)
{
	if (m_bIsInteractible)
	{
		p_SiltarnController->DISPLAY_InteractableEntityTag(m_Name);
	}
}

void AInteractableActor::INTERFACE_OnEndFocus(ASiltarnPlayerController* p_SiltarnController)
{
	if (m_bIsInteractible)
	{
		p_SiltarnController->SHUTDOWN_PickupItemWidget();
	}
}

void AInteractableActor::ADD_Impulse(const FVector& p_Impulse) const
{
	checkf(m_StaticMesh != nullptr, TEXT("AInteractableActor::ADD_Impulse() : m_StaticMesh is NULL !"));

	m_StaticMesh->AddImpulse(p_Impulse);
}