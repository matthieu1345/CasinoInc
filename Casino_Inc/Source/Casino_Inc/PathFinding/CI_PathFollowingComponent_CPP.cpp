// Fill out your copyright notice in the Description page of Project Settings.

#include "CI_PathFollowingComponent_CPP.h"
#include "Engine/World.h"
#include "TileMap/CI_TileMap_CPP.h"
#include "MainGameMode/CI_GameStateBase_CPP.h"
#include "CI_Path.h"
#include "MainGameMode/CI_GameModeBase_CPP.h"
#include "Casino_Inc.h"
#include "TileMap/CI_TileMapCoordinateMath.h"
#include "DebugMacros.h"
#include "TileMap/CI_BaseTile_CPP.h"


#if DEBUG_PATH > 0
#include "DrawDebugHelpers.h"
#endif

// Sets default values for this component's properties
UCI_PathFollowingComponent_CPP::UCI_PathFollowingComponent_CPP()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UCI_PathFollowingComponent_CPP::BeginDestroy()
{
	if (path != nullptr)
	{
		path->Stop();
		path = nullptr;
	}

	Super::BeginDestroy();
}

void UCI_PathFollowingComponent_CPP::CancelPath()
{
	switch (currentState)
	{
	
	case EPathFollowingState::PFS_Calculating:
		path->Stop();

	case EPathFollowingState::PFS_Moving:
	case EPathFollowingState::PFS_ReachedNode:
	case EPathFollowingState::PFS_ReachedGoal:
	case EPathFollowingState::PFS_InvalidPath: 
		currentState = EPathFollowingState::PFS_Idle;

		if (pathFinishedDelegate.IsBound())
			pathFinishedDelegate.Broadcast(false);

		break;

	case EPathFollowingState::PFS_Idle:
	default: ;
	}
}

void UCI_PathFollowingComponent_CPP::PrintPathState()
{
	UE_LOG(PathFinding, Log, TEXT("- The Pathing state of actor %s is: %i"), *GetOwner()->GetName(), (int)currentState);
}

// Called when the game starts
void UCI_PathFollowingComponent_CPP::BeginPlay()
{
	Super::BeginPlay();

	GetOwner()->SetActorLocation(FVector(GetOwner()->GetActorLocation().X, GetOwner()->GetActorLocation().Y, ACI_GameModeBase_CPP::CHARACTER_LAYER));
	
	gameState = Cast<ACI_GameStateBase_CPP>(GetOwner()->GetWorld()->GetGameState());
	tileMap = gameState->tileMap;
}

// Called every frame
void UCI_PathFollowingComponent_CPP::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	LastTickMovement = FVector::ZeroVector;

	switch (currentState)
	{
	case EPathFollowingState::PFS_Calculating: 
		if (path->ReadQueue())
		{
			PathCalculationFinished(path->outputData->isComplete);
		}
		break;

	case EPathFollowingState::PFS_Moving: 
		DoMovement(DeltaTime);

#if DEBUG_PATH > 0
		DrawPathDebug();
#endif
		break;

	case EPathFollowingState::PFS_ReachedNode: break;

	case EPathFollowingState::PFS_ReachedGoal: 
		if (!broadcastedFinished)
		{
			broadcastedFinished = true;
			if (pathFinishedDelegate.IsBound())
				pathFinishedDelegate.Broadcast(true);
		}
		break;

	case EPathFollowingState::PFS_InvalidPath: 
		if (!broadcastedFinished)
		{
			broadcastedFinished = true;
			if (pathFinishedDelegate.IsBound())
				pathFinishedDelegate.Broadcast(false);
		}
		break;

	case EPathFollowingState::PFS_Idle: 
	default: ;
	}
}

double UCI_PathFollowingComponent_CPP::LastCalcTime() const
{
	if (currentState != EPathFollowingState::PFS_Calculating)
		return lastCalcEnd - lastCalcStart;
	else
		return FPlatformTime::ToMilliseconds(FPlatformTime::Cycles()) - lastCalcStart;

}

