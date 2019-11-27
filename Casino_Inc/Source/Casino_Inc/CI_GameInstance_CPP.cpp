// Fill out your copyright notice in the Description page of Project Settings.

#include "CI_GameInstance_CPP.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGames/CI_SoundSaveGame_CPP.h"

UCI_GameInstance_CPP::UCI_GameInstance_CPP(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UCI_GameInstance_CPP::LoadSoundOptions()
{
	if (!UGameplayStatics::DoesSaveGameExist(TEXT("SoundLevels"), 0))
	{
		if (!IsValid(soundSaveOptions))
		{
			soundSaveOptions = Cast<UCI_SoundSaveGame_CPP>(UGameplayStatics::CreateSaveGameObject(UCI_SoundSaveGame_CPP::StaticClass()));
		}
	}
	else
	{
		soundSaveOptions = Cast<UCI_SoundSaveGame_CPP>(UGameplayStatics::LoadGameFromSlot(TEXT("SoundLevels"), 0));
	}
}
