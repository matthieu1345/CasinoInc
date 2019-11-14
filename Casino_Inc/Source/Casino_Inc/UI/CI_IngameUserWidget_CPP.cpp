// Fill out your copyright notice in the Description page of Project Settings.

#include "CI_IngameUserWidget_CPP.h"
#include "MenuAnchor.h"
#include "DebugMacros.h"
#include "TileMap/CI_BaseTile_CPP.h"
#include "TileMap/CI_TileMap_CPP.h"
#include "MainGameMode/CI_GameStateBase_CPP.h"
#include "MainGameMode/CI_PlayerController_CPP.h"
#include "Engine/Texture2D.h"
#include "TileMap/CI_BaseTileDataComponent_CPP.h"
#include "Managers/CI_TimeManager_CPP.h"
#include "Managers/CI_StatsManager_CPP.h"
#include "Casino_Inc.h"
#include "AI/CI_AIManager_CPP.h"


void UCI_IngameUserWidget_CPP::NativeConstruct()
{
	Super::NativeConstruct();

	gameState = Cast<ACI_GameStateBase_CPP>(GetWorld()->GetGameState());
	if (gameState)
		gameState->mainUI = this;

	ACI_StatsManager_CPP::GetInstance(GetWorld())->moneyChangedDelegate.AddDynamic(this, &UCI_IngameUserWidget_CPP::MoneyChanged);
	ACI_StatsManager_CPP::GetInstance(GetWorld())->AddMoney(0);
}

void UCI_IngameUserWidget_CPP::OpenPopupMenu()
{
	if (popupAnchor == NULL)
	{
		PRINT_EDITOR(TEXT("anchor to add popup to is not set!"));
		UE_LOG(GUI, Error, TEXT("anchor to add popup to is not set!"));
		return;
	}

	if (popupAnchor->IsOpen() && popupAnchor->MenuClass == popupToCreate)
	{
		popupAnchor->Close();
	}
	else
	{
		if (popupAnchor->IsOpen())
			popupAnchor->Close();
		popupAnchor->MenuClass = popupToCreate;
		popupAnchor->Open(false);
	}
}

void UCI_IngameUserWidget_CPP::ClosePopupMenu()
{
	if (popupAnchor != NULL)
	{
		if (popupAnchor->IsOpen())
			popupAnchor->Close();
	}

}

TSubclassOf<UCI_BaseTileDataComponent_CPP> UCI_IngameUserWidget_CPP::GetSelectBuildObject()
{
	return gameState->GetSelectedBuildObject();
}

FText UCI_IngameUserWidget_CPP::GetXCoordinateText()
{
	int32 x;
	int32 y;
	gameState->currentController->GetMouseTilePosition(x, y);

	return FText::FromString("X: " + FString::FromInt(x));
}

FText UCI_IngameUserWidget_CPP::GetYCoordinateText()
{
	int32 x;
	int32 y;
	gameState->currentController->GetMouseTilePosition(x, y);

	return FText::FromString("Y: " + FString::FromInt(y));
}

FText UCI_IngameUserWidget_CPP::GetHoverName()
{
	int32 x;
	int32 y;
	gameState->currentController->GetMouseTilePosition(x, y);

	
	ACI_BaseTile_CPP* tile = Cast<ACI_GameStateBase_CPP>(GetWorld()->GetGameState())->tileMap->GetUpperTileFromCoordinate(x,y);

	if (tile != NULL)
		return FText::FromString(tile->GetTileName());

	return FText::FromString("No Tile");
}

UTexture2D* UCI_IngameUserWidget_CPP::GetHoverTilePreview()
{
	UTexture2D* output = NULL;

	int32 x;
	int32 y;
	gameState->currentController->GetMouseTilePosition(x, y);

	ACI_BaseTile_CPP* tile = gameState->tileMap->GetUpperTileFromCoordinate(x, y);
	
	if (tile != NULL)
		if (tile->GetDataCompRef() != NULL)
			output = tile->GetDataCompRef()->GetTilePreview();

	return output;

}

FText UCI_IngameUserWidget_CPP::GetSelectedBuildObjectName()
{
	if (GetSelectBuildObject() == NULL)
		return FText::FromString("No Selection");

	return FText::FromString(UCI_BaseTileDataComponent_CPP::GetTileTypeName(GetSelectBuildObject()));
}

FText UCI_IngameUserWidget_CPP::GetCurrentTimeText()
{
	FVector time = ACI_TimeManager_CPP::GetInstance(GetWorld())->GetCurrentTime();
	FString hours = FString::FromInt((int)time.Y);
	FString minutes = FString::FromInt((int)time.Z);

	if (hours.Len() == 1)
	{
		hours = "0" + hours;
	}
	if (minutes.Len() == 1)
	{
		minutes = "0" + minutes;
	}

	return FText::FromString(FString::Printf(TEXT("Day %d, %s:%s"), (int)time.X, *hours, *minutes));

	
}

void UCI_IngameUserWidget_CPP::ChangeSpeed_Implementation(ESpeedSetting newSpeed)
{
	ACI_TimeManager_CPP::GetInstance(GetWorld())->SetGameSpeed(newSpeed);
}

void UCI_IngameUserWidget_CPP::MoneyChanged(int newBalance, int newDaily, int change)
{
	//limit the balance to be shown to 999.999.999
	if (newBalance > 999999999)
		newBalance = 999999999;

	if (newDaily > 999999999)
		newDaily = 999999999;

	currentMoneyText = FText::AsNumber(newBalance);
	dailyMoneyText = FText::AsNumber(newDaily);
}

FText UCI_IngameUserWidget_CPP::GetHappinessText()
{
	return FText::FromString(FString::Printf(TEXT("%f%"), ACI_StatsManager_CPP::GetInstance(GetWorld())->GetAverageHappiness()));
}

void UCI_IngameUserWidget_CPP::CasinoOpenToggled(bool isChecked)
{
	if (isChecked)
		ACI_AIManager_CPP::GetInstance(GetWorld())->EnableGuestSpawning();
	else
		ACI_AIManager_CPP::GetInstance(GetWorld())->DisableGuestSpawning();
}

void UCI_IngameUserWidget_CPP::AICountChanged(int staff, int guests)
{
	staffCountText = FText::AsNumber(staff);
	guestCountText = FText::AsNumber(guests);
}

UTexture2D* UCI_IngameUserWidget_CPP::GetSelectedBuildObjectPreview()
{
	if (GetSelectBuildObject() == NULL)
		return NULL;

	return UCI_BaseTileDataComponent_CPP::GetTileTypePreview(GetSelectBuildObject());
}

bool UCI_IngameUserWidget_CPP::GetCornerInfoVisible()
{
	return gameState->GetCurrentMode() == EBuildInputMode::BIM_BuildRoom && gameState->isSelecting2NdCorner;
}

FText UCI_IngameUserWidget_CPP::GetCornerXText()
{
	return FText::FromString("X: " + FString::FromInt(gameState->selectedBuildCorner.X));
}

FText UCI_IngameUserWidget_CPP::GetCornerYText()
{
	return FText::FromString("Y: " + FString::FromInt(gameState->selectedBuildCorner.Y));
}

FText UCI_IngameUserWidget_CPP::GetCornerSelectText()
{
	if (gameState->GetCurrentMode() == EBuildInputMode::BIM_BuildRoom && !gameState->isSelecting2NdCorner)
		return FText::FromString("Corner 1:");
	else if (gameState->GetCurrentMode() == EBuildInputMode::BIM_BuildRoom)
		return FText::FromString("Corner 2:");
	else
		return FText();
}