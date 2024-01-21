#include "Characters/SiltarnCharacter.h"
#include "Camera/CameraComponent.h"
#include "Siltarn/Public/Weapons/Rifle.h"
#include "Siltarn/Public/Weapons/Pistol.h"
#include "Siltarn/Public/Weapons/DebugActor.h"
#include "Siltarn/Public/Characters/SiltarnCharacterAnimInstance.h"
#include "Siltarn/Public/Characters/SiltarnPlayerController.h"
#include "Siltarn/Public/Components/FOVSkeletalMeshComponent.h"
#include "Siltarn/Public/Interfaces/InteractInterface.h"
#include "Siltarn/Public/Interactables/PickupActor.h"
#include "Siltarn/Public/Interactables/PickupEntity.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/MovementComponent.h"

DEFINE_LOG_CATEGORY(LogClass_SiltarnCharacter);

ASiltarnCharacter::ASiltarnCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	m_bIsAiming              = false                 ;
	m_bActionAimingPressed   = false                 ;
	m_AimState               = EAimState::NOT_AIMING ;
	m_AimingMovementDuration = 0.1f                  ; 
	m_TimeElapsed            = 0.0f                  ;
	m_TraceDistance          = 1200.0f               ;

	m_DefaultCameraRelativeLocation = FVector(15.5f, 0.0f, 5.15f)  ;
	m_AimingCameraRelativeLocation  = FVector(15.5f, 3.65f, 5.15f) ;

	//

	m_CharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Character Mesh"));
	m_CharacterMesh->SetupAttachment(RootComponent);

	m_DefaultCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Default Camera"));
	m_DefaultCamera->SetupAttachment(m_CharacterMesh, FName("HeadSocket"));
	m_DefaultCamera->SetRelativeLocation(m_DefaultCameraRelativeLocation); // The location of the "DefaultCamera" socket. See doc for why these != to the coordinates in the editor
	m_DefaultCamera->bUsePawnControlRotation = false;
	m_DefaultCamera->SetActive(true);


	// *** Debugging camera *** //

	m_DebugRightSideSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Debug Right Side Spring Arm"));
	m_DebugRightSideSpringArm->SetupAttachment(RootComponent);
	m_DebugRightSideSpringArm->TargetArmLength = 400.0f;
	m_DebugRightSideSpringArm->bUsePawnControlRotation = false;

	m_DebugRightSideCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Debug Right Side Camera"));
	m_DebugRightSideCamera->SetupAttachment(m_DebugRightSideSpringArm, USpringArmComponent::SocketName);
	m_DebugRightSideCamera->bUsePawnControlRotation = false;
	m_DebugRightSideCamera->SetActive(false);

	//

	m_DebugLeftSideSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Debug Left Side Spring Arm"));
	m_DebugLeftSideSpringArm->SetupAttachment(RootComponent);
	m_DebugLeftSideSpringArm->TargetArmLength = 400.0f;
	m_DebugLeftSideSpringArm->bUsePawnControlRotation = false;

	m_DebugLeftSideCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Debug Left Side Camera"));
	m_DebugLeftSideCamera->SetupAttachment(m_DebugLeftSideSpringArm, USpringArmComponent::SocketName);
	m_DebugLeftSideCamera->bUsePawnControlRotation = false;
	m_DebugLeftSideCamera->SetActive(false);

	//

	m_DebugTopSideSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Debug Top Side Spring Arm"));
	m_DebugTopSideSpringArm->SetupAttachment(RootComponent);
	m_DebugTopSideSpringArm->TargetArmLength = 400.0f;
	m_DebugTopSideSpringArm->bUsePawnControlRotation = false;
		   
	m_DebugTopSideCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Debug Top Side Camera"));
	m_DebugTopSideCamera->SetupAttachment(m_DebugTopSideSpringArm, USpringArmComponent::SocketName);
	m_DebugTopSideCamera->bUsePawnControlRotation = false;
	m_DebugTopSideCamera->SetActive(false);

	//

	m_DebugFrontSideSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Debug Front Side Spring Arm"));
	m_DebugFrontSideSpringArm->SetupAttachment(RootComponent);
	m_DebugFrontSideSpringArm->TargetArmLength = 400.0f;
	m_DebugFrontSideSpringArm->bUsePawnControlRotation = false;
		   
	m_DebugFrontSideCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Debug Front Side Camera"));
	m_DebugFrontSideCamera->SetupAttachment(m_DebugFrontSideSpringArm, USpringArmComponent::SocketName);
	m_DebugFrontSideCamera->bUsePawnControlRotation = false;
	m_DebugFrontSideCamera->SetActive(false);
}

