#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pistol.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogClass_Pistol, Log, All);

class ASiltarnCharacter;
class USkeletalMeshComponent;
class USceneComponent;
class UMaterialInstance;

UCLASS()
class SILTARN_API APistol : public AActor
{
	GENERATED_BODY()
	
public:	

	APistol();
	~APistol();

	void CALCULATE_SightTransform();

	FORCEINLINE USkeletalMeshComponent* GET_PistolMesh() const { return m_PistolMesh; }
	FORCEINLINE FTransform GET_SightTransform() const { return m_SightTransform; }

	void SET_Owner(ASiltarnCharacter* p_Owner);

protected:

	virtual void BeginPlay() override;

private:

	USceneComponent* m_RootComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = "Pistol Geometry", meta = (DisplayName = "Pistol Mesh"))
	USkeletalMeshComponent* m_PistolMesh = nullptr;

	ASiltarnCharacter* m_PistolOwner = nullptr;

	FTransform m_SightTransform;

	UPROPERTY(EditAnywhere, Category = "Optics", meta = (DisplayName = "Distance from camera"))
	float m_DistanceFromCamera;

	UPROPERTY(EditAnywhere, Category = "UI", meta = (DisplayName = "Inventory icon"))
	UMaterialInstance* m_InventoryIcon = nullptr; // Not sure UMaterialInstance is the right class for this
};
