// Fill out your copyright notice in the Description page of Project Settings.

#include "CI_IngamePopupMenu_CPP.h"
#include "MainGameMode/CI_GameStateBase_CPP.h"
#include "CI_IngameUserWidget_CPP.h"
#include "AI/Tasks/BuildTask.h"
#include "AI/CI_AIManager_CPP.h"

//TODO:DOCUMENT comment/document this file

void UCI_IngamePopupMenu_CPP::ExitButtonClicked()
{
	if (gameState == NULL)
		gameState = Cast<ACI_GameStateBase_CPP>(GetWorld()->GetGameState());

	if (gameState)
		gameState->mainUI->ClosePopupMenu();
}

void UCI_IngamePopupMenu_CPP::SelectBuildObjectClicked(UBuildTask* taskData)
{
	if (gameState == NULL)
		gameState = Cast<ACI_GameStateBase_CPP>(GetWorld()->GetGameState());

	if (gameState)
	{
		gameState->ChangeMode(EBuildInputMode::BIM_Build);
		gameState->SetSelectedTask(taskData);
		gameState->SetSelectedBuildObject(taskData->GetTileData());
	}

}

void UCI_IngamePopupMenu_CPP::SelectBuildRoomClicked(TSubclassOf<UCI_BaseTileDataComponent_CPP> tileData)
{
	if (gameState == NULL)
		gameState = Cast<ACI_GameStateBase_CPP>(GetWorld()->GetGameState());

	if (gameState)
	{
		gameState->ChangeMode(EBuildInputMode::BIM_Build);
		gameState->SetSelectedTask(nullptr);
		gameState->SetSelectedBuildObject(tileData);
	}

}

void UCI_IngamePopupMenu_CPP::SpawnPawnClicked(TSubclassOf<APawn> pawnToSpawn)
{

	ACI_AIManager_CPP::GetInstance(GetWorld())->SpawnAI(pawnToSpawn);

}