void ASiltarnCharacter::BeginPlay()
{
	Super::BeginPlay();

	INIT_References();
	INIT_SpawnWeaponAndAttachToCharacter();
}

void ASiltarnCharacter::INIT_References()
{
	verifyf((m_AnimInstance = Cast<USiltarnCharacterAnimInstance>(m_CharacterMesh->GetAnimInstance())) != nullptr,
		TEXT("ASiltarnCharacter::INIT_References() : m_AnimInstance wasn't properly initialized !"));

	verifyf((m_PlayerController = Cast<ASiltarnPlayerController>(Controller)) != nullptr, 
		TEXT("ASiltarnCharacter::INIT_References() : m_PlayerController wasn't properly initialized !"));
}

void ASiltarnCharacter::INIT_SpawnWeaponAndAttachToCharacter()
{
	/*if (m_WeaponClass)
	{
		m_Weapon = GetWorld()->SpawnActor<ARifle>(m_WeaponClass);

		if (m_Weapon && m_Weapon->GET_RifleMesh())
		{
			m_Weapon->SET_Owner(this);
			m_Weapon->SetOwner(this);
			m_Weapon->GET_RifleMesh()->AttachToComponent(m_CharacterMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), FName("Example_Socket")); // Need to change that garbage name
			m_Weapon->CALCULATE_SightTransform();

			if (m_AnimInstance)
			{
				m_AnimInstance->NEW_SightTransform(m_Weapon->GET_SightTransform());
			}
		}
	}*/

	if (m_PistolClass)
	{
		m_Pistol = GetWorld()->SpawnActor<APistol>(m_PistolClass);

		if (m_Pistol && m_Pistol->GET_PistolMesh())
		{
			m_Pistol->SET_Owner(this);
			m_Pistol->SetOwner(this);
			m_Pistol->GET_PistolMesh()->AttachToComponent(m_CharacterMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), FName("Example_Socket"));
			m_Pistol->CALCULATE_SightTransform();

			if (m_AnimInstance)
			{
				m_AnimInstance->NEW_SightTransform(m_Pistol->GET_SightTransform());
			}
		}
	}
}

void ASiltarnCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	INIT_Inputs();

	PlayerInputComponent->BindAxis("MoveForward", this, &ASiltarnCharacter::MOVE_Forward);
	PlayerInputComponent->BindAxis("MoveRight"  , this, &ASiltarnCharacter::MOVE_Right  );
	PlayerInputComponent->BindAxis("Turn"       , this, &ASiltarnCharacter::Turn        );
	PlayerInputComponent->BindAxis("LookUp"     , this, &ASiltarnCharacter::LookUp      );

	PlayerInputComponent->BindAction("Aim"             , IE_Pressed, this, &ASiltarnCharacter::ACTION_Aiming_PRESSED           );
	PlayerInputComponent->BindAction("DefaultCamera"   , IE_Pressed, this, &ASiltarnCharacter::ACTION_DefaultCamera_PRESSED    );
	PlayerInputComponent->BindAction("DebugCameraRight", IE_Pressed, this, &ASiltarnCharacter::ACTION_DebugCameraRight_PRESSED );
	PlayerInputComponent->BindAction("DebugCameraLeft" , IE_Pressed, this, &ASiltarnCharacter::ACTION_DebugCameraLeft_PRESSED  );
	PlayerInputComponent->BindAction("DebugCameraTop"  , IE_Pressed, this, &ASiltarnCharacter::ACTION_DebugCameraTop_PRESSED   );
	PlayerInputComponent->BindAction("DebugCameraFront", IE_Pressed, this, &ASiltarnCharacter::ACTION_DebugCameraFront_PRESSED );
	PlayerInputComponent->BindAction("Interact"        , IE_Pressed, this, &ASiltarnCharacter::ACTION_Interact_PRESSED         );
	PlayerInputComponent->BindAction("ToggleInventory" , IE_Pressed, this, &ASiltarnCharacter::ACTION_ToggleInventory_PRESSED  );
	PlayerInputComponent->BindAction("ToggleEchapMenu" , IE_Pressed, this, &ASiltarnCharacter::ACTION_ToggleEchapMenu_PRESSED  );
}

