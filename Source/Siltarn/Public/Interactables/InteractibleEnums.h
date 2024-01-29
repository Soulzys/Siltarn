#pragma once

#include "CoreMinimal.h"

UENUM()
enum class EItemEquipmentType : uint8
{
	Head      = 0  , 
	Chest     = 1  , 
	Pants     = 2  , 
	RightHand = 3  , 
	LeftHand  = 4  , 
	TwoHands  = 5  , 
	Neck      = 6  , 
	Shoulders = 7  , 
	Gloves    = 8  , 
	Belt      = 9  , 
	Shoes     = 10 , 
	Bag       = 11
};