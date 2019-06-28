#pragma once

#include "DealerTask.h"
#include "MainGameMode/CI_GameStateBase_CPP.h"
#include "TileMap/CI_TileMap_CPP.h"
#include "TileMap/CI_InteractableTileData_CPP.h"
#include "TileMap/CI_BaseTile_CPP.h"

void UDealerTask::FinishTask_Implementation(bool success, int x, int y, ACI_GameStateBase_CPP* gamestate)
{
	if (!success || !gamestate)
		return;

	ACI_BaseTile_CPP* tile;
	gamestate->tileMap->GetTileFromLayerCoordinate(x, y, ETileLayer::TL_Object, tile);

	UCI_InteractableTileData_CPP *interactableTileData = Cast<UCI_InteractableTileData_CPP>(tile->GetDataCompRef());

	if (IsValid(interactableTileData))
		interactableTileData->DealerArrived();
}

bool UDealerTask::CheckValidLocation_Implementation(int _x, int _y, ACI_GameStateBase_CPP* _gamestate)
{
	return true;
}
