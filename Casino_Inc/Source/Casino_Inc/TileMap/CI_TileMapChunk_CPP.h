// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CI_TileMapEnums.h"
#include "CI_TileMapChunk_CPP.generated.h"


//TODO:DOCUMENT comment/document this file

// A class for deviding the tile objects into chunks of CHUNK_SIZE
UCLASS()
class CASINO_INC_API ACI_TileMapChunk_CPP : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACI_TileMapChunk_CPP();

	// Called every frame
	virtual void Tick(float deltaTime) override;

	bool SpawnTiles(TSubclassOf<class ACI_BaseTile_CPP> defaultTile, class ACI_TileMap_CPP *tileMap, class ACI_TileMapLayer_CPP *mapLayer, TArray<class ACI_BaseTile_CPP*> &mapTiles, TArray<ACI_BaseTile_CPP*> &layerTiles);

	void SetChunkCoords(int _x, int _y, ETileLayer layerType);
	FVector2D GetChunkCoords() const { return FVector2D(chunkX, chunkY); }
	ACI_BaseTile_CPP* GetTile(int x, int y, bool isChunkCoords);

	/**
	* get's the bounds of the Chunk\n
	* X = min X\n
	* Y = min Y\n
	* Z = max X\n
	* W = max Y
	*/
	FVector4 GetChunkBounds() const { return chunkBounds; }

	void DestroyChunk();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	static const int CHUNK_SIZE = 5;

protected:

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		ACI_TileMap_CPP *owningTileMap;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		ACI_TileMapLayer_CPP *owningMapLayer;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		TArray<ACI_BaseTile_CPP*> chunkTiles;

	int chunkX;
	int chunkY;

	/**
	* X = min X\n
	* Y = min Y\n
	* Z = max X\n
	* W = max Y
	*/
	FVector4 chunkBounds;
	ETileLayer layer;
};