void ASiltarnCharacter::INIT_Inputs()
{
	static bool _bAreBindingsAdded = false;

	if (_bAreBindingsAdded == false)
	{
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveForward", EKeys::W     ,  1.0f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveForward", EKeys::S     , -1.0f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveRight"  , EKeys::A     , -1.0f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveRight"  , EKeys::D     ,  1.0f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("Turn"       , EKeys::MouseX,  1.0f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("LookUp"     , EKeys::MouseY, -1.0f));

		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("Aim"             , EKeys::RightMouseButton               ));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("DefaultCamera"   , EKeys::NumPadZero       , false, true ));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("DebugCameraRight", EKeys::NumPadOne        , false, true ));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("DebugCameraLeft" , EKeys::NumPadTwo        , false, true ));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("DebugCameraTop"  , EKeys::NumPadThree      , false, true ));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("DebugCameraFront", EKeys::NumPadFour       , false, true ));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("Interact"        , EKeys::E                              ));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("ToggleInventory" , EKeys::I                              ));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("ToggleEchapMenu" , EKeys::Escape                         ));

		_bAreBindingsAdded = true;
	}	
}

void ASiltarnCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SWITCH_Camera(DeltaTime);
	TRACE_LineForward();
}

void ASiltarnCharacter::MOVE_Forward(const float p_Value)
{
	if (p_Value != 0.0f)
	{
		// We do it this way so that we base our movement onto the Mesh direction rather than the capsule component direction, which is the root. 
		//
		const FVector _Direction(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X));
		AddMovementInput(_Direction, p_Value);
	}
}

void ASiltarnCharacter::MOVE_Right(const float p_Value)
{
	if (p_Value != 0.0f)
	{
		// We do it this way so that we base our movement onto the Mesh direction rather than the capsule component direction, which is the root. 
		//
		const FVector _Direction(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::Y));
		AddMovementInput(_Direction, p_Value);
	}
}

void ASiltarnCharacter::Turn(const float p_Value)
{
	AddControllerYawInput(p_Value);
}

void ASiltarnCharacter::LookUp(const float p_Value)
{
	AddControllerPitchInput(p_Value);
}

void ASiltarnCharacter::ACTION_Aiming_PRESSED()
{
	if (m_AnimInstance)
	{
		if (m_bIsAiming == true)
		{
			m_AnimInstance->ACTIVATE_Aiming(false);
			m_bIsAiming = false;
			m_bActionAimingPressed = true;
			m_AimState = EAimState::NOT_AIMING;
		}
		else
		{
			m_AnimInstance->ACTIVATE_Aiming(true);
			m_bIsAiming = true;
			m_bActionAimingPressed = true;
			m_AimState = EAimState::AIMING;
		}
	}
}

void ASiltarnCharacter::ACTION_DefaultCamera_PRESSED()
{
	checkf(m_DefaultCamera        != nullptr, TEXT("ASiltarnCharacter::ACTION_DefaultCamera_PRESSED() : m_DefaultCamera is NULL !"       ));
	checkf(m_DebugRightSideCamera != nullptr, TEXT("ASiltarnCharacter::ACTION_DefaultCamera_PRESSED() : m_DebugRightSideCamera is NULL !"));
	checkf(m_DebugLeftSideCamera  != nullptr, TEXT("ASiltarnCharacter::ACTION_DefaultCamera_PRESSED() : m_DebugLeftSideCamera is NULL !" ));
	checkf(m_DebugTopSideCamera   != nullptr, TEXT("ASiltarnCharacter::ACTION_DefaultCamera_PRESSED() : m_DebugTopSideCamera is NULL !"  ));
	checkf(m_DebugFrontSideCamera != nullptr, TEXT("ASiltarnCharacter::ACTION_DefaultCamera_PRESSED() : m_DebugFrontSideCamera is NULL !"));

	m_DebugRightSideCamera->SetActive(false) ;
	m_DebugLeftSideCamera ->SetActive(false) ;
	m_DebugTopSideCamera  ->SetActive(false) ;
	m_DebugFrontSideCamera->SetActive(false) ;
	m_DefaultCamera       ->SetActive(true ) ;
}

