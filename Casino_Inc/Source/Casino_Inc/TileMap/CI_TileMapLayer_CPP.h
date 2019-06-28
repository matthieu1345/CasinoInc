// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CI_TileMapEnums.h"
#include "CI_TileMapLayer_CPP.generated.h"

// A class for deviding the tile objects into the different layers
UCLASS()
class CASINO_INC_API ACI_TileMapLayer_CPP : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACI_TileMapLayer_CPP();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnChunks(int xSize, int ySize, TSubclassOf<class ACI_TileMapChunk_CPP> defaultChunk, TArray<class ACI_TileMapChunk_CPP*>& allChunks, ETileLayer
	                 layerType);
	void SpawnTiles(TArray<class ACI_BaseTile_CPP*> &mapTiles, TSubclassOf<class ACI_BaseTile_CPP> DefaultTile, class ACI_TileMap_CPP *tileMap);
	
	void SetLayer(ETileLayer newLayer) { layer = newLayer; }
	ETileLayer GetLayer() { return layer; }
	class ACI_BaseTile_CPP* GetTile(int x, int y);
	class ACI_TileMapChunk_CPP* GetChunk(int x, int y, bool isChunkCoord);
	TArray<ACI_BaseTile_CPP*> GetAllTiles() { return layerTiles; }

	void DestroyLayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class ACI_TileMap_CPP* owningTileMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<class ACI_TileMapChunk_CPP*> layerChunks;
	UPROPERTY(BlueprintReadOnly)
		TArray<ACI_BaseTile_CPP*> layerTiles;

	ETileLayer layer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int xChunks;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int yChunks;

};
