// Fill out your copyright notice in the Description page of Project Settings.

#include "CI_BuilderController_CPP.h"
#include "PathFinding/CI_PathFollowingComponent_CPP.h"
#include "AI/Base/CI_BaseAIPawn_CPP.h"
#include "AI/CI_AIManager_CPP.h"
#include "TileMap/CI_BaseTile_CPP.h"
#include "AI/CI_RegisterAbleInterface_CPP.h"

ACI_BuilderController_CPP::ACI_BuilderController_CPP() : ACI_BaseStaffController_CPP()
{
	// set's somde defaults for the builder
	type = ECharacterType::CT_Builder;
	pathFollowingComp->EnableMovement();
	name = "Builder";
}

void ACI_BuilderController_CPP::BeginPlay()
{
	Super::BeginPlay();
}

void ACI_BuilderController_CPP::Tick(const float deltaSeconds)
{

	Super::Tick(deltaSeconds);

	// get a new task if we don't have a current task and are at work
	if (currentTask == nullptr && !isAway)
	{
		GetNewTask();
	}
}

void ACI_BuilderController_CPP::GetNewTask()
{
	Super::GetNewTask();
}

void ACI_BuilderController_CPP::TaskEnded_Implementation(const bool success)
{
	Super::TaskEnded_Implementation(success);
}

void ACI_BuilderController_CPP::DoIdle()
{
	Super::DoIdle();

	// don't do anything when we have a active path
	if (pathFollowingComp->IsActiveState())
		return;

	// get all staff rooms from the ai manager
	auto rooms = ACI_AIManager_CPP::GetInstance(GetWorld())->GetRegisteredTile("StaffRoom");

	if (rooms.Num() == 0)
	{
		return; // can't do anything when there's no staff room, so we just keep standing on our spot
	}

	// get a random room from the list to go towards
	const auto goal = rooms[FMath::RandRange(0, rooms.Num() - 1)];

	// go to the staff room
	pathFollowingComp->GetNewPath(possessedCustomBasePawn->GetTileCoordinates(), ICI_RegisterAbleInterface_CPP::Execute_GetRegisterLocation(goal));
}
