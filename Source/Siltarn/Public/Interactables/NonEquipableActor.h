#pragma once

#include "CoreMinimal.h"
#include "Interactables/PickupActor.h"
#include "NonEquipableActor.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogClass_ANonEquipableActor, Log, All);

class ASiltarnPlayerController;

/*
	Act as ammo
*/
UCLASS()
class SILTARN_API ANonEquipableActor : public APickupActor
{
	GENERATED_BODY()

public:

	ANonEquipableActor();
	~ANonEquipableActor();

	void BeginPlay() override;

	FORCEINLINE int32 GET_DamageBonus() const { return m_DamageBonus; }

protected:

	void INTERFACE_Interact    (ASiltarnPlayerController* p_SiltarnController) override;

protected:

	UPROPERTY(EditAnywhere, Category = "Item data", meta = (DisplayName = "Damage bonus")) int32 m_DamageBonus;
};
