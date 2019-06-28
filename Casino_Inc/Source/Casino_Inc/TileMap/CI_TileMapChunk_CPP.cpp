// Fill out your copyright notice in the Description page of Project Settings.

#include "CI_TileMapChunk_CPP.h"
#include "CI_BaseTile_CPP.h"
#include "Engine/World.h"
#include "Casino_Inc.h"
#include <string>

// Sets default values
ACI_TileMapChunk_CPP::ACI_TileMapChunk_CPP()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent->SetMobility(EComponentMobility::Static);
}



// Called when the game starts or when spawned
void ACI_TileMapChunk_CPP::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACI_TileMapChunk_CPP::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ACI_TileMapChunk_CPP::SpawnTiles(TSubclassOf<class ACI_BaseTile_CPP> DefaultTile, class ACI_TileMap_CPP *tileMap, class ACI_TileMapLayer_CPP *mapLayer, TArray<class ACI_BaseTile_CPP*> &mapTiles, TArray<ACI_BaseTile_CPP*> &layerTiles)
{
	owningTileMap = tileMap;
	owningMapLayer = mapLayer;

	ACI_BaseTile_CPP *newwestTile;

	for (int y = 0; y < CHUNK_SIZE; y++)
	{
		for (int x = 0; x < CHUNK_SIZE; x++)
		{
			newwestTile = GetWorld()->SpawnActor<ACI_BaseTile_CPP>(DefaultTile, FVector(y * ACI_BaseTile_CPP::TILE_SIZE * -1, x * ACI_BaseTile_CPP::TILE_SIZE, 0), FRotator(0, 0, 0));
			newwestTile->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
			newwestTile->SetTileCoords(chunkX * CHUNK_SIZE + x, chunkY * CHUNK_SIZE + y, layer);
			newwestTile->SetTileMapReference(tileMap, mapLayer, this);

			//add tile to arrays
			mapTiles.Add(newwestTile);
			layerTiles.Add(newwestTile);
			chunkTiles.Add(newwestTile);
		}
	}

	return true;
}

void ACI_TileMapChunk_CPP::SetChunkCoords(int X, int Y, ETileLayer layerType)
{
	chunkX = X;
	chunkY = Y;

	layer = layerType;

#if WITH_EDITOR
	SetActorLabel(FString::FromInt(chunkX) + TEXT("X") + FString::FromInt(chunkY) + TEXT(" chunk"));
#endif

	ChunkBounds.X = chunkX * CHUNK_SIZE * ACI_BaseTile_CPP::TILE_SIZE;
	ChunkBounds.Z = ((chunkX + 1) * CHUNK_SIZE - 1) * ACI_BaseTile_CPP::TILE_SIZE;
	ChunkBounds.Y = chunkY * CHUNK_SIZE * ACI_BaseTile_CPP::TILE_SIZE;
	ChunkBounds.W = ((chunkY + 1) * CHUNK_SIZE - 1) * ACI_BaseTile_CPP::TILE_SIZE;
}

void ACI_TileMapChunk_CPP::DestroyChunk()
{
	for (ACI_BaseTile_CPP* tile : chunkTiles)
	{
		tile->DestroyTile();
		tile->Destroy();
	}

	chunkTiles.Empty();

}

ACI_BaseTile_CPP* ACI_TileMapChunk_CPP::GetTile(int x, int y, bool isChunkCoords)
{
	if (isChunkCoords)
	{
		return chunkTiles[y * CHUNK_SIZE + x];
	}
	else
	{
		return GetTile(x % CHUNK_SIZE, y % CHUNK_SIZE, true);
	}
}

