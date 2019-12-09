// Fill out your copyright notice in the Description page of Project Settings.

#include "CI_BaseTile_CPP.h"
#include "Casino_Inc.h"
#include "PaperTileMapComponent.h"
#include "PaperSpriteComponent.h"
#include "CI_TileMap_CPP.h"
#include "CI_BaseTileDataComponent_CPP.h"
#include "DebugMacros.h"
#include "AI/CI_AIManager_CPP.h"

//TODO:DOCUMENT comment/document this file

// Sets default values
ACI_BaseTile_CPP::ACI_BaseTile_CPP()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent->SetMobility(EComponentMobility::Static);
}

bool ACI_BaseTile_CPP::HasDataComponent() const
{
	if (tileDataComponent == nullptr)
		return false;
	if (tileDataComponent->IsBeingDestroyed())
		return false;

	return true;
}

FString ACI_BaseTile_CPP::GetTileName() const
{
	return tileDataComponent->GetTileTypeName();
}

bool ACI_BaseTile_CPP::GetWalkable() const
{
	if (tileLayer == ETileLayer::TL_Ground || tileDataComponent == nullptr)
		return true;

	return tileDataComponent->GetWalkable();

}

// Called when the game starts or when spawned
void ACI_BaseTile_CPP::BeginPlay()
{
	Super::BeginPlay();
	
	if (tileLayer == ETileLayer::TL_INVALID)
	{
		UE_LOG(TileMap, Error, TEXT("%s has an invalid layer Type!"), *this->GetClass()->GetName());
	}
}

void ACI_BaseTile_CPP::UpdateSprite() const
{
	if (tileDataComponent != nullptr)
		owningTileMap->SetTileSprite(tileX, tileY, tileDataComponent->tileLayer, tileDataComponent->GetTileInfo(tileX - tileDataComponent->GetTileX(), tileY - tileDataComponent->GetTileY()));
	else
		owningTileMap->SetTileSprite(tileX, tileY, tileLayer, FPaperTileInfo());
}

void ACI_BaseTile_CPP::UpdateTileData()
{
	// FString oldDataType;
	// if (tileDataComponent != nullptr)
	//	oldDataType = ICI_RegisterAbleInterface_CPP::Execute_GetRegisterName(tileDataComponent);

	if (newTileDataComponent == nullptr)
	{
		if (tileDataComponent != nullptr)
		{
			tileDataComponent->DestroyComponent(owningTileMap, tileX, tileY);
			tileDataComponent = nullptr;
		}
		//UpdateRegister(oldDataType);
		owningTileMap->UpdateWalkable(tileX, tileY);
		return;
	}

	if (Cast<UCI_BaseTileDataComponent_CPP>(newTileDataComponent->GetDefaultObject())->tileLayer != tileLayer)
	{
		PRINT_EDITOR("Trying to add the wrong tile type to the wrong layer!");
		UE_LOG(TileMap, Warning, TEXT("Trying to add the wrong tile type to the wrong layer! coords: %d, %d"), tileX, tileY);
		//UpdateRegister(oldDataType);
		return;
	}

	if (!Cast<UCI_BaseTileDataComponent_CPP>(newTileDataComponent->GetDefaultObject())->CheckIfValidPlacement(tileX, tileY, tileLayer, owningTileMap))
	{
		PRINT_EDITOR("tile will not fit in this location!");
		UE_LOG(TileMap, VeryVerbose, TEXT("Trying to set an occupied tile! coords: %d, %d"), tileX, tileY);
		//UpdateRegister(oldDataType);
		return;
	}

	if (tileDataComponent != nullptr)
	{
		tileDataComponent->DestroyComponent(owningTileMap, tileX, tileY);
		tileDataComponent = nullptr;
	}

	tileDataComponent = NewObject<UCI_BaseTileDataComponent_CPP>(this, newTileDataComponent);
	tileDataComponent->SetupAttachment(RootComponent);
	tileDataComponent->RegisterComponent();

	if (tileDataComponent->hasSprite)
		AddSprite();

	tileDataComponent->SetInfo(tileX, tileY, owningTileMap);
	owningTileMap->UpdateWalkable(tileX, tileY);

	UpdateSprite();
	//UpdateRegister(oldDataType);
}

void ACI_BaseTile_CPP::UpdateRegister(const FString oldDataType)
{
	if (isRegistered)
	{
		ACI_AIManager_CPP::GetInstance(GetWorld())->DeRegisterObject(oldDataType, this);
		isRegistered = false;
	}

	if (GetDataCompRef() == nullptr)
		return;

	if (GetDataCompRef()->GetNeedsRegister())
	{
		ACI_AIManager_CPP::GetInstance(GetWorld())->RegisterObject(GetDataCompRef()->GetRegisterName(), this);
		isRegistered = true;
	}
}

// Called every frame
void ACI_BaseTile_CPP::Tick(const float deltaTime)
{
	Super::Tick(deltaTime);

}

void ACI_BaseTile_CPP::SetTileCoords(const int _x, const int _y, const ETileLayer layerType)
{
	tileX = _x;
	tileY = _y;

	tileLayer = layerType;
#if WITH_EDITOR
	SetActorLabel(FString::FromInt(tileX) + "X" + FString::FromInt(tileY) + " " + GetActorLabel());
#endif
}


void ACI_BaseTile_CPP::DestroyTile() const
{

	if (tileDataComponent != nullptr)
	{
		tileDataComponent->DestroyComponent(owningTileMap, tileX, tileY);
	}

}

void ACI_BaseTile_CPP::AddSprite()
{

	if (spriteComponent != nullptr)
		return;

	spriteComponent = NewObject<UPaperSpriteComponent>(this, UPaperSpriteComponent::StaticClass());
	spriteComponent->RegisterComponent();
	spriteComponent->SetupAttachment(tileDataComponent);

	FVector location = GetActorLocation();
	location += FVector(TILE_SIZE / 2, TILE_SIZE / 2, 0);
	FRotator rotation = GetActorRotation();
	rotation += FRotator(0, 90, -90);

	spriteComponent->SetWorldLocationAndRotation(location, rotation);
}

void ACI_BaseTile_CPP::RemoveSprite()
{

	if (spriteComponent == nullptr)
		return;

	spriteComponent->DestroyComponent();
	spriteComponent = nullptr;
}

void ACI_BaseTile_CPP::ChangeTileType(const TSubclassOf<UCI_BaseTileDataComponent_CPP> newData)
{
	newTileDataComponent = newData;
	UpdateTileData();
}

void ACI_BaseTile_CPP::MarkIsBeingEdited(const bool newEditMode) const
{

	tileDataComponent->isBeingEdited = newEditMode;

	owningTileMap->UpdateWalkable(tileX, tileY);
}

bool ACI_BaseTile_CPP::SetDataComponentRef(UCI_BaseTileDataComponent_CPP* newData)
{
	tileDataComponent = newData;
	owningTileMap->UpdateWalkable(tileX, tileY);
	return true;
}
