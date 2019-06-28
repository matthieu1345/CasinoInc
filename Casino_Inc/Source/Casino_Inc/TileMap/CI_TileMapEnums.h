#pragma once
#include "CI_TileMapEnums.generated.h"

UENUM(BlueprintType)
enum class ETileLayer : uint8
{
	TL_Ground = 0		UMETA(DisplayName = "Ground"),
	TL_Object = 1		UMETA(DisplayName = "Object"),
	TL_Wall = 2			UMETA(DisplayName = "Wall"),
	TL_INVALID = 3		UMETA(DisplayName = "Invalid"),
	Default = TL_INVALID
};
