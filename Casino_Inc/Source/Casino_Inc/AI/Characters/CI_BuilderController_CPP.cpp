// Fill out your copyright notice in the Description page of Project Settings.

#include "CI_BuilderController_CPP.h"
#include "PathFinding/CI_PathFollowingComponent_CPP.h"
#include "AI/Base/CI_BaseAIPawn_CPP.h"
#include "AI/CI_AIManager_CPP.h"
#include "TileMap/CI_BaseTile_CPP.h"
#include "AI/CI_RegisterAbleInterface_CPP.h"

//TODO:DOCUMENT comment/document this file

ACI_BuilderController_CPP::ACI_BuilderController_CPP() : ACI_BaseStaffController_CPP()
{
	type = ECharacterType::CT_Builder;
	pathFollowingComp->EnableMovement();
	name = "Builder";
}

void ACI_BuilderController_CPP::BeginPlay()
{
	Super::BeginPlay();
}

void ACI_BuilderController_CPP::Tick(float DeltaSeconds)
{

	Super::Tick(DeltaSeconds);


	if (currentTask == nullptr && !isAway)
	{
		GetNewTask();
	}
}

void ACI_BuilderController_CPP::GetNewTask()
{
	Super::GetNewTask();
}

void ACI_BuilderController_CPP::TaskEnded_Implementation(bool success)
{
	Super::TaskEnded_Implementation(success);
	//GetNewTask();
}

void ACI_BuilderController_CPP::DoIdle()
{
	Super::DoIdle();

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
