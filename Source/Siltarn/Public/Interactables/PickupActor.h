#pragma once

#include "CoreMinimal.h"
#include "Interactables/InteractableActor.h"
#include "Siltarn/Public/Interactables/InteractableEnums.h"
#include "PickupActor.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogClass_APickupActor, Log, All);

class UMaterialInstance;
class ASiltarnPlayerController;

UCLASS()
class SILTARN_API APickupActor : public AInteractableActor
{
	GENERATED_BODY()

public:

	APickupActor();
	~APickupActor();

	FORCEINLINE UMaterialInstance*              GET_Icon()              const { return m_Icon              ; }
	FORCEINLINE FIntPoint                       GET_InventorySpace()    const { return m_InventorySpace    ; }
	FORCEINLINE int32                           GET_ResellValue()       const { return m_ResellValue       ; }
	FORCEINLINE FString                         GET_Description()       const { return m_Description       ; }
	FORCEINLINE TSubclassOf<AInteractableActor> GET_ItemClass()         const { return m_ItemClass         ; }
	FORCEINLINE EPickupActorRaretyType          GET_Rarety()            const { return m_Rarety            ; }
	FORCEINLINE bool                            GET_IsEquipable()       const { return m_bIsEquipable      ; }
	FORCEINLINE bool                            GET_IsDroppableAsIs()   const { return m_bIsDroppableAsIs  ; }
	FORCEINLINE TSubclassOf<AInteractableActor> GET_DroppableBagClass() const { return m_DroppableBagClass ; }

protected:

	void INTERFACE_Interact(ASiltarnPlayerController* p_SiltarnController) override;

protected:

	UPROPERTY(EditAnywhere, Category = "Actor data", meta = (DisplayName = "Icon"                    )) UMaterialInstance*              m_Icon = nullptr              ;
	UPROPERTY(EditAnywhere, Category = "Actor data", meta = (DisplayName = "Space taken in inventory")) FIntPoint                       m_InventorySpace              ;
	UPROPERTY(EditAnywhere, Category = "Actor data", meta = (DisplayName = "Resell value"            )) int32                           m_ResellValue                 ;
	UPROPERTY(EditAnywhere, Category = "Actor data", meta = (DisplayName = "Item description"        )) FString                         m_Description                 ;
	UPROPERTY(EditAnywhere, Category = "Actor data", meta = (DisplayName = "Blueprint Class"         )) TSubclassOf<AInteractableActor> m_ItemClass = nullptr         ;
	UPROPERTY(EditAnywhere, Category = "Actor data", meta = (DisplayName = "Rarety"                  )) EPickupActorRaretyType          m_Rarety                      ;	
	UPROPERTY(EditAnywhere, Category = "Actor data", meta = (DisplayName = "Is placed in level"      )) bool                            m_bIsPlacedInLevel            ;
	UPROPERTY(EditAnywhere, Category = "Actor data", meta = (DisplayName = "Is equipable"            )) bool                            m_bIsEquipable                ;
	UPROPERTY(EditAnywhere, Category = "Actor data", meta = (DisplayName = "Is droppable as is"      )) bool                            m_bIsDroppableAsIs            ;
	UPROPERTY(EditAnywhere, Category = "Actor data", meta = (DisplayName = "Droppable Bag Class"     )) TSubclassOf<AInteractableActor> m_DroppableBagClass = nullptr ;
};
