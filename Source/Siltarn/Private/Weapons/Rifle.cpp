#include "Weapons/Rifle.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Siltarn/Public/Characters/SiltarnCharacter.h"
#include "Siltarn/Public/Components/FOVSkeletalMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY(LogClass_Rifle);

int32 ARifle::m_InstanceCount = 0;

ARifle::ARifle()
{
	UE_LOG(LogClass_Rifle, Log, TEXT("Constructor called !"));
	m_InstanceCount++;
	UE_LOG(LogClass_Rifle, Log, TEXT("Number of instances : %d"), m_InstanceCount);

	PrimaryActorTick.bCanEverTick = false;

	// Variables initialization
	m_DistanceFromCamera = 5.0f;

	m_RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(m_RootComponent);

	m_RifleMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Rifle Mesh"));
	m_RifleMesh->SetupAttachment(RootComponent);

	m_OpticSightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Optic Sight Mesh"));
	m_OpticSightMesh->SetupAttachment(m_RifleMesh);	

	m_IronSightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Iron Sight Mesh"));
	m_IronSightMesh->SetupAttachment(m_RifleMesh);
}

ARifle::~ARifle()
{
	UE_LOG(LogClass_Rifle, Log, TEXT("Destructor called !"));
	m_InstanceCount--;
	UE_LOG(LogClass_Rifle, Log, TEXT("Number of instances : %d"), m_InstanceCount);
}

void ARifle::BeginPlay()
{
	Super::BeginPlay();

	if (m_OpticSightMesh && m_OpticSightMesh->GetStaticMesh())
	{
		m_OpticSightMeshSocket = m_RifleMesh->GetSocketByName(FName("Aiming_OpticSightSocket"));
		
		if (m_OpticSightMeshSocket)
		{
			m_OpticSightMesh->SetRelativeLocation(m_OpticSightMeshSocket->RelativeLocation);
		}
	}

	if (m_IronSightMesh && m_IronSightMesh->GetStaticMesh())
	{
		m_IronSightMeshSocket = m_RifleMesh->GetSocketByName(FName("Aiming_IronSightSocket"));

		if (m_IronSightMeshSocket)
		{
			m_IronSightMesh->SetRelativeLocation(m_IronSightMeshSocket->RelativeLocation);
		}
	}
}

void ARifle::CALCULATE_SightTransform()
{
	if (m_RifleOwner)
	{
		if (m_RifleMesh->SkeletalMesh)
		{
			FTransform _OpticAimpointTransform = m_RifleMesh->GetSocketTransform(FName("OpticAimpoint"), ERelativeTransformSpace::RTS_World);
			FTransform _RightHandTransform = m_RifleOwner->GET_CharacterMesh()->GetSocketTransform(FName("hand_r"), ERelativeTransformSpace::RTS_World);

			FTransform _OpticAimpointRelativeToRightHand = UKismetMathLibrary::MakeRelativeTransform(_OpticAimpointTransform, _RightHandTransform);
			FTransform _RightHandRelativeToOpticAimpoint = UKismetMathLibrary::MakeRelativeTransform(_RightHandTransform, _OpticAimpointTransform);

			// Do we actually need these 3 lines ? 
			float _RightHandRelativeToOpticAimpointX = _RightHandRelativeToOpticAimpoint.GetLocation().X;
			_RightHandRelativeToOpticAimpointX *= -1.0f;
			_RightHandRelativeToOpticAimpointX += m_DistanceFromCamera;

			m_SightTransform = FTransform();
			m_SightTransform.SetLocation(_OpticAimpointRelativeToRightHand.GetLocation());
			m_SightTransform.SetRotation(_OpticAimpointRelativeToRightHand.GetRotation());
			m_SightTransform.SetScale3D(FVector(_RightHandRelativeToOpticAimpointX, 1.0f, 1.0f)); // Do we really need to pass it as the Scale3D ? 
		}
	}
}

void ARifle::SET_Owner(ASiltarnCharacter* p_Owner)
{
	m_RifleOwner = p_Owner;
}