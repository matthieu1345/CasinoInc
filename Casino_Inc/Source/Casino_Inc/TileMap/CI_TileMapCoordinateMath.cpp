// Fill out your copyright notice in the Description page of Project Settings.

#include "CI_TileMapCoordinateMath.h"
#include "CI_BaseTile_CPP.h"

//TODO:DOCUMENT comment/document this file

FVector2D UCI_TileMapCoordinateMath::WorldVectorToTile(FVector input)
{
	return FVector2D(floor(input.X / ACI_BaseTile_CPP::TILE_SIZE), floor(input.Y / ACI_BaseTile_CPP::TILE_SIZE));
}

FVector2D UCI_TileMapCoordinateMath::WorldFloatsToTile(float x, float y)
{
	return FVector2D(floor(x / ACI_BaseTile_CPP::TILE_SIZE), floor(y / ACI_BaseTile_CPP::TILE_SIZE));
}

FVector UCI_TileMapCoordinateMath::TileVectorToWorldCorner(FVector2D input, float height)
{
	return FVector(input.X * ACI_BaseTile_CPP::TILE_SIZE, input.Y * ACI_BaseTile_CPP::TILE_SIZE, height);
}

FVector UCI_TileMapCoordinateMath::TileVectorToWorldCenter(FVector2D input, float height)
{
	return FVector(input.X * ACI_BaseTile_CPP::TILE_SIZE + ACI_BaseTile_CPP::TILE_SIZE / 2, input.Y * ACI_BaseTile_CPP::TILE_SIZE+ ACI_BaseTile_CPP::TILE_SIZE / 2, height);
}

FVector UCI_TileMapCoordinateMath::TileFloatsToWorldCorner(float x, float y, float height)
{
	return FVector(x * ACI_BaseTile_CPP::TILE_SIZE, y * ACI_BaseTile_CPP::TILE_SIZE, height);
}

FVector UCI_TileMapCoordinateMath::TileFloatsToWorldCenter(float x, float y, float height)
{
	return FVector(x * ACI_BaseTile_CPP::TILE_SIZE + ACI_BaseTile_CPP::TILE_SIZE / 2, y * ACI_BaseTile_CPP::TILE_SIZE + ACI_BaseTile_CPP::TILE_SIZE / 2, height);
}