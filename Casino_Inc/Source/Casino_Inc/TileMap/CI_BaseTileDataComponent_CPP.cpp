// Fill out your copyright notice in the Description page of Project Settings.

#include "CI_BaseTileDataComponent_CPP.h"
#include "CI_BaseTile_CPP.h"
#include "CI_TileMap_CPP.h"
#include "Casino_Inc.h"
#include "Engine/World.h"
#include "AI/CI_AIManager_CPP.h"
#include "DebugMacros.h"

// Sets default values for this component's properties
UCI_BaseTileDataComponent_CPP::UCI_BaseTileDataComponent_CPP()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

FString UCI_BaseTileDataComponent_CPP::GetRegisterName_Implementation()
{
	return GetTileTypeName();
}

FVector2D UCI_BaseTileDataComponent_CPP::GetRegisterLocation_Implementation()
{
	return FVector2D(tileX, tileY);
}

void UCI_BaseTileDataComponent_CPP::OnComponentCreated()
{
	Super::OnComponentCreated();

	if (needsRegistered)
	{
		ACI_AIManager_CPP::GetInstance(GetWorld())->RegisterObject(this);
	}
}

void UCI_BaseTileDataComponent_CPP::DestroyComponent_Implementation(ACI_TileMap_CPP *tileMap, int _x, int _y)
{
	for (USceneComponent* const attachChild : GetAttachChildren())
	{
		attachChild->DestroyComponent();
	}

	for (int x = 1; x <= xSize; ++x)
	{
		for (int y = 1; y <= ySize; ++y)
		{
			ACI_BaseTile_CPP* tiles;
			tileMap->GetTileFromLayerCoordinate(tileX + x - 1,tileY + y - 1, tileLayer, tiles);

			if (tiles->GetDataCompRef() != this)
			{
				UE_LOG(TileMap, Warning, TEXT("trying to remove a multi size tile to a occupied space"));

				continue;
			}

			tiles->SetDataComponentRef(nullptr);

				
			tiles->UpdateSprite();
		}
	}


	if (needsRegistered)
		ACI_AIManager_CPP::GetInstance(GetWorld())->DeRegisterObject(GetRegisterName(), this);

	UnregisterComponent();
}

void UCI_BaseTileDataComponent_CPP::SetInfo_Implementation(int _x, int _y, ACI_TileMap_CPP* tileMap)
{
	tileX = _x;
	tileY = _y;
	this->tileMap = tileMap;

	if (xSize > 1 || ySize > 1)
	{
		for (int x = 1; x <= xSize; ++x)
		{
			for (int y = 1; y <= ySize; ++y)
			{
				if (x == 1 && y == 1)
					continue;


				TArray<ACI_BaseTile_CPP*> tiles;
				tileMap->GetAllTilesFromCoordinate(tileX + x - 1, tileY + y - 1, tiles);

				ACI_BaseTile_CPP* usedTile = tiles[(int)tileLayer];

				if (!usedTile->SetDataComponentRef(this))
				{
					UE_LOG(TileMap, Warning, TEXT("trying to add a multi size tile to a occupied space"));

					continue;
				}

				usedTile->UpdateSprite();
			}
		}
	}


}

bool UCI_BaseTileDataComponent_CPP::CheckIfValidPlacement(int _x, int _y, ETileLayer layer, ACI_TileMap_CPP* map)
{
	if (ALWAYS_EDIT_FLOOR && layer == ETileLayer::TL_Ground)
		return true;

	bool valid = true;

	for (int x = 1; x <= xSize; ++x)
	{
		for (int y = 1; y <= ySize; ++y)
		{
			TArray<ACI_BaseTile_CPP*> tiles;
			map->GetAllTilesFromCoordinate(_x + x - 1, _y + y - 1, tiles);


			for (ACI_BaseTile_CPP* usedTile : tiles)
			{
				if (usedTile->GetDataCompRef() != NULL)
				{
					for (auto occupiedLayer : occupiesTile)
					{
						if (usedTile->GetDataCompRef()->GetOccupatesTile().Contains(occupiedLayer))
						{
							return false;
						}
					}
				}
			}
		}
	}

	return true;
}

FString UCI_BaseTileDataComponent_CPP::GetTileTypeName(TSubclassOf<UCI_BaseTileDataComponent_CPP> tileDataClass)
{
	return Cast<UCI_BaseTileDataComponent_CPP>(tileDataClass->GetDefaultObject())->GetTileTypeName();

}

UTexture2D* UCI_BaseTileDataComponent_CPP::GetTileTypePreview(TSubclassOf<UCI_BaseTileDataComponent_CPP> tileDataClass)
{
	return Cast<UCI_BaseTileDataComponent_CPP>(tileDataClass->GetDefaultObject())->GetTilePreview();
}

ACI_BaseTile_CPP* UCI_BaseTileDataComponent_CPP::GetTile()
{
	ACI_BaseTile_CPP* output;
	tileMap->GetTileFromLayerCoordinate(tileX, tileY, tileLayer, output);
	return output;
}

// Called when the game starts
void UCI_BaseTileDataComponent_CPP::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCI_BaseTileDataComponent_CPP::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FPaperTileInfo UCI_BaseTileDataComponent_CPP::GetTileInfo_Implementation(int xOffset = 0, int yOffset = 0)
{
	int index = FMath::RandRange(0, tileInfo.Num() - 1);

	if (!isRandomTile)
	{
		index = xOffset + yOffset * xSize;
	}

	return tileInfo[index];
}

