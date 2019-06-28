#pragma once
#include "CoreMinimal.h"
#include "CI_AITypes.generated.h"

//NOTE: if adding more make sure to change CI_AIManager_CPP.cpp :: SpawnAIInternal
UENUM(BlueprintType)
enum class ECharacterType : uint8
{
	CT_Builder			UMETA(DisplayName = "Builder"),
	CT_Guest			UMETA(DisplayName = "Guest"),
	CT_Dealer			UMETA(DisplayName = "Dealer"),
	CT_Invalid			UMETA(DisplayName = "INVALID")
};