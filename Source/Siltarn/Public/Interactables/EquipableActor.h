#pragma once

#include "CoreMinimal.h"
#include "Interactables/PickupActor.h"
#include "Siltarn/Public/Interactables/InteractibleEnums.h"
#include "EquipableActor.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogClass_AEquipableActor, Log, All);

/*
	For the time being, act like a Pistol
*/
UCLASS()
class SILTARN_API AEquipableActor : public APickupActor
{
	GENERATED_BODY()

public:

	AEquipableActor();
	~AEquipableActor();	

	void BeginPlay() override;

	FORCEINLINE int32              GET_Damage()            const { return m_Damage            ; }
	FORCEINLINE int32              GET_MagazineCapacity()  const { return m_MagazineCapacity  ; }
	FORCEINLINE float              GET_RateOfFire()        const { return m_RateOfFire        ; }
	FORCEINLINE EItemEquipmentType GET_ItemEquipmentType() const { return m_ItemEquipmentType ; }

protected:

	void INTERFACE_Interact    (ASiltarnPlayerController* p_SiltarnController) override;

protected:

	UPROPERTY(EditAnywhere, Category = "Actor data", meta = (DisplayName = "Damage"             )) int32 m_Damage                         ;
	UPROPERTY(EditAnywhere, Category = "Actor data", meta = (DisplayName = "Magazine capacity"  )) int32 m_MagazineCapacity               ;
	UPROPERTY(EditAnywhere, Category = "Actor data", meta = (DisplayName = "Rate of fire"       )) float m_RateOfFire                     ;
	UPROPERTY(EditAnywhere, Category = "Actor data", meta = (DisplayName = "Item equipable slot")) EItemEquipmentType m_ItemEquipmentType ;


};
