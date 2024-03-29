#pragma once

#include "CoreMinimal.h"

UENUM()
enum class EInventoryItemWidgetLocation : uint8
{
	PLAYER_INVENTORY   = 0,
	EQUIPPED           = 1,
	EXTERNAL_INVENTORY = 2, 
	UNKNOWN            = 3
};