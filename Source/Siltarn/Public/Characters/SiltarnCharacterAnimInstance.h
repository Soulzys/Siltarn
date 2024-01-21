#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SiltarnCharacterAnimInstance.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogClass_SiltarnCharacterAnimInstance, Log, All);

class ASiltarnCharacter;

UCLASS()
class SILTARN_API USiltarnCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	USiltarnCharacterAnimInstance();

	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void NEW_SightTransform(const FTransform& p_NewRelativeToHand);
	void ACTIVATE_Aiming(bool p_bIsAiming);

protected:

	void SET_IKTransforms();
	void GET_CameraPosition();
	void SETUPDATE_PitchPerBone();

protected:

	UPROPERTY(BlueprintReadOnly, meta = (DisplayName = "CODE_SiltarnCharacter"))
	ASiltarnCharacter* m_SiltarnCharacter = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (DisplayName = "CODE_LastSight"))
	FTransform m_LastSight;

	UPROPERTY(BlueprintReadOnly, meta = (DisplayName = "CODE_RelativeTohandLocation"))
	FVector m_RelativeToHandLocation;

	UPROPERTY(BlueprintReadOnly, meta = (DisplayName = "CODE_RelativeTohandRotation"))
	FRotator m_RelativeToHandRotation;

	UPROPERTY(BlueprintReadOnly, meta = (DisplayName = "CODE_DistanceFromCameraCompensated"))
	float m_DistanceFromCameraCompensated;

	UPROPERTY(BlueprintReadOnly, meta = (DisplayName = "CODE_CamSocketLocation"))
	FVector m_CamSocketLocation;

	UPROPERTY(BlueprintReadOnly, meta = (DisplayName = "CODE_CamSocketRotation"))
	FRotator m_CamSocketRotation;

	UPROPERTY(BlueprintReadOnly, meta = (DisplayName = "CODE_SightLocation"))
	FVector m_SightLocation;

	UPROPERTY(BlueprintReadOnly, meta = (DisplayName = "CODE_bIsAiming"))
	bool m_bIsAiming;

	UPROPERTY(BlueprintReadOnly, meta = (DisplayName = "CODE_PitchPerBone"))
	FRotator m_PitchPerBone; 

	UPROPERTY(BlueprintReadOnly, meta = (DisplayName = "CODE_AimingMovementDuration"))
	float m_AimingMovementDuration;
};
