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
	bAttachToPawn = true;
	bIsPlayerController = false;
	IgnoreLookInput = true;
	IgnoreMoveInput = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	pathFollowingComp = CreateDefaultSubobject<UCI_PathFollowingComponent_CPP>(TEXT("PathFollowingComponent"));
	AddOwnedComponent(pathFollowingComp);

	goapWorkerComponent = CreateDefaultSubobject<UCI_GOAPWorkerComponent_CPP>(TEXT("GOAPWorkerComp"));
	AddOwnedComponent(goapWorkerComponent);
}

ACI_BaseAIController_CPP::~ACI_BaseAIController_CPP()
{
	/*if (IsValid(pathFollowingComp))
		pathFollowingComp->DestroyComponent();
	if (IsValid(goapWorkerComponent))
		goapWorkerComponent->DestroyComponent*/
}

void ACI_BaseAIController_CPP::RegisterAI()
{
	ACI_AIManager_CPP::GetInstance(GetWorld())->RegisterAI(possessedCustomBasePawn, this);
}

void ACI_BaseAIController_CPP::GetNewTask()
{
	ACI_AIManager_CPP::GetInstance(GetWorld())->GetNextTask(type, currentTask);

	if (currentTask != nullptr)
	{
		goapWorkerComponent->StartNewPlan(currentTask);
	}
	else
	{
		DoIdle();
	}
}

void ACI_BaseAIController_CPP::TaskEnded_Implementation(bool success)
{
	if (success)
		ACI_AIManager_CPP::GetInstance(GetWorld())->FinishTask(currentTask);
	else
		ACI_AIManager_CPP::GetInstance(GetWorld())->CancelTask(currentTask);

	currentTask = nullptr;
}

void ACI_BaseAIController_CPP::BeginPlay()
{
	Super::BeginPlay();

	goapWorkerComponent->startPath.RemoveDynamic(pathFollowingComp, &UCI_PathFollowingComponent_CPP::GetNewPath);
	goapWorkerComponent->startPath.AddDynamic(pathFollowingComp, &UCI_PathFollowingComponent_CPP::GetNewPath);
	goapWorkerComponent->finishedCallback.RemoveDynamic(this, &ACI_BaseAIController_CPP::TaskEnded);
	goapWorkerComponent->finishedCallback.AddDynamic(this, &ACI_BaseAIController_CPP::TaskEnded);

	pathFollowingComp->pathFinishedDelegate.RemoveDynamic(goapWorkerComponent, &UCI_GOAPWorkerComponent_CPP::PathFinished);
	pathFollowingComp->pathFinishedDelegate.AddDynamic(goapWorkerComponent, &UCI_GOAPWorkerComponent_CPP::PathFinished);

	ACI_TimeManager_CPP::GetInstance(GetWorld())->nextHourDelegate.AddDynamic(this, &ACI_BaseAIController_CPP::HourChanged);
}

void ACI_BaseAIController_CPP::Tick(float DeltaSeconds) 
{
	Super::Tick(DeltaSeconds);
}

void ACI_BaseAIController_CPP::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);


	if (InPawn->GetClass()->IsChildOf(ACI_BaseAIPawn_CPP::StaticClass()))
	{
		possessedCustomBasePawn = Cast<ACI_BaseAIPawn_CPP>(InPawn);

		goapWorkerComponent->stateChangedCallback.AddDynamic(possessedCustomBasePawn, &ACI_BaseAIPawn_CPP::StateChanged);
		GetWorldTimerManager().SetTimerForNextTick(this, &ACI_BaseAIController_CPP::RegisterAI);

		possessedCustomBasePawn->aiName = name;

		HourChanged(ACI_TimeManager_CPP::GetInstance(GetWorld())->GetCurrentTime().Y);
	}
}

void ACI_BaseAIController_CPP::UnPossess()
{
	Super::UnPossess();

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
	isAway = true;

	if (goapWorkerComponent->IsActiveState())
	{
		goapWorkerComponent->CancelPlan();
	}

	if (pathFollowingComp->IsActiveState())
	{
		pathFollowingComp->CancelPath();
	}

	pathFollowingComp->GetNewPath(UCI_TileMapCoordinateMath::WorldVectorToTile(possessedCustomBasePawn->GetActorLocation()), ACI_AIManager_CPP::GetInstance(GetWorld())->GetRandomSpawnTile());
}