// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "CI_TileMapEnums.h"
#include "PaperTileLayer.h"
#include "AI/CI_RegisterAbleInterface_CPP.h"
#include "CI_BaseTileDataComponent_CPP.generated.h"

//TODO:DOCUMENT comment/document this file

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CASINO_INC_API UCI_BaseTileDataComponent_CPP : public USceneComponent, public ICI_RegisterAbleInterface_CPP
{
GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCI_BaseTileDataComponent_CPP();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Registerable")
		FString GetRegisterName();
	virtual FString GetRegisterName_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Registerable")
	FVector2D GetRegisterLocation();
	virtual FVector2D GetRegisterLocation_Implementation() override;

	virtual void OnComponentCreated() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DestroyComponent(class ACI_TileMap_CPP *tileMap, int x, int y);
	virtual void DestroyComponent_Implementation(ACI_TileMap_CPP* tileMap, int x, int y);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		FPaperTileInfo GetTileInfo(int xOffset = 0, int yOffset = 0);

	UFUNCTION(BlueprintNativeEvent)
	void SetInfo(int x, int y, ACI_TileMap_CPP* tileMap);
	virtual void SetInfo_Implementation(int32 x, int32 y, ACI_TileMap_CPP* tileMap);

	UFUNCTION(BlueprintGetter)
		TSet<ETileLayer> GetOccupatesTile() { return occupiesTile; }

	bool CheckIfValidPlacement(int x, int y, ETileLayer layer, ACI_TileMap_CPP* map);

	FString GetTileTypeName() { return tileTypeName; }
	class UTexture2D* GetTilePreview() { return tilePreview; }
	UFUNCTION(BlueprintImplementableEvent)
		class UPaperSprite* GetSprite();

	static FString GetTileTypeName(TSubclassOf<UCI_BaseTileDataComponent_CPP> tileDataClass);
	static UTexture2D* GetTileTypePreview(TSubclassOf<UCI_BaseTileDataComponent_CPP> tileDataClass);

	bool GetWalkable() { return walkable; }

	bool GetNeedsRegister() { return needsRegistered; }

	int GetTileX() { return tileX; }
	int GetTileY() { return tileY; }
	class ACI_BaseTile_CPP* GetTile();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TArray<FPaperTileInfo> tileInfo;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	ETileLayer tileLayer = ETileLayer::TL_INVALID;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		bool hasNeighborConnection = false;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		bool isRandomTile = false;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (ClampMin = "1", UIMin = "1"))
		int xSize = 1;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (ClampMin = "1", UIMin = "1"))
		int ySize = 1;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		bool hasSprite = false;

	UPROPERTY(BlueprintReadOnly)
		bool isBeingEdited = false;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		int tileX;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		int tileY;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		ACI_TileMap_CPP* tileMap;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		TSet<ETileLayer> occupiesTile;
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	FString tileTypeName;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		UTexture2D* tilePreview;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		bool walkable;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		bool needsRegistered = false;

private:
	static const bool ALWAYS_EDIT_FLOOR = true; 
};
