// Fill out your copyright notice in the Description page of Project Settings.

#include "DestroyTask.h"
#include "TileMap/CI_TileMap_CPP.h"
#include "MainGameMode/CI_GameStateBase_CPP.h"
#include "TileMap/CI_BaseTile_CPP.h"
#include "TileMap/CI_BaseTileDataComponent_CPP.h"

//TODO:DOCUMENT comment/document this file

void UDestroyTask::InitTask_Implementation(const int x, const int y, ACI_GameStateBase_CPP* gamestate)
{
	Super::InitTask_Implementation(x, y, gamestate);

	CreatePreview(x, y, gamestate);

}

void UDestroyTask::CreatePreview(const int x, const int y, class ACI_GameStateBase_CPP* gamestate)
{
	if (!gamestate)
		return;

	ACI_BaseTile_CPP* tile = nullptr;
	gamestate->tileMap->GetTileFromLayerCoordinate(x, y, tileLayer, tile);
	for (int _x = 0; _x < tile->GetDataCompRef()->xSize; ++_x)
	{
		for (int _y = 0; _y < tile->GetDataCompRef()->ySize; ++_y)
		{
			gamestate->tileMap->AddBuildPreview(x + _x, y + _y, false);
		}
	}
		
}

void UDestroyTask::DestroyTask_Implementation(const int x, const int y, class ACI_GameStateBase_CPP* gamestate)
{
	Super::DestroyTask_Implementation(x, y, gamestate);
}

bool UDestroyTask::CheckValidLocation_Implementation(const int x, const int y, ACI_GameStateBase_CPP* gamestate)
{
	ACI_BaseTile_CPP* tile = nullptr;
	gamestate->tileMap->GetTileFromLayerCoordinate(x, y, tileLayer, tile);
	if (tile->GetDataCompRef())
		return !tile->GetDataCompRef()->isBeingEdited;

	return false;
}

void UDestroyTask::FinishTask_Implementation(const bool success, const int x, const int y, class ACI_GameStateBase_CPP* gamestate)
{
	Super::FinishTask_Implementation(success, x, y, gamestate);

	if (!gamestate)
		return;

	ACI_BaseTile_CPP* tile = nullptr;
	gamestate->tileMap->GetTileFromLayerCoordinate(x, y, tileLayer, tile);
	for (int _x = 0; _x < tile->GetDataCompRef()->xSize; ++_x)
	{
		for (int _y = 0; _y < tile->GetDataCompRef()->ySize; ++_y)
		{
			gamestate->tileMap->RemoveBuildPreview(x + _x, y + _y);
		}
	}

	if (success)
	{
		gamestate->tileMap->SetTileWithLayer(x, y, tileLayer, nullptr);
	}
}
