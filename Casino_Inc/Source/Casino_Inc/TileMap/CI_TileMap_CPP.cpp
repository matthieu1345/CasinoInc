// Fill out your copyright notice in the Description page of Project Settings.

#include "CI_TileMap_CPP.h"
#include "CI_BaseTile_CPP.h"
#include "CI_TileMapChunk_CPP.h"
#include "Casino_Inc.h"
#include "PaperTileMapComponent.h"
#include "Engine/Classes/Engine/World.h"
#include "MainGameMode/CI_GameStateBase_CPP.h"
#include "CI_TileMapLayer_CPP.h"
#include <algorithm>
#include "CI_BaseTileDataComponent_CPP.h"
#include "AI/CI_AIManager_CPP.h"

//TODO:DOCUMENT comment/document this file

//********************************************************************
// constructors

// Sets default values
ACI_TileMap_CPP::ACI_TileMap_CPP()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent->SetMobility(EComponentMobility::Static);
	tilemapComponent = CreateDefaultSubobject<UPaperTileMapComponent>(TEXT("Tilemap Renderer"));
	tilemapComponent->SetupAttachment(RootComponent);
}

void ACI_TileMap_CPP::SetupTilemapComponent() const
{
	tilemapComponent->SetRelativeRotation(FRotator(0, 90, -90));
	tilemapComponent->SetRelativeLocation(FVector(-(ACI_BaseTile_CPP::TILE_SIZE / 2), ACI_BaseTile_CPP::TILE_SIZE / 2, 0));
}

void ACI_TileMap_CPP::OnConstruction(const FTransform& transform)
{
	Super::OnConstruction(transform);

	SetupTilemapComponent();
}

// Called when the game starts or when spawned
void ACI_TileMap_CPP::BeginPlay()
{
	Super::BeginPlay();
	
	gameState = Cast<ACI_GameStateBase_CPP>(GetWorld()->GetGameState());
	if (gameState)
		gameState->tileMap = this;

	if (allLayers.Num() == 0)
		CreateMap();

	walkableMap.SetNum(iXTiles);

	for (int x = 0; x < iXTiles; ++x)
	{
		walkableMap[x].values.Init(false, iYTiles);

		for (int y = 0; y < iYTiles; ++y)
		{
			walkableMap[x].values[y] = CheckWalkableUncached(x, y);
		}
	}
	
}

// Called every frame
void ACI_TileMap_CPP::Tick(const float deltaTime)
{
	Super::Tick(deltaTime);


	if (tilesToUpdateNext.Num() > 0)
	{
		UpdateTiles();
	}
}

// ***********************************************************************
// map creation functions

void ACI_TileMap_CPP::CreateMap()
{
	if (defaultTile == nullptr || defaultChunk == nullptr || defaultLayer == nullptr)
	{
		UE_LOG(TileMap, Error, TEXT("Tilemap %s has no default tile, chunk, or layer set!"), *GetNameSafe(this));
		return;
	}

	if (allLayers.Num() > 0)
	{
		if (CheckDestroy())
		{
			DestroyMap();
		}
		else
		{
			return;
		}
	}

	MakeLayers();
	MakeChunks();
	SpawnTiles();
}

void ACI_TileMap_CPP::MakeLayers()
{
	for (uint8 i = 0; i < (uint8)ETileLayer::TL_INVALID; i++)
	{
		ACI_TileMapLayer_CPP *newwestLayer = GetWorld()->SpawnActor<ACI_TileMapLayer_CPP>(defaultLayer, FVector(0, 0, i * LAYER_OFFSET), FRotator::ZeroRotator);
		newwestLayer->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		
		// give the layer a nice name in the world outline
		FString newName;
		switch ((ETileLayer)i)
		{
		case ETileLayer::TL_Ground:
			newName = "Ground Layer";
			break;

		case ETileLayer::TL_Object:
			newName = "Object Layer";
			break;

		case ETileLayer::TL_Wall:
			newName = "Wall Layer";
			break;

		case ETileLayer::TL_INVALID:
		default:
			newName = "INVALID LAYER";
			break;
		}
#if WITH_EDITOR
		newwestLayer->SetActorLabel(newName);
#endif

		allLayers.Add(newwestLayer);

	}
}

void ACI_TileMap_CPP::MakeChunks()
{
	for (int i = 0; i < (uint8)ETileLayer::TL_INVALID; i++)
	{
		allLayers[i]->SpawnChunks(iXTiles, iYTiles, defaultChunk, allChunks, (ETileLayer)i);
	}
}

void ACI_TileMap_CPP::SpawnTiles()
{
	for (ACI_TileMapLayer_CPP* layer : allLayers)
	{
		layer->SpawnTiles(allTiles, defaultTile, this);
	}
}

