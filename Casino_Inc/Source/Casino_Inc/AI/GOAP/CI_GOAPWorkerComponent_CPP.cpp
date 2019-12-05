// Fill out your copyright notice in the Description page of Project Settings.

#include "CI_GOAPWorkerComponent_CPP.h"
#include "Actions/CI_GOAPActionBase_CPP.h"
#include "GameFramework/Actor.h"
#include "PathFinding/CI_PathFollowingComponent_CPP.h"
#include "TileMap/CI_TileMapCoordinateMath.h"
#include "Plan/CI_GOAPPlan_CPP.h"
#include "AI/Tasks/CI_BaseTask_CPP.h"
#include "DebugMacros.h"
#include "AI/CI_AIManager_CPP.h"
#include "MainGameMode/CI_GameStateBase_CPP.h"
#include "Engine/World.h"
#include "AssetRegistryModule.h"

//TODO:DOCUMENT comment/document this file

// Sets default values for this component's properties
UCI_GOAPWorkerComponent_CPP::UCI_GOAPWorkerComponent_CPP()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

void UCI_GOAPWorkerComponent_CPP::BeginDestroy()
{
	if (plan != nullptr)
	{
		plan->Stop();
	}
	
	Super::BeginDestroy();
}

// Called when the game starts
void UCI_GOAPWorkerComponent_CPP::BeginPlay()
{
	Super::BeginPlay();
}

void UCI_GOAPWorkerComponent_CPP::CancelPlan()
{
	switch (currentState)
	{

	case EGOAPWorkerState::GWS_Planning: 
		plan->Stop();

	case EGOAPWorkerState::GWS_Pathing: 
	case EGOAPWorkerState::GWS_PathingTask: 
	case EGOAPWorkerState::GWS_DoingTask:
	case EGOAPWorkerState::GWS_DoingAction:
	case EGOAPWorkerState::GWS_NextAction: 
		BroadcastFinished(false);

	case EGOAPWorkerState::GWS_FinishedPlan: 
	case EGOAPWorkerState::GWS_InvalidPlan: 
		currentState = EGOAPWorkerState::GWS_Idle;
		break;

	case EGOAPWorkerState::GWS_Idle: break;
	default: ;
	}
}

void UCI_GOAPWorkerComponent_CPP::PrintState()
{
	UE_LOG(GOAP, Log, TEXT("----------------------------------"))
	UE_LOG(GOAP, Log, TEXT("- The state of actor %s is:"), *GetOwner()->GetName())
	UGOAPStateUtil::Print(state);
	UE_LOG(GOAP, Log, TEXT("----------------------------------"))
}

void UCI_GOAPWorkerComponent_CPP::PrintPlan(bool printTested)
{
	if (plan->HasValidPlan())
		plan->LogPlan(printTested);

	plan->ReadQueue();
}

void UCI_GOAPWorkerComponent_CPP::PrintPlanState()
{
	UE_LOG(GOAP, Log, TEXT("- The goap state of actor %s is: %i"), *GetOwner()->GetName(), (int)currentState);
}

bool UCI_GOAPWorkerComponent_CPP::HasState(FGOAPStateList inState)
{
	int i;
	return UGOAPStateUtil::TestConditions(inState, state, i);
}

void UCI_GOAPWorkerComponent_CPP::StartNewPlan(struct FTask* newTask)
{
	currentTask = newTask;

	newTask->taskFinished.AddDynamic(this, &UCI_GOAPWorkerComponent_CPP::FinishedTask);

	StartPlanning();
}

void UCI_GOAPWorkerComponent_CPP::StartPlanning()
{
	if (plan != nullptr)
	{
		plan->MarkPendingKill();
		plan = nullptr;
	}

	plan = NewObject<UPlan>();
	plan->Init(state, currentTask->taskType->GetAIStateAction(), actions);

	plan->StartCalculation();

	broadcastedFinished = false;

	currentState = EGOAPWorkerState::GWS_Planning;
}

