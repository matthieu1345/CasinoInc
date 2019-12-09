// Fill out your copyright notice in the Description page of Project Settings.

#include "CI_TileMapChunk_CPP.h"
#include "CI_BaseTile_CPP.h"
#include "Engine/World.h"

//TODO:DOCUMENT comment/document this file

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
void ACI_TileMapChunk_CPP::Tick(const float deltaTime)
{
	Super::Tick(deltaTime);
}

bool ACI_TileMapChunk_CPP::SpawnTiles(const TSubclassOf<class ACI_BaseTile_CPP> defaultTile, class ACI_TileMap_CPP *tileMap, class ACI_TileMapLayer_CPP *mapLayer, TArray<class ACI_BaseTile_CPP*> &mapTiles, TArray<ACI_BaseTile_CPP*> &layerTiles)
{
	owningTileMap = tileMap;
	owningMapLayer = mapLayer;

	for (int y = 0; y < CHUNK_SIZE; y++)
	{
		for (int x = 0; x < CHUNK_SIZE; x++)
		{
			ACI_BaseTile_CPP *newwestTile = GetWorld()->SpawnActor<ACI_BaseTile_CPP>(defaultTile, FVector(y * ACI_BaseTile_CPP::TILE_SIZE * -1, x * ACI_BaseTile_CPP::TILE_SIZE, 0), FRotator(0, 0, 0));
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

void ACI_TileMapChunk_CPP::SetChunkCoords(const int _x, const int _y, const ETileLayer layerType)
{
	chunkX = _x;
	chunkY = _y;

	layer = layerType;

#if WITH_EDITOR
	SetActorLabel(FString::FromInt(chunkX) + TEXT("X") + FString::FromInt(chunkY) + TEXT(" chunk"));
#endif

	chunkBounds.X = chunkX * CHUNK_SIZE * ACI_BaseTile_CPP::TILE_SIZE;
	chunkBounds.Z = ((chunkX + 1) * CHUNK_SIZE - 1) * ACI_BaseTile_CPP::TILE_SIZE;
	chunkBounds.Y = chunkY * CHUNK_SIZE * ACI_BaseTile_CPP::TILE_SIZE;
	chunkBounds.W = ((chunkY + 1) * CHUNK_SIZE - 1) * ACI_BaseTile_CPP::TILE_SIZE;
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

ACI_BaseTile_CPP* ACI_TileMapChunk_CPP::GetTile(const int x, const int y, const bool isChunkCoords)
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

