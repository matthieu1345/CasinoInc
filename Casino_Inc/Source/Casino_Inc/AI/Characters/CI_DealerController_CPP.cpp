// Fill out your copyright notice in the Description page of Project Settings.

#include "CI_DealerController_CPP.h"
#include "PathFinding/CI_PathFollowingComponent_CPP.h"
#include "AI/CI_AIManager_CPP.h"
#include "AI/CI_RegisterAbleInterface_CPP.h"
#include "AI/Base/CI_BaseAIPawn_CPP.h"
#include "Engine/World.h"
#include "MainGameMode/CI_GameStateBase_CPP.h"
#include "TileMap/CI_TileMap_CPP.h"
#include "TileMap/CI_InteractableTileData_CPP.h"
#include "TileMap/CI_BaseTile_CPP.h"
#include "TileMap/CI_TileMapCoordinateMath.h"
#include "TileMap/CI_BaseTile_CPP.h"

//TODO:DOCUMENT comment/document this file

ACI_DealerController_CPP::ACI_DealerController_CPP()
{
	type = ECharacterType::CT_Dealer;
	pathFollowingComp->EnableMovement();

	name = "Dealer";
}


void ACI_DealerController_CPP::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (currentTask == nullptr && isIdle && !isAway)
		GetNewTask();
}

void ACI_DealerController_CPP::BeginPlay()
{
	Super::BeginPlay();
	isAway = false;
}

void ACI_DealerController_CPP::DoIdle()
{
	Super::DoIdle();

	isIdle = true;

	if (pathFollowingComp->IsActiveState())
		return;

	auto rooms = ACI_AIManager_CPP::GetInstance(GetWorld())->GetRegisteredTile("StaffRoom");

	if (rooms.Num() == 0)
	{
		return;
	}

	auto goal = rooms[FMath::RandRange(0, rooms.Num() - 1)];

	pathFollowingComp->GetNewPath(possessedCustomBasePawn->GetTileCoordinates(), ICI_RegisterAbleInterface_CPP::Execute_GetRegisterLocation(goal));
}

void ACI_DealerController_CPP::EnterMap()
{
	Super::EnterMap();
	isIdle = true;
}

void ACI_DealerController_CPP::LeaveMap()
{
	if (taskInteractable != nullptr)
	{
		UCI_InteractableTileData_CPP* dataComp = Cast<UCI_InteractableTileData_CPP>(taskInteractable->GetDataCompRef());
		if (IsValid(dataComp))
			dataComp->DealerLeft();
	}

	Super::LeaveMap();
}

void ACI_DealerController_CPP::TaskEnded_Implementation(bool success)
{
	Super::TaskEnded_Implementation(success);
	if (!IsValid(taskInteractable))
	{
		isIdle = true;
	}
}

void ACI_DealerController_CPP::GetNewTask()
{
	Super::GetNewTask();

	if (currentTask == nullptr)
		return;

	isIdle = false;
	taskLocation = currentTask->GetLocation();
	taskInteractable = currentTask->GetTileLocation();
}
