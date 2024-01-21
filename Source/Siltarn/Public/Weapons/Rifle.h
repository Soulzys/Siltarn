#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Rifle.generated.h"

class UStaticMeshComponent      ;
class USkeletalMeshComponent    ;
class UFOVSkeletalMeshComponent ;
class USceneComponent           ;
class ASiltarnCharacter         ;
class USkeletalMeshSocket       ;

DECLARE_LOG_CATEGORY_EXTERN(LogClass_Rifle, Log, All);

UCLASS()
class SILTARN_API ARifle : public AActor
{
	GENERATED_BODY()
	
public:	

	ARifle();
	~ARifle();

	void CALCULATE_SightTransform();

	FORCEINLINE USkeletalMeshComponent* GET_RifleMesh()      const { return m_RifleMesh      ; }
	FORCEINLINE UStaticMeshComponent*   GET_IronSightMesh()  const { return m_IronSightMesh  ; }
	FORCEINLINE FTransform              GET_SightTransform() const { return m_SightTransform ; }
	
	void SET_Owner(ASiltarnCharacter* p_Owner);

protected:

	virtual void BeginPlay() override;

protected:

	USceneComponent* m_RootComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = "Rifle Geometry", meta = (DisplayName = "Rifle Mesh"))
	USkeletalMeshComponent* m_RifleMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Optics", meta = (DisplayName = "Optic Sight")) // "Front"
	UStaticMeshComponent * m_OpticSightMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Optics", meta = (DisplayName = "Iron Sight")) // "Rear"
	UStaticMeshComponent* m_IronSightMesh = nullptr;

	ASiltarnCharacter* m_RifleOwner = nullptr;

	const USkeletalMeshSocket* m_OpticSightMeshSocket = nullptr;
	const USkeletalMeshSocket* m_IronSightMeshSocket  = nullptr;

	FTransform m_SightTransform;

	UPROPERTY(EditAnywhere, Category = "Optics", meta = (DisplayName = "Distance from camera"))
	float m_DistanceFromCamera;

private:

	static int32 m_InstanceCount;
};