void ASiltarnCharacter::ACTION_DebugCameraRight_PRESSED()
{
	checkf(m_DefaultCamera        != nullptr, TEXT("ASiltarnCharacter::ACTION_DebugCameraRight_PRESSED() : m_DefaultCamera is NULL !"       ));
	checkf(m_DebugRightSideCamera != nullptr, TEXT("ASiltarnCharacter::ACTION_DebugCameraRight_PRESSED() : m_DebugRightSideCamera is NULL !"));
	checkf(m_DebugLeftSideCamera  != nullptr, TEXT("ASiltarnCharacter::ACTION_DebugCameraRight_PRESSED() : m_DebugLeftSideCamera is NULL !" ));
	checkf(m_DebugTopSideCamera   != nullptr, TEXT("ASiltarnCharacter::ACTION_DebugCameraRight_PRESSED() : m_DebugTopSideCamera is NULL !"  ));
	checkf(m_DebugFrontSideCamera != nullptr, TEXT("ASiltarnCharacter::ACTION_DebugCameraRight_PRESSED() : m_DebugFrontSideCamera is NULL !"));

	m_DefaultCamera       ->SetActive(false) ;
	m_DebugLeftSideCamera ->SetActive(false) ;
	m_DebugTopSideCamera  ->SetActive(false) ;
	m_DebugFrontSideCamera->SetActive(false) ;
	m_DebugRightSideCamera->SetActive(true ) ;
}

void ASiltarnCharacter::ACTION_DebugCameraLeft_PRESSED()
{
	checkf(m_DefaultCamera        != nullptr, TEXT("ASiltarnCharacter::ACTION_DebugCameraLeft_PRESSED() : m_DefaultCamera is NULL !"       ));
	checkf(m_DebugRightSideCamera != nullptr, TEXT("ASiltarnCharacter::ACTION_DebugCameraLeft_PRESSED() : m_DebugRightSideCamera is NULL !"));
	checkf(m_DebugLeftSideCamera  != nullptr, TEXT("ASiltarnCharacter::ACTION_DebugCameraLeft_PRESSED() : m_DebugLeftSideCamera is NULL !" ));
	checkf(m_DebugTopSideCamera   != nullptr, TEXT("ASiltarnCharacter::ACTION_DebugCameraLeft_PRESSED() : m_DebugTopSideCamera is NULL !"  ));
	checkf(m_DebugFrontSideCamera != nullptr, TEXT("ASiltarnCharacter::ACTION_DebugCameraLeft_PRESSED() : m_DebugFrontSideCamera is NULL !"));

	m_DefaultCamera       ->SetActive(false) ;
	m_DebugRightSideCamera->SetActive(false) ;
	m_DebugTopSideCamera  ->SetActive(false) ;
	m_DebugFrontSideCamera->SetActive(false) ;
	m_DebugLeftSideCamera ->SetActive(true ) ;
}

void ASiltarnCharacter::ACTION_DebugCameraTop_PRESSED()
{
	checkf(m_DefaultCamera        != nullptr, TEXT("ASiltarnCharacter::ACTION_DebugCameraTop_PRESSED() : m_DefaultCamera is NULL !"       ));
	checkf(m_DebugRightSideCamera != nullptr, TEXT("ASiltarnCharacter::ACTION_DebugCameraTop_PRESSED() : m_DebugRightSideCamera is NULL !"));
	checkf(m_DebugLeftSideCamera  != nullptr, TEXT("ASiltarnCharacter::ACTION_DebugCameraTop_PRESSED() : m_DebugLeftSideCamera is NULL !" ));
	checkf(m_DebugTopSideCamera   != nullptr, TEXT("ASiltarnCharacter::ACTION_DebugCameraTop_PRESSED() : m_DebugTopSideCamera is NULL !"  ));
	checkf(m_DebugFrontSideCamera != nullptr, TEXT("ASiltarnCharacter::ACTION_DebugCameraTop_PRESSED() : m_DebugFrontSideCamera is NULL !"));

	m_DefaultCamera       ->SetActive(false) ;
	m_DebugRightSideCamera->SetActive(false) ;
	m_DebugLeftSideCamera ->SetActive(false) ;
	m_DebugFrontSideCamera->SetActive(false) ;
	m_DebugTopSideCamera  ->SetActive(true ) ;
}

