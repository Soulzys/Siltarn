#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "DefaultGameMode.generated.h"

UCLASS()
class UTestClass : public UObject
{
	GENERATED_BODY()
public:

	UTestClass()
	{
		UE_LOG(LogTemp, Warning, TEXT("A TestClass instance was created !"));
	}

	~UTestClass()
	{
		UE_LOG(LogTemp, Warning, TEXT("A TestClass instance was destroyed !"));
	}

	FIntPoint s_Point;
};

UCLASS()
class SILTARN_API ADefaultGameMode : public AGameMode
{
	GENERATED_BODY()

public:

	ADefaultGameMode();

protected:

	virtual void BeginPlay() override;

private:

	TArray<UTestClass*> m_Classes;
	
};
