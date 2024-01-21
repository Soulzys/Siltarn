#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SiltarnCharacter.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogClass_SiltarnCharacter, Log, All);

class USkeletalMeshComponent        ;
class UCameraComponent              ;
//class ARifle                        ;
class APistol                       ;
class USiltarnCharacterAnimInstance ;
class UFOVSkeletalMeshComponent     ;
class USpringArmComponent           ;
class ASiltarnPlayerController      ;
class IInteractInterface;
class ADebugActor;
class UPickupEntity;


UENUM()
enum class EAimState : uint8
{
	AIMING     = 0,
	NOT_AIMING = 1, 
	INBETWEEN  = 2
};

UCLASS()
class SILTARN_API ASiltarnCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	ASiltarnCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void DROP_Item(UPickupEntity* p_Item);

	FORCEINLINE USkeletalMeshComponent* GET_CharacterMesh()          const { return m_CharacterMesh;          }
	//FORCEINLINE ARifle*                 GET_Weapon()                 const { return m_Weapon;                 }
	FORCEINLINE APistol*                GET_Pistol()                 const { return m_Pistol;                 }
	FORCEINLINE float                   GET_AimingMovementDuration() const { return m_AimingMovementDuration; } // Used in AnimBP for "Blend Poses by bool"

	FVector GET_DefaultCameraLocation() const;

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:

	void INIT_Inputs                         ();
	void INIT_SpawnWeaponAndAttachToCharacter();
	void INIT_References                     ();

	void MOVE_Forward(const float p_Value);
	void MOVE_Right  (const float p_Value);
	void Turn        (const float p_Value);
	void LookUp      (const float p_Value);

	void ACTION_Aiming_PRESSED          ();
	void ACTION_DefaultCamera_PRESSED   ();
	void ACTION_DebugCameraRight_PRESSED();
	void ACTION_DebugCameraLeft_PRESSED ();
	void ACTION_DebugCameraTop_PRESSED  ();
	void ACTION_DebugCameraFront_PRESSED();
	void ACTION_Interact_PRESSED        ();
	void ACTION_ToggleInventory_PRESSED ();
	void ACTION_ToggleEchapMenu_PRESSED ();

	void SWITCH_Camera(float p_DeltaTime);
	void TRACE_LineForward();
	

protected:

	UPROPERTY(EditAnywhere, Category = "Character Geometry", meta = (DisplayName = "Character Mesh"))
	USkeletalMeshComponent* m_CharacterMesh = nullptr;

	// Main Camera used for playing. Default position is in the middle of the character's face
	UPROPERTY(EditAnywhere, Category = "Camera", meta = (DisplayName = "Default Camera"))
	UCameraComponent* m_DefaultCamera = nullptr;


	// *** Debugging camera *** //
	UPROPERTY(EditAnywhere, Category = "Camera", meta = (DisplayName = "Debug Right Side Spring Arm"))
	USpringArmComponent* m_DebugRightSideSpringArm = nullptr;

	UPROPERTY(EditAnywhere, Category = "Camera", meta = (DisplayName = "Debug Right Side Camera"))
	UCameraComponent* m_DebugRightSideCamera = nullptr;

	UPROPERTY(EditAnywhere, Category = "Camera", meta = (DisplayName = "Debug Left Side Spring Arm"))
	USpringArmComponent* m_DebugLeftSideSpringArm = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Camera", meta = (DisplayName = "Debug Left Side Camera"))
	UCameraComponent* m_DebugLeftSideCamera = nullptr;

	UPROPERTY(EditAnywhere, Category = "Camera", meta = (DisplayName = "Debug Top Side Spring Arm"))
	USpringArmComponent* m_DebugTopSideSpringArm = nullptr;

	UPROPERTY(EditAnywhere, Category = "Camera", meta = (DisplayName = "Debug Top Side Camera")) 
	UCameraComponent* m_DebugTopSideCamera = nullptr;

	UPROPERTY(EditAnywhere, Category = "Camera", meta = (DisplayName = "Debug Front Side Spring Arm"))
	USpringArmComponent* m_DebugFrontSideSpringArm = nullptr;

	UPROPERTY(EditAnywhere, Category = "Camera", meta = (DisplayName = "Debug Front Side Camera"))
	UCameraComponent* m_DebugFrontSideCamera = nullptr;

	


	//UPROPERTY(EditAnywhere, Category = "Weapon", meta = (DisplayName = "Weapon Class"))
	//TSubclassOf<ARifle> m_WeaponClass;

	UPROPERTY(EditAnywhere, Category = "Weapon", meta = (DisplayName = "Pistol Class"))
	TSubclassOf<APistol> m_PistolClass;

	// These should be regarded as const 
	FVector m_DefaultCameraRelativeLocation ;
	FVector m_AimingCameraRelativeLocation  ;

	//ARifle* m_Weapon = nullptr;
	APistol* m_Pistol = nullptr;

	USiltarnCharacterAnimInstance* m_AnimInstance = nullptr;

	bool m_bIsAiming;
	bool m_bActionAimingPressed;

private:

	EAimState m_AimState;

	UPROPERTY(EditAnywhere, Category = "Aiming", meta = (DisplayName = "Aiming movement duration"))
	float m_AimingMovementDuration;

	float m_TimeElapsed;

	bool m_bIsTesting = false;

	ADebugActor* m_DebugActor = nullptr;


	// Line Tracing
	//
	UPROPERTY(EditAnywhere, Category = "LineTracing", meta = (DisplayName = "Trace distance"))
	float m_TraceDistance;

	AActor* m_FocusedActor = nullptr;
	IInteractInterface* m_FocusedInteractActor = nullptr;

	ASiltarnPlayerController* m_PlayerController = nullptr;

};