void ACI_TileMap_CPP::UpdateAllTiles()
{
	for (ACI_BaseTile_CPP* tile : allTiles)
	{
		tile->UpdateSprite();
	}
}

bool ACI_TileMap_CPP::CheckWalkableUncached(const int x, const int y)
{
	if (x < 0 || x >= iXTiles || y < 0 || y >= iYTiles)
		return false;

	bool output = true;

	TArray<ACI_BaseTile_CPP*> tiles;
	GetAllTilesFromCoordinate(x, y, tiles);

	for (ACI_BaseTile_CPP* tile : tiles)
	{
		if (!tile->GetWalkable())
		{
			output = false;
		}
	}

	return output;

}

bool ACI_TileMap_CPP::CheckWalkable(const int x, const int y)
{
	if (x < 0 || x >= iXTiles || y < 0 || y >= iYTiles)
		return false;

	if (x < walkableMap.Num())
		if (y < walkableMap[x].values.Num())
			return walkableMap[x].values[y];

	return false;
}

void ACI_TileMap_CPP::UpdateWalkable(const int x, const int y)
{
	if (x < walkableMap.Num())
		if (y < walkableMap[x].values.Num())
			walkableMap[x].values[y] = CheckWalkableUncached(x, y);
}

// ***********************************************************************************
// Map destroy functions

void ACI_TileMap_CPP::DestroyMap()
{
	if (!CheckDestroy())
		return;

	allowDestroy = false;

	for (ACI_TileMapLayer_CPP* layer : allLayers)
	{
		layer->DestroyLayer();
		layer->Destroy();
	}
	
	allLayers.Empty();
	allChunks.Empty();
	allTiles.Empty();
}

bool ACI_TileMap_CPP::CheckDestroy()
{
	if (allowDestroy)
	{
		return true;
	}
	else
	{
		DestroyWarning();
		return false;
	}
}

// ***********************************************************************************
// Map manipulation functions

void ACI_TileMap_CPP::SetTileSprite(const int x, const int y, ETileLayer layer, const FPaperTileInfo newTile) const
{
	if (layer == ETileLayer::TL_INVALID)
	{
		UE_LOG(TileMap, Warning, TEXT("%s tried to update %d X %d for the invalid layer!"), *GetNameSafe(this), x, y);
		return;
	}


	tilemapComponent->SetTile(x, y, (uint8)layer, newTile);
}

void ACI_TileMap_CPP::UpdateTiles()
{

	TSet<ACI_BaseTile_CPP*> updateTiles ;
	TSet<ACI_BaseTile_CPP*> difTiles = tilesToUpdateNext;
	tilesToUpdateNext.Empty();

	do
	{
		updateTiles.Append(difTiles);

		for (ACI_BaseTile_CPP* tile : difTiles)
		{
			tile->UpdateSprite();
		}

		difTiles = tilesToUpdateNext.Difference(updateTiles);
		tilesToUpdateNext.Empty();

	} while (difTiles.Num() != 0);

}

void ACI_TileMap_CPP::SetTileWithLayer(const int x, const int y, const ETileLayer layer, const TSubclassOf<UCI_BaseTileDataComponent_CPP> tileData)
{
	ACI_BaseTile_CPP* tile;
	GetTileFromLayerCoordinate(x, y, layer, tile);

	tile->ChangeTileType(tileData);

}

void ACI_TileMap_CPP::SetTile(const int x, const int y, TSubclassOf<UCI_BaseTileDataComponent_CPP> tileData)
{
	SetTileWithLayer(x, y, Cast<UCI_BaseTileDataComponent_CPP>(tileData->GetDefaultObject())->tileLayer, tileData);
}

void ACI_TileMap_CPP::DestroyUpperTile(const int x, const int y)
{
	ACI_BaseTile_CPP* tile = GetUpperTileFromCoordinate(x, y);

	if (tile->GetTileLayer() != ETileLayer::TL_Ground)
	{
		tile->GetDataCompRef()->DestroyComponent(this, x, y);
		tile->ChangeTileType(nullptr);
	}
}

void ACI_TileMap_CPP::CreateDestroyUpperTask(const int x, const int y)
{

	if (!ACI_AIManager_CPP::GetInstance(GetWorld()).IsValid())
		return;

	ACI_BaseTile_CPP* tile = GetUpperTileFromCoordinate(x, y);

	switch (tile->GetTileLayer())
	{
	case ETileLayer::TL_Ground:
		ACI_AIManager_CPP::GetInstance(GetWorld()).Get()->CreateNewTask(destroyFloorTask, x, y);
		break;

	case ETileLayer::TL_Object:
		ACI_AIManager_CPP::GetInstance(GetWorld()).Get()->CreateNewTask(destroyObjectTask, x, y);
		break;

	case ETileLayer::TL_Wall:
		ACI_AIManager_CPP::GetInstance(GetWorld()).Get()->CreateNewTask(destroyWallTask, x, y);
		break;

	case ETileLayer::TL_INVALID:
	default:
		break;
	}

	//set it so the ai can reach the node
	walkableMap[x].values[y] = true;
	tile->MarkIsBeingEdited(true);
}


