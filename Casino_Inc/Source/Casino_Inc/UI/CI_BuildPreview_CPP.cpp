// Fill out your copyright notice in the Description page of Project Settings.

#include "CI_BuildPreview_CPP.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"
#include "PaperTileMapComponent.h"
#include "MainGameMode/CI_GameStateBase_CPP.h"
#include "TileMap/CI_BaseTileDataComponent_CPP.h"
#include "MainGameMode/CI_PlayerController_CPP.h"
#include "TileMap/CI_BaseTile_CPP.h"
#include "MainGameMode/CI_Pawn_CPP.h"
#include "AI/Tasks/BuildTask.h"
#include "TileMap/CI_TileMapCoordinateMath.h"
#include "Managers/CI_StatsManager_CPP.h"

//TODO:DOCUMENT comment/document this file

// Sets default values
ACI_BuildPreview_CPP::ACI_BuildPreview_CPP()
{

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	spriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComp"));
	spriteComponent->SetupAttachment(RootComponent);

	tilemapComponent = CreateDefaultSubobject<UPaperTileMapComponent>(TEXT("TilemapComp"));
	tilemapComponent->SetupAttachment(RootComponent);
	tilemapComponent->CreateNewTileMap(1, 1, 64, 64);

	tilemapComponent->SetRelativeLocationAndRotation(FVector(32, 32, 0), FRotator(0, 0, -90));
	spriteComponent->SetRelativeLocationAndRotation(FVector(32, 32, 0), FRotator(0, 0, -90));
}

// Called when the game starts or when spawned
void ACI_BuildPreview_CPP::BeginPlay()
{

	Super::BeginPlay();
	
	gameState = Cast<ACI_GameStateBase_CPP>(GetWorld()->GetGameState());
}

bool ACI_BuildPreview_CPP::CheckValidPlacement(int newX, int newY)
{
	if (ACI_StatsManager_CPP::GetInstance(GetWorld())->HasMoneyToBuild())
		return Cast<UCI_BaseTileDataComponent_CPP>(gameState->GetSelectedBuildObject()->GetDefaultObject())->CheckIfValidPlacement(newX, newY, Cast<UCI_BaseTileDataComponent_CPP>(gameState->GetSelectedBuildObject()->GetDefaultObject())->tileLayer, gameState->tileMap);

	return false;
}

// Called every frame
void ACI_BuildPreview_CPP::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

	if (gameState->GetCurrentMode() != EBuildInputMode::BIM_Build && gameState->GetCurrentMode() != EBuildInputMode::BIM_BuildRoom)
	{
		SetActorLocation(homelocation);
		return;
	}

	if (gameState->GetCurrentMode() == EBuildInputMode::BIM_BuildRoom)
	{
		UpdateRoomMap();
		return;
	}

	if (gameState->GetSelectedBuildObject() == NULL)
	{
		if (previousSprite || previousMap)
		{
			spriteComponent->SetVisibility(false);
			tilemapComponent->SetVisibility(false);
			previousSprite = previousMap = false;
		}

		return;
	}

	if (gameState->changedSelectedBuildObject)
		UpdatePreview();

	UpdateLocation();
}

void ACI_BuildPreview_CPP::UpdateLocation()
{
		
	if (gameState->GetSelectedBuildObject() == NULL && gameState->changedSelectedBuildObject)
	{
		SetActorLocation(homelocation);
		return;
	}

	int newX, newY;
	gameState->currentController->GetMouseTilePosition(newX, newY);

	if (newX != oldX || newY != oldY)
	{
		SetActorLocation(UCI_TileMapCoordinateMath::TileFloatsToWorldCorner(newX, newY, 50));

		if (CheckValidPlacement(newX, newY))
		{
			if (previousSprite)
			{
				spriteComponent->SetSpriteColor(FColor::White);
			}
			else
			{
				tilemapComponent->SetTileMapColor(FColor::White);
			}
		}
		else
		{
			if (previousSprite)
			{
				spriteComponent->SetSpriteColor(FColor::Red);
			}
			else
			{
				tilemapComponent->SetTileMapColor(FColor::Red);
			}
		}
	}

	oldX = newX;
	oldY = newY;
}

