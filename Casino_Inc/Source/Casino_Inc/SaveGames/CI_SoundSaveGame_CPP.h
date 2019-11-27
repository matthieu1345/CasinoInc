// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CI_SoundSaveGame_CPP.generated.h"

USTRUCT(BlueprintType)
struct FSoundLevels
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float master = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float music = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float effects = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float voice = 1.0f;
};

/**
 * 
 */
UCLASS()
class CASINO_INC_API UCI_SoundSaveGame_CPP : public USaveGame
{
	GENERATED_BODY()

public:

	UCI_SoundSaveGame_CPP();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FSoundLevels levels;

	UFUNCTION(BlueprintCallable)
	void SaveOptions();
};
