// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CI_GameInstance_CPP.generated.h"

UENUM(BlueprintType)
enum class EVolumeGroup : uint8
{
	VG_Master		UMETA(DisplayName = "Master"),
	VG_Music		UMETA(DisplayName = "Music"),
	VG_Effects		UMETA(DisplayName = "Effects"),
	VG_Voice		UMETA(DisplayName = "Voice")
};

/**
 * 
 */
UCLASS()
class CASINO_INC_API UCI_GameInstance_CPP : public UGameInstance
{
	GENERATED_BODY()

public:
	UCI_GameInstance_CPP(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void LoadSoundOptions();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class UCI_SoundSaveGame_CPP* soundSaveOptions;
};