void UCI_PathFollowingComponent_CPP::GetNewPath(FVector2D start, FVector2D end)
{
	start.X = (int)start.X;
	start.Y = (int)start.Y;

	if (start.Equals(end, 0.5f))
		return;

	this->start = start;
	this->end = end;
	broadcastedFinished = false;
	currentState = EPathFollowingState::PFS_Calculating;

#if PROFILE_PATHFINDING_LEVEL > 0
	lastCalcStart = FPlatformTime::ToMilliseconds(FPlatformTime::Cycles());
#endif

	end.X = (int)end.X;
	end.Y = (int)end.Y;

	if (path == nullptr)
		path = NewObject<UPath>();
	path->Init(start, end, maxReach, tileMap);

	path->StartCaluclatePath();

}

void UCI_PathFollowingComponent_CPP::PathCalculationFinished(bool completePath)
{
#if PROFILE_PATHFINDING_LEVEL > 0
	lastCalcEnd = FPlatformTime::ToMilliseconds(FPlatformTime::Cycles());


#if PROFILE_PATHFINDING_LEVEL == 1
	{
		UE_LOG(PathFinding, Verbose, TEXT("Finished a %s path from %dx%d to %dx%d!"), *FString("incomplete"), (int)start.X, (int)start.Y, (int)end.X, (int)end.X)
	}
#elif PROFILE_PATHFINDING_LEVEL > 1
	{
		UE_LOG(PathFinding, Verbose, TEXT("Finished a %s path from %dx%d to %dx%d, time taken: %f miliseconds"), completePath ? *FString("complete") : *FString("incomplete"), (int)start.X, (int)start.Y, (int)end.X, (int)end.X, LastCalcTime())
	}
#endif


	// ReSharper disable All			        This code gives quite a lot of inspection warnings on "printColor" because of it's debug use	
	FColor printColor = FColor::Red;
	if (LastCalcTime() < 1000)
	{
		printColor = FColor::Green;

		Cast<ACI_GameModeBase_CPP>(GetWorld()->GetAuthGameMode())->pathFindingQuick++;
	}
	else if (LastCalcTime() < 3000)
	{
		printColor = FColor::Yellow;
		Cast<ACI_GameModeBase_CPP>(GetWorld()->GetAuthGameMode())->pathFindingMedium++;
	}
	else
	{

#if PROFILE_PATHFINDING_LEVEL > 1
		UE_LOG(PathFinding, Warning, TEXT("Finished a %s path from %dx%d to %dx%d, time taken: %f miliseconds"), completePath ? *FString("complete") : *FString("incomplete"), (int)start.X, (int)start.Y, (int)end.X, (int)end.X, LastCalcTime())
#endif

		printColor = FColor::Red;
		Cast<ACI_GameModeBase_CPP>(GetWorld()->GetAuthGameMode())->pathFindingLong++;
	}

#if PROFILE_PATHFINDING_LEVEL > 2
	PRINT_COLOR(printColor ,*FString::Printf(TEXT("Finished a %s path from %dx%d to %dx%d, time taken: %f miliseconds"), completePath ? *FString("complete") : *FString("incomplete"), (int)start.X, (int)start.Y, (int)end.X, (int)end.X, LastCalcTime()));
#endif
	// ReSharper restore All
#endif

	if (completePath)
	{
		if (path->outputData->path.Num() > 0)
		{
			nextGoalNode = path->outputData->path[0];
			currentState = EPathFollowingState::PFS_Moving;
		}
		else
		{
			currentState = EPathFollowingState::PFS_InvalidPath;
		}
	}
	else
	{
		currentState = EPathFollowingState::PFS_InvalidPath;
	}
}