void ASiltarnCharacter::ACTION_DebugCameraFront_PRESSED()
{
	checkf(m_DefaultCamera        != nullptr, TEXT("ASiltarnCharacter::ACTION_DebugCameraFront_PRESSED() : m_DefaultCamera is NULL !"       ));
	checkf(m_DebugRightSideCamera != nullptr, TEXT("ASiltarnCharacter::ACTION_DebugCameraFront_PRESSED() : m_DebugRightSideCamera is NULL !"));
	checkf(m_DebugLeftSideCamera  != nullptr, TEXT("ASiltarnCharacter::ACTION_DebugCameraFront_PRESSED() : m_DebugLeftSideCamera is NULL !" ));
	checkf(m_DebugTopSideCamera   != nullptr, TEXT("ASiltarnCharacter::ACTION_DebugCameraFront_PRESSED() : m_DebugTopSideCamera is NULL !"  ));
	checkf(m_DebugFrontSideCamera != nullptr, TEXT("ASiltarnCharacter::ACTION_DebugCameraFront_PRESSED() : m_DebugFrontSideCamera is NULL !"));

	m_DefaultCamera       ->SetActive(false) ;
	m_DebugRightSideCamera->SetActive(false) ;
	m_DebugLeftSideCamera ->SetActive(false) ;
	m_DebugTopSideCamera  ->SetActive(false) ;
	m_DebugFrontSideCamera->SetActive(true ) ;
}

void ASiltarnCharacter::ACTION_Interact_PRESSED()
{
	checkf(GetController() != nullptr, TEXT("ASiltarnCharacter::ACTION_Interact_PRESSED() : GetController() returns NULL !"));
	checkf(GetWorld()      != nullptr, TEXT("ASiltarnCharacter::ACTION_Interact_PRESSED() : GetWorld() returns NULL !"     ));

	FVector _Loc;
	FRotator _Rot;
	FHitResult _HitResult;

	GetController()->GetPlayerViewPoint(_Loc, _Rot);

	FVector _Start = _Loc;
	FVector _End = _Start + (_Rot.Vector() * m_TraceDistance); // Rot.Vector() is the forward vector

	FCollisionQueryParams _TraceParams;

	bool _bHit = GetWorld()->LineTraceSingleByChannel(_HitResult, _Start, _End, ECC_Visibility, _TraceParams);

	//DrawDebugLine(GetWorld(), _Start, _End, FColor::Orange, false, 2.0f);

	if (_bHit)
	{
		IInteractInterface* _Interface = Cast<IInteractInterface>(_HitResult.GetActor());

		if (_Interface)
		{
			_Interface->INTERFACE_Interact(m_PlayerController);
		}
	}
}

void ASiltarnCharacter::ACTION_ToggleInventory_PRESSED()
{
	checkf(m_PlayerController != nullptr, TEXT("ASiltarnCharacter::ACTION_ToggleInventory_PRESSED() : m_PlayerController is NULL !"));

	m_PlayerController->TOGGLE_Inventory();
}

void ASiltarnCharacter::ACTION_ToggleEchapMenu_PRESSED()
{
	checkf(m_PlayerController != nullptr, TEXT("ASiltarnCharacter::ACTION_ToggleEchapMenu_PRESSED() : m_PlayerController is NULL !"));

	m_PlayerController->TOGGLE_EchapMenu();
}

FVector ASiltarnCharacter::GET_DefaultCameraLocation() const
{
	checkf(m_DefaultCamera != nullptr, TEXT("ASiltarnCharacter::GET_DefaultCameraLocation() : m_DefaultCamera is NULL !"));

	return m_DefaultCamera->GetComponentLocation();
}

void ASiltarnCharacter::SWITCH_Camera(float p_DeltaTime)
{
	checkf(m_DefaultCamera != nullptr, TEXT("ASiltarnCharacter::SWITCH_Camera() : m_DefaultCamera is NULL !"));

	if (m_bActionAimingPressed)
	{
		if (m_AimState == EAimState::AIMING)
		{
			if (m_TimeElapsed < m_AimingMovementDuration)
			{
				m_DefaultCamera->SetRelativeLocation(FMath::Lerp
				(
					m_DefaultCameraRelativeLocation,
					m_AimingCameraRelativeLocation,
					m_TimeElapsed / m_AimingMovementDuration
				));

				m_TimeElapsed += p_DeltaTime;
			}
			else
			{
				m_DefaultCamera->SetRelativeLocation(m_AimingCameraRelativeLocation); // Otherwise the camera is not properly aligned when "m_CameraAimingLerpDuration" is too small
				m_TimeElapsed = 0.0f;
				m_bActionAimingPressed = false;
			}
		}
		else
		{
			if (m_TimeElapsed < m_AimingMovementDuration)
			{
				m_DefaultCamera->SetRelativeLocation(FMath::Lerp
				(
					m_AimingCameraRelativeLocation,
					m_DefaultCameraRelativeLocation,
					m_TimeElapsed / m_AimingMovementDuration
				));

				m_TimeElapsed += p_DeltaTime;
			}
			else
			{
				m_DefaultCamera->SetRelativeLocation(m_DefaultCameraRelativeLocation); // See above
				m_TimeElapsed = 0.0f;
				m_bActionAimingPressed = false;
			}
		}
	}
}