void ACI_BuildPreview_CPP::UpdatePreview()
{

	if (previousSprite)
	{
		if (Cast<UCI_BaseTileDataComponent_CPP>(gameState->GetSelectedBuildObject()->GetDefaultObject())->hasSprite)
		{
			UpdateSprite();
		}
		else
		{
			previousSprite = false;
			spriteComponent->SetVisibility(false);
			tilemapComponent->SetVisibility(true);
			previousMap = true;

			UpdateMap();
		}
	}
	else if (previousMap)
	{
		if (!Cast<UCI_BaseTileDataComponent_CPP>(gameState->GetSelectedBuildObject()->GetDefaultObject())->hasSprite)
		{
			UpdateMap();
		}
		else
		{
			previousMap = false;
			tilemapComponent->SetVisibility(false);
			spriteComponent->SetVisibility(true);
			previousSprite = true;

			UpdateSprite();
		}
	}
	else
	{
		if (Cast<UCI_BaseTileDataComponent_CPP>(gameState->GetSelectedBuildObject()->GetDefaultObject())->hasSprite)
		{
			spriteComponent->SetVisibility(true);
			previousSprite = true;

			UpdateSprite();
		}
		else
		{
			tilemapComponent->SetVisibility(true);
			previousMap = true;

			UpdateMap();
		}
	}

}

void ACI_BuildPreview_CPP::UpdateSprite()
{

	spriteComponent->SetSprite(Cast<UCI_BaseTileDataComponent_CPP>(gameState->GetSelectedBuildObject()->GetDefaultObject())->GetSprite());

}

void ACI_BuildPreview_CPP::UpdateMap()
{
	if (gameState->GetCurrentMode() != EBuildInputMode::BIM_BuildRoom)
	{
		auto defaultObject = Cast<UCI_BaseTileDataComponent_CPP>(gameState->GetSelectedBuildObject()->GetDefaultObject());

		tilemapComponent->ResizeMap(defaultObject->xSize, defaultObject->ySize);

		for (int x = 0; x < defaultObject->xSize; x++)
		{
			for (int y = 0; y < defaultObject->ySize; y++)
			{
				tilemapComponent->SetTile(x, y, 0, defaultObject->GetTileInfo(x, y));
			}
		}
	}
}

void ACI_BuildPreview_CPP::UpdateRoomMap()
{

	tilemapComponent->SetVisibility(true);


	int newX, newY;
	gameState->currentController->GetMouseTilePosition(newX, newY);

	if (newX == oldX && newY == oldY)
	{
		return;
	}

	oldX = newX;
	oldY = newY;

	if (gameState->isSelecting2NdCorner)
	{
		int xSize, ySize;
		xSize = FMath::Abs(gameState->selectedBuildCorner.X - newX);
		ySize = FMath::Abs(gameState->selectedBuildCorner.Y - newY);
		tilemapComponent->ResizeMap(xSize + 1, ySize + 1);



		for (int x = 0; x <= xSize; x++)
		{
			for (int y = 0; y <= ySize; y++)
			{
				if (x == 0 || x == xSize || y == 0 || y == ySize)
				{
					tilemapComponent->SetTile(x, y, 0, Cast<UCI_BaseTileDataComponent_CPP>(gameState->currentPawn->GetDefaultWall()->GetTileData()->GetDefaultObject())->GetTileInfo());
					continue;
				}

				tilemapComponent->SetTile(x, y, 0, Cast<UCI_BaseTileDataComponent_CPP>(gameState->currentPawn->GetDefaultFloor()->GetTileData()->GetDefaultObject())->GetTileInfo());
			}
		}

		int lowX, lowY;
		lowX = FMath::Min(newX, (int)gameState->selectedBuildCorner.X);
		lowY = FMath::Min(newY, (int)gameState->selectedBuildCorner.Y);

		SetActorLocation(UCI_TileMapCoordinateMath::TileFloatsToWorldCorner(lowX, lowY, 990));
	}
	else
	{
		SetActorLocation(UCI_TileMapCoordinateMath::TileFloatsToWorldCorner(newX, newY, 990));
		tilemapComponent->ResizeMap(1, 1);
		tilemapComponent->SetTile(0, 0, 0, Cast<UCI_BaseTileDataComponent_CPP>(gameState->currentPawn->GetDefaultWall()->GetTileData()->GetDefaultObject())->GetTileInfo());
	}


}
