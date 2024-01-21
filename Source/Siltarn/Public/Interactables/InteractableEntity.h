#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InteractableEntity.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogClass_UInteractableEntity, Log, All);

class AInteractableActor;

UCLASS()
class SILTARN_API UInteractableEntity : public UObject
{
	GENERATED_BODY()

public:

	UInteractableEntity();
	~UInteractableEntity();

	void Initialize_Entity(AInteractableActor* p_IA);
	
	virtual void DEBUG_DisplayDataThroughUELogs();	

	FORCEINLINE const FString& GET_Name() const { return m_Name; }


protected:

	// *** Item data coming from AInteractableActor
	//
	FString m_Name;	


private:

	static uint32 m_InstanceCount;	
};
