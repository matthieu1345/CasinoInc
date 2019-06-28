// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CI_GOAPStateContainer_CPP.h"
#include "CI_GOAPWorkerComponent_CPP.generated.h"

UENUM(BlueprintType)
enum class EGOAPWorkerState : uint8
{
	GWS_Idle			UMETA(DisplayName = "Idle"),
	GWS_Planning		UMETA(DisplayName = "Planning"),
	GWS_NextAction		UMETA(DisplayName = "Getting Next Action"),
	GWS_Pathing			UMETA(DisplayName = "Pathing"),
	GWS_DoingAction		UMETA(DisplayName = "Doing Action"),
	GWS_PathingTask		UMETA(DisplayName = "Pathing to Task"),
	GWS_DoingTask		UMETA(DisplayName = "Doing Task"),
	GWS_FinishedPlan	UMETA(DisplayName = "Finished Plan"),
	GWS_InvalidPlan		UMETA(DisplayName = "Invalid Plan")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStartPathing, FVector2D, start, FVector2D, end);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlanFinished, bool, success);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStateChanged);

UCLASS( ClassGroup=(AI), meta=(BlueprintSpawnableComponent) )
class CASINO_INC_API UCI_GOAPWorkerComponent_CPP : public UActorComponent
{
GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCI_GOAPWorkerComponent_CPP();
	void BeginDestroy() override;
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void StartNewPlan(struct FTask* newTask);

	UFUNCTION(BlueprintCallable)
		void StartPlanning();

	UFUNCTION()
		void PathFinished(bool successfull);

	UFUNCTION(BlueprintCallable)
		bool IsActiveState()
	{
		return	currentState == EGOAPWorkerState::GWS_Planning ||
				currentState == EGOAPWorkerState::GWS_NextAction ||
				currentState == EGOAPWorkerState::GWS_Pathing ||
				currentState == EGOAPWorkerState::GWS_DoingAction ||
				currentState == EGOAPWorkerState::GWS_PathingTask ||
				currentState == EGOAPWorkerState::GWS_DoingTask;
	}

	UFUNCTION(BlueprintCallable)
		void CancelPlan();

	UFUNCTION(BlueprintCallable)
		void PrintState();

	UFUNCTION(BlueprintCallable)
		void PrintPlan(bool printTested = false);

	UFUNCTION(BlueprintCallable)
		void PrintPlanState();

	UFUNCTION(BlueprintCallable)
		bool HasState(FGOAPStateList inState);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void StartNextAction();

	void CheckPlanningProgress();
	void DoAction(float DeltaTime);
	UFUNCTION()
	void FinishedTask(bool successfull);

	FVector2D GetActionLocation();

	void BroadcastFinished(bool success);

public:	

	UPROPERTY(BlueprintAssignable)
		FStartPathing startPath;

	UPROPERTY(BlueprintAssignable)
		FPlanFinished finishedCallback;

	UPROPERTY(BlueprintAssignable)
		FStateChanged stateChangedCallback;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "state")
		FGOAPStateList state;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "state")
		TArray<class UCI_GOAPActionBase_CPP*> actions;

	bool isPathing = false;
	struct FTask* currentTask;

	UPROPERTY()
		class UPlan* plan = nullptr;

protected:
	UPROPERTY()
		EGOAPWorkerState currentState = EGOAPWorkerState::GWS_Idle;

private:
	bool broadcastedFinished = false;
};
