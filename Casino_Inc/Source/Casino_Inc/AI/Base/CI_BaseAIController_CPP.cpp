// Fill out your copyright notice in the Description page of Project Settings.

#include "CI_BaseAIController_CPP.h"
#include "PathFinding/CI_PathFollowingComponent_CPP.h"
#include "CI_BaseAIPawn_CPP.h"
#include "AI/CI_AIManager_CPP.h"
#include "AI/GOAP/CI_GOAPWorkerComponent_CPP.h"
#include "Managers/CI_TimeManager_CPP.h"
#include "TileMap/CI_TileMapCoordinateMath.h"
#include "TimerManager.h"

ACI_BaseAIController_CPP::ACI_BaseAIController_CPP()
{
	//set default controller values
	bAttachToPawn = true;
	bIsPlayerController = false;
	IgnoreLookInput = true;
	IgnoreMoveInput = false;

	// Create the root component, and add a pathfollowing and goapWorker component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	pathFollowingComp = CreateDefaultSubobject<UCI_PathFollowingComponent_CPP>(TEXT("PathFollowingComponent"));
	AddOwnedComponent(pathFollowingComp);

	goapWorkerComponent = CreateDefaultSubobject<UCI_GOAPWorkerComponent_CPP>(TEXT("GOAPWorkerComp"));
	AddOwnedComponent(goapWorkerComponent);
}

ACI_BaseAIController_CPP::~ACI_BaseAIController_CPP()
{
}

void ACI_BaseAIController_CPP::RegisterAI()
{
	//register with the AI Manager
	ACI_AIManager_CPP::GetInstance(GetWorld())->RegisterAI(possessedCustomBasePawn, this);
}

void ACI_BaseAIController_CPP::GetNewTask()
{
	// Get a new task from the AI manager
	ACI_AIManager_CPP::GetInstance(GetWorld())->GetNextTask(type, currentTask);

	if (currentTask != nullptr) //if there's actually a task for this ai
	{
		goapWorkerComponent->StartNewPlan(currentTask); //create a plan to do the task
	}
	else
	{
		DoIdle(); // do the ai's idle behaviour
	}
}

void ACI_BaseAIController_CPP::TaskEnded_Implementation(const bool success)
{
	if (success)
		ACI_AIManager_CPP::GetInstance(GetWorld())->FinishTask(currentTask); //send to the AIManager that the task has ended succesfully and needs to be finished
	else
		ACI_AIManager_CPP::GetInstance(GetWorld())->CancelTask(currentTask); //send to the AIManager that the task has ended unsuccesful and needs to be canceled

	currentTask = nullptr; //reset the task this ai is doing to "none"
}

void ACI_BaseAIController_CPP::BeginPlay()
{
	Super::BeginPlay();

	// Register the goapWorker component's callback functions
	goapWorkerComponent->startPath.RemoveDynamic(pathFollowingComp, &UCI_PathFollowingComponent_CPP::GetNewPath);
	goapWorkerComponent->startPath.AddDynamic(pathFollowingComp, &UCI_PathFollowingComponent_CPP::GetNewPath);
	goapWorkerComponent->finishedCallback.RemoveDynamic(this, &ACI_BaseAIController_CPP::TaskEnded);
	goapWorkerComponent->finishedCallback.AddDynamic(this, &ACI_BaseAIController_CPP::TaskEnded);

	// Register the pathFollowing component's callback functions
	pathFollowingComp->pathFinishedDelegate.RemoveDynamic(goapWorkerComponent, &UCI_GOAPWorkerComponent_CPP::PathFinished);
	pathFollowingComp->pathFinishedDelegate.AddDynamic(goapWorkerComponent, &UCI_GOAPWorkerComponent_CPP::PathFinished);

	// Register the TimeManager's hourly callback function
	ACI_TimeManager_CPP::GetInstance(GetWorld())->nextHourDelegate.AddDynamic(this, &ACI_BaseAIController_CPP::HourChanged);
}

void ACI_BaseAIController_CPP::Tick(const float deltaSeconds)
{
	Super::Tick(deltaSeconds);
}

void ACI_BaseAIController_CPP::Possess(APawn* inPawn)
{
	Super::Possess(inPawn);
	
	if (inPawn->GetClass()->IsChildOf(ACI_BaseAIPawn_CPP::StaticClass())) // if the possessed pawn is a ACI_BaseAIPawn_CPP
	{
		// keep a casted reference (casting is heavy so this is to keep that to a single time)
		possessedCustomBasePawn = Cast<ACI_BaseAIPawn_CPP>(inPawn);

		// Register the goapWorker component's callback functions to the pawn
		goapWorkerComponent->stateChangedCallback.AddDynamic(possessedCustomBasePawn, &ACI_BaseAIPawn_CPP::StateChanged);

		// Register the ai to the manager in the next tick
		GetWorldTimerManager().SetTimerForNextTick(this, &ACI_BaseAIController_CPP::RegisterAI);

		// set the ai's name
		possessedCustomBasePawn->aiName = name;

		//call the "HourChanged" function once to do any initial checks based on time
		HourChanged(ACI_TimeManager_CPP::GetInstance(GetWorld())->GetCurrentTime().Y);
	}
}

void ACI_BaseAIController_CPP::UnPossess()
{
	Super::UnPossess();

	// un-register the ai to the ai manager
	if (ACI_AIManager_CPP::GetInstance(GetWorld()).IsValid())
		ACI_AIManager_CPP::GetInstance(GetWorld())->RemoveAI(possessedCustomBasePawn, type);

	possessedCustomBasePawn = nullptr;
}

void ACI_BaseAIController_CPP::EnterMap()
{
	isAway = false;
	possessedCustomBasePawn->SetActorHiddenInGame(false);
}

void ACI_BaseAIController_CPP::LeaveMap()
{
	isAway = true; // set the isAway flag

	if (goapWorkerComponent->IsActiveState()) // stop the task that the ai is currently doing
	{
		goapWorkerComponent->CancelPlan();
	}

	if (pathFollowingComp->IsActiveState()) // stop the path the ai is currently following
	{
		pathFollowingComp->CancelPath();
	}

	// Get a new path towards a leaving location
	pathFollowingComp->GetNewPath(UCI_TileMapCoordinateMath::WorldVectorToTile(possessedCustomBasePawn->GetActorLocation()), ACI_AIManager_CPP::GetInstance(GetWorld())->GetRandomSpawnTile());
}