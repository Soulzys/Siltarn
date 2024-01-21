#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DebugActor.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogClass_DebugActor, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogStruct_FDebugActore, Log, All);

UCLASS()
class SILTARN_API ADebugActor : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ADebugActor();
	~ADebugActor();

protected:

	virtual void BeginPlay() override;

private:

	static int32 m_InstanceCount;

	int32 m_Toto;
};


struct FDebugActore
{
	//GENERATED_BODY()

public:

	FDebugActore();
	~FDebugActore();

private:

	static int32 s_InstanceCount;
};
