#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Siltarn/Public/Interfaces/InteractInterface.h"
#include "InteractableActor.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogClass_AInteractableActor, Log, All);

class ASiltarnCharacter        ;
class ASiltarnPlayerController ;
class UMaterialInstance        ;
class UStaticMeshComponent     ;
class USceneComponent          ;

/*
	Currently using this class as if it was APistol. 
	We will deal with inheritance later.
*/

UCLASS()
class SILTARN_API AInteractableActor : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	

	AInteractableActor();
	~AInteractableActor();

	void ADD_Impulse(const FVector& p_Impulse) const;

	FORCEINLINE FString GET_Name()        const { return m_Name            ; }
	FORCEINLINE bool    IS_Interactable() const { return m_bIsInteractible ; }

protected:

	virtual void BeginPlay() override;

	void INTERFACE_Interact    (ASiltarnPlayerController* p_SiltarnController) override;
	void INTERFACE_OnStartFocus(ASiltarnPlayerController* p_SiltarnController) override;
	void INTERFACE_OnEndFocus  (ASiltarnPlayerController* p_SiltarnController) override;

protected:

	// *** Item data
	//
	UPROPERTY(EditAnywhere, Category = "Actor data"  , meta = (DisplayName = "Name"                    )) FString m_Name            ;
	UPROPERTY(EditAnywhere, Category = "Actor status", meta = (DisplayName = "Is actor interactible"   )) bool    m_bIsInteractible ;


	// Components
	USceneComponent* m_RootComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = "Mesh", meta = (DisplayName = "Static Mesh")) 
	UStaticMeshComponent* m_StaticMesh = nullptr;



	static int32 m_InstanceCount;
};