void ASiltarnCharacter::TRACE_LineForward()
{
	checkf(GetController() != nullptr, TEXT("ASiltarnCharacter::TRACE_LineForward() : GetController() returns NULL !"));
	checkf(GetWorld()      != nullptr, TEXT("ASiltarnCharacter::TRACE_LineForward() : GetWorld() returns NULL !"     ));

	FVector _Loc;
	FRotator _Rot;
	FHitResult _Hit;

	GetController()->GetPlayerViewPoint(_Loc, _Rot);

	FVector _Start = _Loc;
	FVector _End = _Start + (_Rot.Vector() * m_TraceDistance); // Rot.Vector() is the forward vector

	FCollisionQueryParams _TraceParams;

	bool _bHit = GetWorld()->LineTraceSingleByChannel(_Hit, _Start, _End, ECC_Visibility, _TraceParams);

	//DrawDebugLine(GetWorld(), _Start, _End, FColor::Orange, false, 0.01f);

	if (_bHit)
	{
		//DrawDebugBox(GetWorld(), _Hit.ImpactPoint, FVector(5.0f), FColor::Emerald, false, 0.01f);

		AActor* _Interactable = _Hit.GetActor();

		if (_Interactable == nullptr)
		{
			return;
		}

		IInteractInterface* _InterfaceActor = Cast<IInteractInterface>(_Interactable);

		if (_InterfaceActor)
		{
			if (_InterfaceActor == m_FocusedInteractActor)
			{
				return;
			}

			if (m_FocusedInteractActor)
			{
				m_FocusedInteractActor->INTERFACE_OnEndFocus(m_PlayerController);
			}

			_InterfaceActor->INTERFACE_OnStartFocus(m_PlayerController);
			m_FocusedInteractActor = _InterfaceActor;
		}
		else
		{
			if (m_FocusedInteractActor)
			{
				m_FocusedInteractActor->INTERFACE_OnEndFocus(m_PlayerController);
				m_FocusedInteractActor = nullptr;
			}
		}
	}
	else
	{
		if (m_FocusedInteractActor)
		{
			m_FocusedInteractActor->INTERFACE_OnEndFocus(m_PlayerController);
			m_FocusedInteractActor = nullptr;
		}
	}
}

void ASiltarnCharacter::DROP_Item(UPickupEntity* p_Item)
{
	checkf(GetWorld() != nullptr, TEXT("ASiltarnCharacter::DROP_Item() : GetWorld() returns NULL."));

	check(m_CharacterMesh != nullptr);

	const USkeletalMeshSocket* _DropItemSocket = m_CharacterMesh->GetSocketByName("DropItemSocket");
	check(_DropItemSocket != nullptr);

	UE_LOG(LogTemp, Error, TEXT("We've arrived up to here !"));

	const FTransform _SocketTransform = _DropItemSocket->GetSocketTransform(m_CharacterMesh);

	//EPickupEntityType _ItemEntityType = p_Item->GET_ItemType();

	//if ()

	const FVector _Loc(_SocketTransform.GetLocation());
	const FRotator _Rot(_SocketTransform.GetRotation().Rotator());

	check(p_Item != nullptr);
	check(p_Item->GET_ActorClass() != nullptr);
	check(p_Item->GET_ActorClass()->GetName().IsEmpty() == false);

	UE_LOG(LogClass_SiltarnCharacter, Warning, TEXT("p_Item class name : %s"), *p_Item->GET_ActorClass()->GetName());



	//APickupActor* _Actor = GetWorld()->SpawnActor<APickupActor>(p_Item->GET_ActorClass(), _SocketTransform.GetLocation(), _SocketTransform.GetRotation().Rotator());




	GetWorld()->SpawnActor(p_Item->GET_ActorClass(), &_Loc, &_Rot);


	/*
		Luciole || Need to eventually normalize the mass of all pickup items and set the impulse to a constant.
		No matter the size of the object, they all should be dropped following the same curve.
	*/
	//_Actor->ADD_Impulse(_Actor->GetActorForwardVector() * 250000.0f);
}