// Fill out your copyright notice in the Description page of Project Settings.

#include "CI_PlayerController_CPP.h"
#include "TileMap/CI_BaseTile_CPP.h"
#include "Engine/World.h"
#include "CI_GameStateBase_CPP.h"
#include "TileMap/CI_TileMap_CPP.h"
#include "CI_Pawn_CPP.h"
#include "TileMap/CI_TileMapCoordinateMath.h"

void ACI_PlayerController_CPP::BeginPlay()
{

	Super::BeginPlay();
	
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	
}

void ACI_PlayerController_CPP::GetMouseTilePosition(int32& x, int32& y)
{
	FVector location;
	FVector direction;
	DeprojectMousePositionToWorld(location, direction);

	if (!FMath::IsNearlyEqual(location.Z, Cast<ACI_GameStateBase_CPP>(GetWorld()->GetGameState())->currentPawn->GetActorLocation().Z, 5))
	{
		location = Cast<ACI_GameStateBase_CPP>(GetWorld()->GetGameState())->currentPawn->GetActorLocation();
	}

	FVector2D tileLocation = UCI_TileMapCoordinateMath::WorldVectorToTile(location);

	x = tileLocation.X;
	y = tileLocation.Y;

	if (x < 0)
		x = 0;
	else if (x >= Cast<ACI_GameStateBase_CPP>(GetWorld()->GetGameState())->tileMap->iXTiles)
		x = Cast<ACI_GameStateBase_CPP>(GetWorld()->GetGameState())->tileMap->iXTiles - 1;
	

	if (y < 0)
		y = 0;
	else if (y >= Cast<ACI_GameStateBase_CPP>(GetWorld()->GetGameState())->tileMap->iYTiles)
		y = Cast<ACI_GameStateBase_CPP>(GetWorld()->GetGameState())->tileMap->iYTiles - 1;
}

void ACI_PlayerController_CPP::GetMouseWorldPosition(int32& x, int32& y)
{
	FVector location;
	FVector direction;
	DeprojectMousePositionToWorld(location, direction);
	x = location.X;
	y = location.Y;
}