void UCI_PathFollowingComponent_CPP::DoMovement(const float deltaTime)
{
	if (nextGoalNode == nullptr)
		return;

	FVector goal = UCI_TileMapCoordinateMath::TileVectorToWorldCenter(nextGoalNode->location, ACI_GameModeBase_CPP::CHARACTER_LAYER);

	if (nextGoalNode->isEnd)
	{
		float* temp = new float(9.0f);
		float xValue = FMath::Modf(end.X, temp);
		float yValue = FMath::Modf(end.Y, temp);
		delete temp;

		goal += FVector(xValue * ACI_BaseTile_CPP::TILE_SIZE, yValue * ACI_BaseTile_CPP::TILE_SIZE, 0);
	}

	FVector movementDirection = goal - GetOwner()->GetActorLocation();
	if (!movementDirection.IsNormalized())
		movementDirection.Normalize();

	movementDirection = movementDirection * maxSpeed;
	
	MoveActor(movementDirection * deltaTime);

	CheckGoalNodeReached();
}

void UCI_PathFollowingComponent_CPP::CheckGoalNodeReached()
{
	float distance = FVector::DistSquared2D(UCI_TileMapCoordinateMath::TileVectorToWorldCenter(nextGoalNode->location, ACI_GameModeBase_CPP::CHARACTER_LAYER), GetOwner()->GetActorLocation());
	if (distance <= LastTickMovement.SizeSquared2D())
	{
		currentState = EPathFollowingState::PFS_ReachedNode;

		if (nextGoalNode->child != nullptr)
		{
			
			if (!tileMap->CheckWalkable(nextGoalNode->child->location.X, nextGoalNode->child->location.Y) && !nextGoalNode->child->isEnd)
			{
				GetNewPath(nextGoalNode->location, end);
				return;
			}

			nextGoalNode = nextGoalNode->child;
			currentState = EPathFollowingState::PFS_Moving;
		}
		else
		{
			currentState = EPathFollowingState::PFS_ReachedGoal;
		}
	}
}

void UCI_PathFollowingComponent_CPP::MoveActor(FVector input)
{
	
	LastTickMovement = input;

	if (GetOwner()->GetClass()->IsChildOf(AController::StaticClass()))
	{
		Cast<AController>(GetOwner())->GetPawn()->AddActorWorldOffset(input);
		Cast<AController>(GetOwner())->GetPawn()->SetActorLocation(FVector(GetOwner()->GetActorLocation().X, GetOwner()->GetActorLocation().Y, ACI_GameModeBase_CPP::CHARACTER_LAYER));
	}
	else
	{

		GetOwner()->AddActorWorldOffset(input);

		GetOwner()->SetActorLocation(FVector(GetOwner()->GetActorLocation().X, GetOwner()->GetActorLocation().Y, ACI_GameModeBase_CPP::CHARACTER_LAYER));
	}
	
}

#if DEBUG_PATH > 0
void UCI_PathFollowingComponent_CPP::DrawPathDebug()
{

#if DEBUG_PATH == 2
	if (debugLevel >= 2)
	{
		FPathNode* temp = nextGoalNode;


		while (temp->child != nullptr)
		{
			DrawDebugLine(
				GetWorld(),
				UCI_TileMapCoordinateMath::TileVectorToWorldCenter(temp->location, ACI_GameModeBase_CPP::CHARACTER_LAYER),
				UCI_TileMapCoordinateMath::TileVectorToWorldCenter(temp->child->location, ACI_GameModeBase_CPP::CHARACTER_LAYER),
				debugColor,
				false, -1, 2,
				12.333
			);

			temp = temp->child;
		}
	}
#endif
	if (debugLevel >= 1)
	{
		if (nextGoalNode != nullptr && nextGoalNode->parent != nullptr)
			DrawDebugLine(
				GetWorld(),
				UCI_TileMapCoordinateMath::TileVectorToWorldCenter(nextGoalNode->location, ACI_GameModeBase_CPP::CHARACTER_LAYER),
				UCI_TileMapCoordinateMath::TileVectorToWorldCenter(nextGoalNode->parent->location, ACI_GameModeBase_CPP::CHARACTER_LAYER),
				FColor(0, 255,0 ),
				false, -1, 1,
				12.333
			);
	}
}
#endif