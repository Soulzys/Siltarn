#pragma once

#include "CoreMinimal.h"
#include "Interactables/PickupEntity.h"
#include "Siltarn/Public/Interactables/InteractibleEnums.h"
#include "EquipableEntity.generated.h"

class AEquipableActor   ;

UCLASS()
class SILTARN_API UEquipableEntity : public UPickupEntity
{
	GENERATED_BODY()

public:

	UEquipableEntity();
	~UEquipableEntity();

	void Initialize_Entity(AEquipableActor* p_IA);

	virtual EPickupEntityType GET_ItemType() const override;

	FORCEINLINE int32              GET_Damage()            const { return m_Damage            ; }
	FORCEINLINE int32              GET_MagazineCapacity()  const { return m_MagazineCapacity  ; }
	FORCEINLINE float              GET_RateOfFire()        const { return m_RateOfFire        ; }
	FORCEINLINE EItemEquipmentType GET_ItemEquipmentType() const { return m_ItemEquipmentType ; }

protected:

	int32              m_Damage            ;
	int32              m_MagazineCapacity  ;
	float              m_RateOfFire        ;
	EItemEquipmentType m_ItemEquipmentType ;	
};
