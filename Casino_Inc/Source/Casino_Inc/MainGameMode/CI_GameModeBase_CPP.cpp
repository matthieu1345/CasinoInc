// Fill out your copyright notice in the Description page of Project Settings.

#include "CI_GameModeBase_CPP.h"
#include "CI_Pawn_CPP.h"
#include "CI_GameStateBase_CPP.h"
#include "CI_HUD_CPP.h"
#include "CI_PlayerController_CPP.h"
#include "Engine/Classes/Engine/World.h"
#include "UserWidget.h"
#include "DebugMacros.h"
#include "Casino_Inc.h"
#include "PathFinding/CI_Path.h"
#include "AI/GOAP/Plan/CI_GOAPPlan_CPP.h"
#include "UI/CI_IngameUserWidget_CPP.h"
#include "Managers/CI_StatsManager_CPP.h"
#include "GameFramework/WorldSettings.h"

//TODO:DOCUMENT comment/document this file

ACI_GameModeBase_CPP::ACI_GameModeBase_CPP()
{
	DefaultPawnClass = ACI_Pawn_CPP::StaticClass();
	HUDClass = ACI_HUD_CPP::StaticClass();
	GameStateClass = ACI_GameStateBase_CPP::StaticClass();
	PlayerControllerClass = ACI_PlayerController_CPP::StaticClass();
}

void ACI_GameModeBase_CPP::BeginPlay()
{
	Super::BeginPlay();

	currentGameState = Cast<ACI_GameStateBase_CPP>(GetWorld()->GetGameState());
	currentGameState->currentController = Cast<ACI_PlayerController_CPP>(GetWorld()->GetFirstPlayerController());
	currentGameState->currentPawn = Cast<ACI_Pawn_CPP>(currentGameState->currentController->GetPawn());

	if (defaultWidgetClass != nullptr)
		ChangeMenuWidget(defaultWidgetClass);
}

void ACI_GameModeBase_CPP::ChangeMenuWidget(const TSubclassOf<UUserWidget> newWidgetClass)
{

	if (currentWidget != nullptr)
	{
		currentWidget->RemoveFromViewport();
		currentWidget = nullptr;
	}

	if (newWidgetClass != nullptr)
	{
		currentWidget = CreateWidget<UUserWidget>(GetWorld(), newWidgetClass);

		if (currentWidget != nullptr)
		{
			currentWidget->AddToViewport();
		}
	}
}

void ACI_GameModeBase_CPP::Logout(AController* Exiting)
{
	FCalculatePathTask::Shutdown();
	FCalculatePlanTask::ShutDown();

	if (LOG_PATHFINDING_TIMES)
	{
		PRINT(*FString::Printf(TEXT("amount of paths found quickly: %d"), pathFindingQuick));
		PRINT(*FString::Printf(TEXT("amount of paths found medium time: %d"), pathFindingMedium));
		PRINT(*FString::Printf(TEXT("amount of paths found long time: %d"), pathFindingLong));

		UE_LOG(PathFinding, Log, TEXT("amount of paths found quickly: %d"), pathFindingQuick);
		UE_LOG(PathFinding, Log, TEXT("amount of paths found medium time: %d"), pathFindingMedium);
		UE_LOG(PathFinding, Log, TEXT("amount of paths found long time: %d"), pathFindingLong);
	}
}

void ACI_GameModeBase_CPP::SpeedUp()
{
	GetWorldSettings()->SetTimeDilation(GetWorldSettings()->GetEffectiveTimeDilation() * 2);
}

void ACI_GameModeBase_CPP::SlowDown()
{
	GetWorldSettings()->SetTimeDilation(GetWorldSettings()->GetEffectiveTimeDilation() * 0.5);
}

void ACI_GameModeBase_CPP::ResetSpeed()
{
	GetWorldSettings()->SetTimeDilation(1.0f);
}

void ACI_GameModeBase_CPP::AddMoney()
{
	ACI_StatsManager_CPP::GetInstance(GetWorld())->AddMoney(100);
}

const float ACI_GameModeBase_CPP::CHARACTER_LAYER = 500.0f;
