#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "FOVSkeletalMeshComponent.generated.h"

UCLASS()
class SILTARN_API UFOVSkeletalMeshComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:

	virtual FMatrix GetRenderMatrix() const override;
	
};
