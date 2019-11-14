// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CI_IngamePopupMenu_CPP.generated.h"

/**
 * 
 */
UCLASS()
class CASINO_INC_API UCI_IngamePopupMenu_CPP : public UUserWidget
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
		void ExitButtonClicked();
	UFUNCTION(BlueprintCallable)
		void SelectBuildObjectClicked(class UBuildTask* tileData);
	UFUNCTION(BlueprintCallable)
		void SelectBuildRoomClicked(TSubclassOf<class UCI_BaseTileDataComponent_CPP> tileData);
	UFUNCTION(BlueprintCallable)
		void SpawnPawnClicked(TSubclassOf<APawn> pawnToSpawn);

protected:
	class ACI_GameStateBase_CPP* gameState;
};