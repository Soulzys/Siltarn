#include "Characters/SiltarnCharacterAnimInstance.h"
#include "Siltarn/Public/Characters/SiltarnCharacter.h"
#include "Siltarn/Public/Weapons/Rifle.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY(LogClass_SiltarnCharacterAnimInstance);

USiltarnCharacterAnimInstance::USiltarnCharacterAnimInstance()
{
	m_bIsAiming = false;
}

void USiltarnCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	m_SiltarnCharacter = Cast<ASiltarnCharacter>(TryGetPawnOwner());

	if (m_SiltarnCharacter)
	{
		FTimerHandle _SetAimSightRelativeToRightHandTimer;

		m_AimingMovementDuration = m_SiltarnCharacter->GET_AimingMovementDuration();
	
		GET_CameraPosition();
	}	
}

void USiltarnCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	SETUPDATE_PitchPerBone();

	if (m_SiltarnCharacter)
	{
		m_bIsAiming = m_SiltarnCharacter->GET_bIsAiming();
	}
}

void USiltarnCharacterAnimInstance::SETUPDATE_PitchPerBone()
{
	if (m_SiltarnCharacter)
	{
		FRotator _CharacterRotation         = m_SiltarnCharacter->GetControlRotation();
		FRotator _CharacterRotationCombined = UKismetMathLibrary::ComposeRotators(_CharacterRotation, FRotator(0.0f, 180.0f, 0.0f));
		float _PitchValue                   = _CharacterRotationCombined.Pitch;
		_PitchValue                        /= 6.0f; // I believe it is because the back bones are composed of four bones, although this needs to be checked
		m_PitchPerBone.Roll                 = - _PitchValue;
	}
}

void USiltarnCharacterAnimInstance::NEW_SightTransform(const FTransform& p_NewRelativeToHand)
{
	m_LastSight = p_NewRelativeToHand;

	SET_IKTransforms();
}

void USiltarnCharacterAnimInstance::SET_IKTransforms()
{
	m_RelativeToHandLocation        = m_LastSight.GetLocation()           ;
	m_RelativeToHandRotation        = m_LastSight.GetRotation().Rotator() ;
	m_DistanceFromCameraCompensated = m_LastSight.GetScale3D ().X         ; // Why not simply using a float variable ? For perf purposes I suppose ? 

	//UE_LOG(LogClass_SiltarnCharacterAnimInstance, Warning, TEXT("m_RelativeToHandLocation : %s"), *m_LastSight.GetLocation().ToString());

	m_SightLocation = FVector(m_CamSocketLocation.X, m_CamSocketLocation.Y + m_DistanceFromCameraCompensated, m_CamSocketLocation.Z);
}

void USiltarnCharacterAnimInstance::GET_CameraPosition()
{
	m_CamSocketLocation = GetOwningComponent()->GetSocketTransform(FName("AimingCamera"), ERelativeTransformSpace::RTS_ParentBoneSpace).GetLocation()           ;
	m_CamSocketRotation = GetOwningComponent()->GetSocketTransform(FName("AimingCamera"), ERelativeTransformSpace::RTS_ParentBoneSpace).GetRotation().Rotator() ;
}