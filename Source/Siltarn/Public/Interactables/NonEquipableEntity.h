#pragma once

#include "CoreMinimal.h"
#include "Interactables/PickupEntity.h"
#include "NonEquipableEntity.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogClass_UNonEquipableEntity, Log, All);

class ANonEquipableActor;

UCLASS()
class SILTARN_API UNonEquipableEntity : public UPickupEntity
{
	GENERATED_BODY()
	
public:

	UNonEquipableEntity();
	~UNonEquipableEntity();

	void Initialize_Entity(ANonEquipableActor* p_IA);

	virtual EPickupEntityType GET_ItemType() const override;

	FORCEINLINE int32 GET_DamageBonus() const { return m_DamageBonus; }

protected:

	int32 m_DamageBonus;
};
