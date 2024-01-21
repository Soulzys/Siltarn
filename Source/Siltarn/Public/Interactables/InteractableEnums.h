#pragma once

#include "CoreMinimal.h"

UENUM()
enum class EPickupActorRaretyType : uint8
{
	Common    = 0 , 
	Uncommon  = 1 , 
	Rare      = 2 , 
	Epic      = 3 , 
	Legendary = 4
};