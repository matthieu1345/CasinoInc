// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildTask.h"
#include "DebugMacros.h"
#include "MainGameMode/CI_GameStateBase_CPP.h"
#include "TileMap/CI_TileMap_CPP.h"
#include "TileMap/CI_BaseTileDataComponent_CPP.h"

void UBuildTask::InitTask_Implementation(int x, int y, ACI_GameStateBase_CPP* gamestate)
{
	Super::InitTask_Implementation(x, y, gamestate);

	CreatePreview(x, y, gamestate);

}

void UBuildTask::CreatePreview(int x, int y, class ACI_GameStateBase_CPP* gamestate)
{
	if (!gamestate)
		return;

	int xSize = Cast<UCI_BaseTileDataComponent_CPP>(tileData->GetDefaultObject())->xSize;
	int ySize = Cast<UCI_BaseTileDataComponent_CPP>(tileData->GetDefaultObject())->ySize;
	for (int _x = 0; _x < xSize; ++_x)
	{
		for (int _y = 0; _y < ySize; ++_y)
		{
			gamestate->tileMap->AddBuildPreview(x + _x, y + _y, true);
		}
	}
}

void UBuildTask::DestroyTask_Implementation(int x, int y, class ACI_GameStateBase_CPP* gamestate)
{
	Super::DestroyTask_Implementation(x, y, gamestate);
}

bool UBuildTask::CheckValidLocation_Implementation(int x, int y, ACI_GameStateBase_CPP* gamestate)
{
	if (tileData == nullptr)
		return false;

	return Cast<UCI_BaseTileDataComponent_CPP>(tileData->GetDefaultObject())->CheckIfValidPlacement(x, y, Cast<UCI_BaseTileDataComponent_CPP>(tileData->GetDefaultObject())->tileLayer, gamestate->tileMap);
}

void UBuildTask::FinishTask_Implementation(bool success, int x, int y, class ACI_GameStateBase_CPP* gamestate)
{
	Super::FinishTask_Implementation(success, x, y, gamestate);

	int xSize = Cast<UCI_BaseTileDataComponent_CPP>(tileData->GetDefaultObject())->xSize;
	int ySize = Cast<UCI_BaseTileDataComponent_CPP>(tileData->GetDefaultObject())->ySize;
	for (int _x = 0; _x < xSize; ++_x)
	{
		for (int _y = 0; _y < ySize; ++_y)
		{
			gamestate->tileMap->RemoveBuildPreview(x + _x, y + _y);
		}
	}

	if(success)
	{
		gamestate->tileMap->SetTile(x, y, tileData);
	}
}
