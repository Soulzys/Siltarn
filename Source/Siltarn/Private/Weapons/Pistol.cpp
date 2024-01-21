#include "Weapons/Pistol.h"
#include "Siltarn/Public/Characters/SiltarnCharacter.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY(LogClass_Pistol);

APistol::APistol()
{
	UE_LOG(LogClass_Pistol, Log, TEXT("Constructor called !"));

	PrimaryActorTick.bCanEverTick = false;
	m_DistanceFromCamera = 5.0f;

	m_RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(m_RootComponent);

	m_PistolMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Pistol Mesh"));
	m_PistolMesh->SetupAttachment(RootComponent);
}

APistol::~APistol()
{
	UE_LOG(LogClass_Pistol, Log, TEXT("Destructor called !"));
}

void APistol::BeginPlay()
{
	Super::BeginPlay();	
}

void APistol::CALCULATE_SightTransform()
{
	if (m_PistolOwner == nullptr)
	{
		UE_LOG(LogClass_Pistol, Warning, TEXT("CALCULATE_SightTransform() : m_PistolOwner is NULL. || Unable to proceed further."));
		return;
	}

	if (m_PistolMesh == nullptr)
	{
		UE_LOG(LogClass_Pistol, Warning, TEXT("CALCULATE_SightTransform() : m_PistolMesh is NULL. || Unable to proceed further"));
		return;
	}

	if (m_PistolMesh->SkeletalMesh == nullptr)
	{
		UE_LOG(LogClass_Pistol, Warning, TEXT("CALCULATE_SightTransform() : m_PistolMesh->SkeletalMesh is NULL. || Unable to proceed further"));
		return;
	}

	FTransform _OpticAimpointTransform = m_PistolMesh->GetSocketTransform(FName("OpticAimpoint"), ERelativeTransformSpace::RTS_World);
	FTransform _RightHandTransform = m_PistolOwner->GET_CharacterMesh()->GetSocketTransform(FName("hand_r"), ERelativeTransformSpace::RTS_World);

	FTransform _OpticAimpointRelativeToRightHand = UKismetMathLibrary::MakeRelativeTransform(_OpticAimpointTransform, _RightHandTransform);
	FTransform _RightHandRelativeToOpticAimpoint = UKismetMathLibrary::MakeRelativeTransform(_RightHandTransform, _OpticAimpointTransform);

	float _RightHandRelativeToOpticAimpointX = _RightHandRelativeToOpticAimpoint.GetLocation().X;
	_RightHandRelativeToOpticAimpointX *= -1.0f;
	_RightHandRelativeToOpticAimpointX += m_DistanceFromCamera;

	m_SightTransform = FTransform();
	m_SightTransform.SetLocation(_OpticAimpointRelativeToRightHand.GetLocation());
	m_SightTransform.SetRotation(_OpticAimpointRelativeToRightHand.GetRotation());
	m_SightTransform.SetScale3D(FVector(_RightHandRelativeToOpticAimpointX, 1.0f, 1.0f));

}

void APistol::SET_Owner(ASiltarnCharacter* p_Owner)
{
	if (p_Owner == nullptr)
	{
		UE_LOG(LogClass_Pistol, Warning, TEXT("SET_Owner() : SecuCheck -->> p_Owner is NULL. || Unable to proceed further."));
		return;
	}

	m_PistolOwner = p_Owner;
}