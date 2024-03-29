#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogClass_Weapon, Log, All);

class USceneComponent        ;
class USkeletalMeshComponent ;

UCLASS()
class SILTARN_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	

	AWeapon();

protected:

	virtual void BeginPlay() override;

};
