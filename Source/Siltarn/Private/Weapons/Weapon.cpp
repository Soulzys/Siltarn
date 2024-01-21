#include "Weapons/Weapon.h"

DEFINE_LOG_CATEGORY(LogClass_Weapon);

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}