// ***********************************************************************************
// Map selection functions

void ACI_TileMap_CPP::GetAllTilesFromCoordinate(const int x, const int y, TArray<ACI_BaseTile_CPP*>& tiles)
{
	tiles.Empty();

	if (x < 0 || x >= iXTiles || y < 0 || y >= iYTiles)
		return;


	for (ACI_TileMapLayer_CPP* layer : allLayers)
	{

		tiles.Add(layer->GetTile(x, y));

	}
}

ACI_BaseTile_CPP* ACI_TileMap_CPP::GetUpperTileFromCoordinate(const int x, const int y) const
{
	TArray<ACI_BaseTile_CPP*> tiles;
	Cast<ACI_GameStateBase_CPP>(GetWorld()->GetGameState())->tileMap->GetAllTilesFromCoordinate(x, y, tiles);

	if (tiles.Num() == 0)
		return nullptr;

	for (int i = (int)ETileLayer::TL_INVALID - 1; i >= 0; --i)
	{
		if (tiles[i]->GetDataCompRef() != nullptr)
		{
			return tiles[i];
		}
	}

	return nullptr;

}

bool ACI_TileMap_CPP::GetTileFromLayerCoordinate(const int x, const int y, ETileLayer layer, ACI_BaseTile_CPP* &tile)
{
	TArray<ACI_BaseTile_CPP*> _allTiles;
	GetAllTilesFromCoordinate(x, y, _allTiles);

	tile = _allTiles[(int)layer];

	return tile->HasDataComponent();
}

void ACI_TileMap_CPP::GetAllTilesOnLayer(ETileLayer layer, TArray<ACI_BaseTile_CPP*> &tiles)
{
	if (layer == ETileLayer::TL_INVALID)
		return;
	tiles = allLayers[(int)layer]->GetAllTiles();
}

void ACI_TileMap_CPP::GetChunkOnLayer(const int minX, const int maxX, const int minY, const int maxY, const ETileLayer layer, TArray<ACI_BaseTile_CPP*> &tiles)
{
	GetRoomOnLayer(minX, maxX, minY, maxY, layer, tiles, tiles);
}

void ACI_TileMap_CPP::GetRoom(const int minX, const int maxX, const int minY, const int maxY, TArray<ACI_BaseTile_CPP*>& walls,
	TArray<ACI_BaseTile_CPP*>& floors)
{
	for (int i = 0; i < (int)ETileLayer::TL_INVALID; i++)
	{

		GetRoomOnLayer(minX, maxX, minY, maxY, (ETileLayer)i, walls, floors);

	}
}

void ACI_TileMap_CPP::GetRoomOnLayer(const int minX, const int maxX, const int minY, const int maxY, const ETileLayer layer, TArray<ACI_BaseTile_CPP*> &walls, TArray<ACI_BaseTile_CPP*> &floors)
{
	ACI_BaseTile_CPP* tempTile;
	for (int y = minY; y < maxY; y++)
	{
		for (int x = minX; x < maxX; x++)
		{
			GetTileFromLayerCoordinate(x, y, layer, tempTile);

			if (x == minX || x == maxX || y == minY || y == maxY)
				walls.Add(tempTile);
			else
				floors.Add(tempTile);
		}
	}
}

void ACI_TileMap_CPP::GetChunk(const int minX, const int maxX, const int minY, const int maxY, TArray<ACI_BaseTile_CPP*> &tiles)
{
	for (int i = 0; i < (int)ETileLayer::TL_INVALID; i++)
	{
		GetChunkOnLayer(minX, maxX, minY, maxY, (ETileLayer)i, tiles);
	}
}

// ***********************************************************************************
// Preview functions

void ACI_TileMap_CPP::AddBuildPreview(const int x, const int y, const bool isBuild) const
{
	if (isBuild)
		tilemapComponent->SetTile(x, y, 3, buildPreview);
	else
	{
		tilemapComponent->SetTile(x, y, 3, destroyPreview);
	}
}

void ACI_TileMap_CPP::RemoveBuildPreview(const int x, const int y) const
{
	tilemapComponent->SetTile(x, y, 3, FPaperTileInfo());
}

void ACI_TileMap_CPP::AddPriorityPreview(const int x, const int y) const
{
	tilemapComponent->SetTile(x, y, 3, priorityPreview);
}
