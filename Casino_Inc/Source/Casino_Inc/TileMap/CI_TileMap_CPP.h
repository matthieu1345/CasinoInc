// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CI_TileMapEnums.h"
#include "PaperTileLayer.h"
#include "CI_TileMap_CPP.generated.h"

struct BoolArray
{
public:
	TArray<bool> values;
};

UCLASS()
class CASINO_INC_API ACI_TileMap_CPP : public AActor
{
GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACI_TileMap_CPP();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetTileSprite(int x, int y, ETileLayer layer, struct FPaperTileInfo newTile);

	UFUNCTION(BlueprintCallable, Category = "Get Tiles")
	void GetAllTilesFromCoordinate(int x, int y, UPARAM(ref) TArray<class ACI_BaseTile_CPP*> &tiles);

	UFUNCTION(BlueprintCallable, Category = "Get Tiles")
	ACI_BaseTile_CPP* GetUpperTileFromCoordinate(int x, int y);

	// get's a tile at a specific layer
	// returns true/false depending on if it already has a object on that location
	UFUNCTION(BlueprintCallable, Category="Get Tiles")
		bool GetTileFromLayerCoordinate(int x, int y, ETileLayer layer, UPARAM(ref) ACI_BaseTile_CPP*& tile);
	UFUNCTION(BlueprintCallable, Category = "Get Tiles")
		void GetAllTilesOnLayer(ETileLayer layer, UPARAM(ref) TArray<ACI_BaseTile_CPP*>& tiles);
	UFUNCTION(BlueprintCallable, Category = "Get Tiles")
		void GetChunkOnLayer(int minX, int maxX, int minY, int maxY, ETileLayer layer, UPARAM(ref) TArray<ACI_BaseTile_CPP*>& tiles);
	UFUNCTION(BlueprintCallable, Category = "Get Tiles")
		void GetRoom(int minX, int maxX, int minY, int maxY, UPARAM(ref) TArray<ACI_BaseTile_CPP*>& walls, UPARAM(ref) TArray<ACI_BaseTile_CPP*>& floors);
	UFUNCTION(BlueprintCallable, Category = "Get Tiles")
		void GetRoomOnLayer(int minX, int maxX, int minY, int maxY, ETileLayer layer, UPARAM(ref) TArray<ACI_BaseTile_CPP*>& walls, UPARAM(ref) TArray<ACI_BaseTile_CPP*>& floors);
	UFUNCTION(BlueprintCallable, Category = "Get Tiles")
		void GetChunk(int minX, int maxX, int minY, int maxY, UPARAM(ref) TArray<ACI_BaseTile_CPP*>& tiles);
	UFUNCTION(BlueprintCallable, Category = "SetTiles", meta=(DisplayName="Set Tile with Layer"))
		void SetTileWithLayer(int x, int y, ETileLayer layer, TSubclassOf<class UCI_BaseTileDataComponent_CPP> tile);
	UFUNCTION(BlueprintCallable, Category = "SetTiles", meta = (DisplayName = "Set Tile"))
		void SetTile(int x, int y, TSubclassOf<UCI_BaseTileDataComponent_CPP> tile);
	void DestroyUpperTile(int x, int y);
	void CreateDestroyUpperTask(int x, int y);

	UFUNCTION(BlueprintCallable, Category = "Edit Sprites")
	void AddTileToUpDate(ACI_BaseTile_CPP* tile) { tilesToUpdateNext.Add(tile); }

	UFUNCTION(BlueprintCallable, Category = "Edit Sprites")
	void UpdateAllTiles();

	bool CheckWalkableUncached(int x, int y);
	bool CheckWalkable(int x, int y);
	void UpdateWalkable(int x, int y);

	void AddBuildPreview(int x, int y, bool isBuild);
	void RemoveBuildPreview(int x, int y);
	void AddPriorityPreview(int x, int y);

	TArray<BoolArray> GetWalkableArray() { return walkableMap; }

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//functions for creating the map
	void SetupTilemapComponent();
	UFUNCTION(BlueprintCallable)
	void CreateMap();
	void MakeLayers();
	void MakeChunks();
	void SpawnTiles();

	UFUNCTION(BlueprintCallable)
	bool CheckDestroy();
	UFUNCTION(BlueprintCallable)
	void DestroyMap();
	UFUNCTION(BlueprintImplementableEvent)
	void DestroyWarning();

private:
	void UpdateTiles();

public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int iXTiles = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int iYTiles = 100;

	static const int LAYER_OFFSET = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<ACI_BaseTile_CPP> defaultTile;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class ACI_TileMapChunk_CPP> defaultChunk;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class ACI_TileMapLayer_CPP> defaultLayer;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UPaperTileMapComponent *tilemapComponent;


	// map components
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<ACI_TileMapLayer_CPP*> allLayers;

	UPROPERTY(BlueprintReadOnly)
	TArray<ACI_TileMapChunk_CPP*> allChunks;

	UPROPERTY(BlueprintReadOnly)
	TArray<ACI_BaseTile_CPP*> allTiles;


	class ACI_GameStateBase_CPP *gameState;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool allowDestroy = false;

	TArray<BoolArray> walkableMap;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	class UCI_BaseTask_CPP* destroyFloorTask = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		UCI_BaseTask_CPP* destroyObjectTask = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		UCI_BaseTask_CPP* destroyWallTask = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FPaperTileInfo buildPreview;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FPaperTileInfo destroyPreview;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		FPaperTileInfo priorityPreview;

private:
	TSet<ACI_BaseTile_CPP*> tilesToUpdateNext;
};
