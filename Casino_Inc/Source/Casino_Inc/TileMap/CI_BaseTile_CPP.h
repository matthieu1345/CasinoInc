// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperTileLayer.h"
#include "CI_TileMapEnums.h"
#include "CI_BaseTile_CPP.generated.h"

//TODO:DOCUMENT comment/document this file

//class UPaperSpriteComponent;


UCLASS()
class CASINO_INC_API ACI_BaseTile_CPP : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACI_BaseTile_CPP();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetTileCoords(int X, int Y, ETileLayer layerType);

	void SetTileMapReference(class ACI_TileMap_CPP* tileMap, class ACI_TileMapLayer_CPP *mapLayer, class ACI_TileMapChunk_CPP *mapChunk)
	{
		owningTileMap = tileMap;
		owningLayer = mapLayer;
		owningChunk = mapChunk;
	}

	void DestroyTile();

	UFUNCTION(BlueprintCallable)
		void UpdateSprite() const;
	UFUNCTION(BlueprintCallable)
		void AddSprite();
	UFUNCTION(BlueprintCallable)
		void RemoveSprite();
	UFUNCTION(BlueprintCallable)
		void ChangeTileType(TSubclassOf<class UCI_BaseTileDataComponent_CPP> newData);
	UFUNCTION(BlueprintCallable)
		void MarkIsBeingEdited(bool newEditMode);

	bool SetDataComponentRef(UCI_BaseTileDataComponent_CPP* newData);

	UCI_BaseTileDataComponent_CPP* GetDataCompRef()
	{
		return tileDataComponent;
	}

	UFUNCTION(BlueprintCallable, Category="Tile Type")
	bool HasDataComponent();

	UFUNCTION(BlueprintCallable)
		FString GetTileName();

	ETileLayer GetTileLayer() { return tileLayer; }

	bool GetWalkable();

	UFUNCTION(BlueprintCallable, Category = "Coordinates")
	FVector GetWorldCoordinate(float z = 0)
	{
		return FVector(tileX * TILE_SIZE + TILE_SIZE / 2, tileY * TILE_SIZE + TILE_SIZE / 2, z);
	}
	UFUNCTION(BlueprintCallable, Category = "Coordinates")
	FVector2D GetTileCoordinates()
	{
		return FVector2D(tileX, tileY);
	}

	int GetX() { return tileX; }
	int GetY() { return tileY; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UFUNCTION(BlueprintCallable)
		void UpdateTileData();

private:
	void UpdateRegister(FString oldDataType);

	//Variables
public:

	static const int TILE_SIZE = 64;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		int tileX;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		int tileY;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		ACI_TileMap_CPP *owningTileMap;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		ACI_TileMapLayer_CPP *owningLayer;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		ACI_TileMapChunk_CPP *owningChunk;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		ETileLayer tileLayer = ETileLayer::TL_INVALID;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		UCI_BaseTileDataComponent_CPP *tileDataComponent = NULL;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<UCI_BaseTileDataComponent_CPP> newTileDataComponent;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class UPaperSpriteComponent* spriteComponent;

	private:
		bool isRegistered = false;
};
