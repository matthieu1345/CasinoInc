// Fill out your copyright notice in the Description page of Project Settings.

#include "CI_SoundSaveGame_CPP.h"
#include "Kismet/GameplayStatics.h"

UCI_SoundSaveGame_CPP::UCI_SoundSaveGame_CPP()
{
}

void UCI_SoundSaveGame_CPP::SaveOptions()
{
	UGameplayStatics::SaveGameToSlot(this, TEXT("SoundLevels"), 0);
}
