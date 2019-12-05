// Fill out your copyright notice in the Description page of Project Settings.

#include "CI_TileMapLayer_CPP.h"
#include "CI_TileMapChunk_CPP.h"
#include "Engine/World.h"
#include "CI_BaseTile_CPP.h"
#include "CI_TileMap_CPP.h"

//TODO:DOCUMENT comment/document this file

//********************************************************************
// constructors and unreal default functions


// Sets default values
ACI_TileMapLayer_CPP::ACI_TileMapLayer_CPP()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent->SetMobility(EComponentMobility::Static);

}

// Called when the game starts or when spawned
void ACI_TileMapLayer_CPP::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACI_TileMapLayer_CPP::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// ***********************************************************************
// layer creation functions

void ACI_TileMapLayer_CPP::SpawnChunks(int xSize, int ySize, TSubclassOf<class ACI_TileMapChunk_CPP> defaultChunk, TArray<ACI_TileMapChunk_CPP*> &allChunks, ETileLayer layerType)
{
	xChunks = xSize / ACI_TileMapChunk_CPP::CHUNK_SIZE;
	yChunks = ySize / ACI_TileMapChunk_CPP::CHUNK_SIZE;

	layer = layerType;

	ACI_TileMapChunk_CPP *newwestChunk;

	for (int y = 0; y < yChunks; y++)
	{
		for (int x = 0; x < xChunks; x++)
		{
			newwestChunk = GetWorld()->SpawnActor<ACI_TileMapChunk_CPP>(defaultChunk, FVector(y * ACI_TileMapChunk_CPP::CHUNK_SIZE * ACI_BaseTile_CPP::TILE_SIZE * -1, x * ACI_TileMapChunk_CPP::CHUNK_SIZE * ACI_BaseTile_CPP::TILE_SIZE, 0), FRotator::ZeroRotator);
			newwestChunk->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
			newwestChunk->SetChunkCoords(x, y, layer);

			layerChunks.Add(newwestChunk);
			allChunks.Add(newwestChunk);
		}
	}



}

void ACI_TileMapLayer_CPP::SpawnTiles(TArray<ACI_BaseTile_CPP*>& mapTiles, TSubclassOf<ACI_BaseTile_CPP> DefaultTile,
	ACI_TileMap_CPP* tileMap)
{
	for (ACI_TileMapChunk_CPP* chunk : layerChunks)
	{
	
		chunk->SpawnTiles(DefaultTile, tileMap, this, mapTiles, layerTiles);

	}
}

// ***********************************************************************
// layer desruction functions

void ACI_TileMapLayer_CPP::DestroyLayer()
{
	for (ACI_TileMapChunk_CPP* chunk : layerChunks)
	{
		chunk->DestroyChunk();
		chunk->Destroy();
	}

	layerChunks.Empty();
	layerTiles.Empty();
}

// ***********************************************************************************
// Layer manipulation functions

ACI_BaseTile_CPP* ACI_TileMapLayer_CPP::GetTile(int x, int y)
{
	ACI_BaseTile_CPP* output = NULL;

	output = GetChunk(x, y , false)->GetTile(x, y, false);

	return output;
}

ACI_TileMapChunk_CPP* ACI_TileMapLayer_CPP::GetChunk(int x, int y, bool isChunkCoord)
{
	if (isChunkCoord)
	{
		return layerChunks[x + y * yChunks];
	}
	else
	{
		return GetChunk(x / ACI_TileMapChunk_CPP::CHUNK_SIZE, y / ACI_TileMapChunk_CPP::CHUNK_SIZE, true);
	}
}