void UCI_GOAPWorkerComponent_CPP::PathFinished(bool successfull)
{
	if (!IsActiveState())
		return;

	if (successfull)
	{
		if (currentState == EGOAPWorkerState::GWS_Pathing)
			currentState = EGOAPWorkerState::GWS_DoingAction;
		else
			currentState = EGOAPWorkerState::GWS_DoingTask;
	}
	else
	{
		currentState = EGOAPWorkerState::GWS_InvalidPlan;
	}
}

// Called every frame
void UCI_GOAPWorkerComponent_CPP::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (currentState)
	{
	case EGOAPWorkerState::GWS_Planning: 
		CheckPlanningProgress();
		break;

	case EGOAPWorkerState::GWS_NextAction: 
		StartNextAction();
		break;

	case EGOAPWorkerState::GWS_Pathing: break;

	case EGOAPWorkerState::GWS_DoingAction: 
		DoAction(DeltaTime);
		break;

	case EGOAPWorkerState::GWS_PathingTask: break;

	case EGOAPWorkerState::GWS_DoingTask:
		if (currentTask != nullptr)
			currentTask->DoTask(DeltaTime, Cast<ACI_GameStateBase_CPP>(GetOwner()->GetWorld()->GetGameState()));
		break;

	case EGOAPWorkerState::GWS_FinishedPlan: 
		if (!broadcastedFinished)
		{
			broadcastedFinished = true;
			BroadcastFinished(true);
		}
		break;

	case EGOAPWorkerState::GWS_InvalidPlan: 
		if (!broadcastedFinished)
		{
			broadcastedFinished = true;
			BroadcastFinished(false);
		}
		break;

	case EGOAPWorkerState::GWS_Idle:
	default:
													// handle idle state (don't think anything is needed here tho?)
		;
	}
	
}

void UCI_GOAPWorkerComponent_CPP::StartNextAction()
{
	if (plan->StartNextAction())
	{
		if (plan->GetCurrentAction()->needsMove && startPath.IsBound())
		{
			startPath.Broadcast(UCI_TileMapCoordinateMath::WorldVectorToTile(GetOwner()->GetActorLocation()), GetActionLocation());
			currentState = EGOAPWorkerState::GWS_Pathing;
		}
		else
		{
			currentState = EGOAPWorkerState::GWS_DoingAction;
		}
	}
	else
	{
		if (startPath.IsBound())
		{
			startPath.Broadcast(UCI_TileMapCoordinateMath::WorldVectorToTile(GetOwner()->GetActorLocation()), currentTask->GetLocation());
			currentState = EGOAPWorkerState::GWS_PathingTask;
		}
		else
			currentState = EGOAPWorkerState::GWS_DoingTask;
	}
}

void UCI_GOAPWorkerComponent_CPP::CheckPlanningProgress()
{
	if (plan->ReadQueue())
	{
		if (!plan->HasValidPlan())
		{
			currentState = EGOAPWorkerState::GWS_InvalidPlan;
			return;
		}

		currentState = EGOAPWorkerState::GWS_NextAction;
	}
}

void UCI_GOAPWorkerComponent_CPP::DoAction(float DeltaTime)
{
	if (plan == nullptr)
		return;

	if (plan->DoCurrentAction(DeltaTime, state, this))
	{
		if (stateChangedCallback.IsBound())
			stateChangedCallback.Broadcast();

		currentState = EGOAPWorkerState::GWS_NextAction;
	}
}

void UCI_GOAPWorkerComponent_CPP::FinishedTask(bool successfull)
{
	currentTask->taskType->GetAIStateAction()->AddEffects(state, state, this);

	if (stateChangedCallback.IsBound())
		stateChangedCallback.Broadcast();

	currentState = EGOAPWorkerState::GWS_FinishedPlan;
}

FVector2D UCI_GOAPWorkerComponent_CPP::GetActionLocation()
{
	FVector2D actorLocation = UCI_TileMapCoordinateMath::WorldVectorToTile(GetOwner()->GetActorLocation());
	FString dataType = plan->GetCurrentAction()->registeryItem;

	return ACI_AIManager_CPP::GetInstance(GetWorld())->GetClosestItem(actorLocation, dataType);
}

void UCI_GOAPWorkerComponent_CPP::BroadcastFinished(bool success)
{
	currentTask->taskFinished.Clear();
	if (finishedCallback.IsBound())
		finishedCallback.Broadcast(success);
}

