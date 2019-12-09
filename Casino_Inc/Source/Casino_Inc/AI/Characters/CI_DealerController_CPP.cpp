// Fill out your copyright notice in the Description page of Project Settings.

#include "CI_DealerController_CPP.h"
#include "PathFinding/CI_PathFollowingComponent_CPP.h"
#include "AI/CI_AIManager_CPP.h"
#include "AI/CI_RegisterAbleInterface_CPP.h"
#include "AI/Base/CI_BaseAIPawn_CPP.h"
#include "Engine/World.h"
#include "TileMap/CI_InteractableTileData_CPP.h"
#include "TileMap/CI_BaseTile_CPP.h"
#include "TileMap/CI_TileMapCoordinateMath.h"

ACI_DealerController_CPP::ACI_DealerController_CPP()
{
	// set dealer specific default variables
	type = ECharacterType::CT_Dealer;
	pathFollowingComp->EnableMovement();
	name = "Dealer";
}


void ACI_DealerController_CPP::Tick(const float deltaSeconds)
{
	Super::Tick(deltaSeconds);

	//if we don't have a task, are idle, and are at the casino, ask for a new task
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

	// set idle flag
	isIdle = true;

	if (pathFollowingComp->IsActiveState())
		return; // don't do anything if we're moving already

	// get all staff rooms from the ai manager
	auto rooms = ACI_AIManager_CPP::GetInstance(GetWorld())->GetRegisteredTile("StaffRoom");

	if (rooms.Num() == 0)
	{
		return; // can't do anything when there's no staff room, so we just keep standing on our spot
	}

	// select a random staff room to go to
	const auto goal = rooms[FMath::RandRange(0, rooms.Num() - 1)];

	// get a path to the selected staff room
	pathFollowingComp->GetNewPath(possessedCustomBasePawn->GetTileCoordinates(), ICI_RegisterAbleInterface_CPP::Execute_GetRegisterLocation(goal));
}

void ACI_DealerController_CPP::EnterMap()
{
	Super::EnterMap();
	isIdle = true;
}

void ACI_DealerController_CPP::LeaveMap()
{

	// tell the interactable that he needs a new dealer
	if (taskInteractable != nullptr)
	{
		UCI_InteractableTileData_CPP* dataComp = Cast<UCI_InteractableTileData_CPP>(taskInteractable->GetDataCompRef());
		if (IsValid(dataComp))
			dataComp->DealerLeft();
	}

	Super::LeaveMap();
}

void ACI_DealerController_CPP::TaskEnded_Implementation(const bool success)
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
		return; // don't do anything if we haven't gotten a new task

	// set references for the new task
	isIdle = false;
	taskLocation = currentTask->GetLocation();
	taskInteractable = currentTask->GetTileLocation();
}
