// Fill out your copyright notice in the Description page of Project Settings.

#include "CI_TileMapCoordinateMath.h"
#include "CI_BaseTile_CPP.h"

//TODO:DOCUMENT comment/document this file

FVector2D UCI_TileMapCoordinateMath::WorldVectorToTile(const FVector input)
{
	return FVector2D(floor(input.X / ACI_BaseTile_CPP::TILE_SIZE), floor(input.Y / ACI_BaseTile_CPP::TILE_SIZE));
}

FVector2D UCI_TileMapCoordinateMath::WorldFloatsToTile(const float x, const float y)
{
	return FVector2D(floor(x / ACI_BaseTile_CPP::TILE_SIZE), floor(y / ACI_BaseTile_CPP::TILE_SIZE));
}

FVector UCI_TileMapCoordinateMath::TileVectorToWorldCorner(const FVector2D input, const float height)
{
	return FVector(input.X * ACI_BaseTile_CPP::TILE_SIZE, input.Y * ACI_BaseTile_CPP::TILE_SIZE, height);
}

FVector UCI_TileMapCoordinateMath::TileVectorToWorldCenter(const FVector2D input, const float height)
{
	return FVector(input.X * ACI_BaseTile_CPP::TILE_SIZE + ACI_BaseTile_CPP::TILE_SIZE / 2, input.Y * ACI_BaseTile_CPP::TILE_SIZE+ ACI_BaseTile_CPP::TILE_SIZE / 2, height);
}

FVector UCI_TileMapCoordinateMath::TileFloatsToWorldCorner(const float x, const float y, const float height)
{
	return FVector(x * ACI_BaseTile_CPP::TILE_SIZE, y * ACI_BaseTile_CPP::TILE_SIZE, height);
}

FVector UCI_TileMapCoordinateMath::TileFloatsToWorldCenter(const float x, const float y, const float height)
{
	return FVector(x * ACI_BaseTile_CPP::TILE_SIZE + ACI_BaseTile_CPP::TILE_SIZE / 2, y * ACI_BaseTile_CPP::TILE_SIZE + ACI_BaseTile_CPP::TILE_SIZE / 2, height);
}