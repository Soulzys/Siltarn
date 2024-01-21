#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Magazine.generated.h"

UCLASS()
class SILTARN_API AMagazine : public AActor
{
	GENERATED_BODY()
	
public:	

	AMagazine();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

};
