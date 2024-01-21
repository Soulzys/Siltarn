#include "Weapons/Magazine.h"

AMagazine::AMagazine()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMagazine::BeginPlay()
{
	Super::BeginPlay();
}

void AMagazine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

