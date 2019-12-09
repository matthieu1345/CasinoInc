#pragma once

#include "CoreMinimal.h"
#include "AI/GOAP/Actions/CI_GOAPActionBase_CPP.h"
#include "Runnable.h"
#include "Queue.h"
#include "CI_GOAPPlan_CPP.generated.h"

//TODO:DOCUMENT comment/document this file

DECLARE_STATS_GROUP(TEXT("GOAP Planner"), STATGROUP_GOAPPlanner, STATCAT_Advanced);

//USTRUCT(BlueprintType)
struct FPlanNode
{
	//GENERATED_BODY()

		FPlanNode() = default;
		FPlanNode(FGOAPStateList startState, FGOAPStateList goalState, class UCI_GOAPActionBase_CPP* action, TArray<int> possibleActions, float g, FPlanNode* parent);

	//UPROPERTY()
		class UCI_GOAPActionBase_CPP* action;

	//UPROPERTY()
		TArray<int> possibleActions;

	//UPROPERTY()
		struct FGOAPStateList endState;
	//UPROPERTY()
		struct FGOAPStateList startState;
	//UPROPERTY()
		struct FGOAPStateList goalState;

	//cost from start
	//UPROPERTY()
		int g = 0;

	// heuristic
	//UPROPERTY()
		int h = 0;

	//final cost
	//UPROPERTY()
		int f = 0;

	//UPROPERTY()
		bool isEnd = false;

	//UPROPERTY()
		bool isValid = false;

	FPlanNode* parent = nullptr;
	FPlanNode* child = nullptr;

	bool operator ==(FPlanNode other) const;
	bool operator !=(FPlanNode other) const;

	bool operator <(FPlanNode other) const;
	bool operator >(FPlanNode other) const;

	static FPlanNode* GetLowest(TSet<FPlanNode*> set);
};

UCLASS()
class UPlan : public UObject
{
	GENERATED_BODY()

public:
	friend class FCalculatePlanTask;

	struct PlanData
	{
		PlanData(const TSet<FPlanNode*> openNodes, const TSet<FPlanNode*> closedNodes, const TArray<FPlanNode*> plan, const bool isComplete) :
			openNodes(openNodes), closedNodes(closedNodes), plan(plan), isComplete(isComplete) {}
		PlanData() {}

		TSet<FPlanNode*> openNodes;
		TSet<FPlanNode*> closedNodes;
		TArray<FPlanNode*> plan;
		bool isComplete = false;
	};

	UPlan() = default;
	UPlan(FGOAPStateList startState, FGOAPStateList goalState, TArray<class UCI_GOAPActionBase_CPP*> allAllowedActions);
	~UPlan();

	void Init(FGOAPStateList startState, FGOAPStateList goalState, TArray<class UCI_GOAPActionBase_CPP*> allAllowedActions);
	void Init(FGOAPStateList startState, UCI_GOAPActionBase_CPP* goalAction, TArray<UCI_GOAPActionBase_CPP*> actions);
	void StartCalculation();

	UFUNCTION(BlueprintCallable)
		void LogPlan(bool printTested = false) const;

	bool ReadQueue();

	bool HasValidPlan() const;

	bool StartNextAction();

	UCI_GOAPActionBase_CPP* GetCurrentAction() const { return currentAction->action; }
	UFUNCTION(BlueprintCallable)
	bool DoCurrentAction(float deltaTime, UPARAM(ref)FGOAPStateList& startState, UCI_GOAPWorkerComponent_CPP* workerComponent);

	void Stop();

protected:
	FGOAPStateList startState;
	FGOAPStateList goalState;
	TArray<class UCI_GOAPActionBase_CPP*> allAllowedActions;

	class FCalculatePlanTask* task = nullptr;
	class UObject* owner = nullptr;

	PlanData* output = nullptr;
	FPlanNode* currentAction = nullptr;
	
	TQueue<PlanData*, EQueueMode::Mpsc> outputQueue;

	float currentActionTimer = 0;


};

class FCalculatePlanTask
	: public FRunnable
{
	FCalculatePlanTask(FGOAPStateList startState, FGOAPStateList goalState, TArray<class UCI_GOAPActionBase_CPP*> actions, UPlan* owner);
	~FCalculatePlanTask();

public:
	static TArray<FCalculatePlanTask*> runnable;
	/** Thread to run the worker FRunnable on */
	FRunnableThread * thread;
	/** Stop this thread? Uses Thread Safe Counter */
	FThreadSafeCounter stopTaskCounter;

	bool IsFinished() const
	{
		return isFinished;
	}

	// Begin FRunnable interface.
	bool Init() override;
	uint32 Run() override;
	void Stop() override;
	// End FRunnable interface

	/** Makes sure this thread has stopped properly */
	void EnsureCompletion();

	void DoWork();

	void ReturnCalculationFinished();

	void _Shutdown();
	static void ShutDown();

	static FCalculatePlanTask* CreatePlanCalculator(FGOAPStateList startState, FGOAPStateList goalState, TArray<class UCI_GOAPActionBase_CPP*> actions, UPlan* owner);

	//state
	FGOAPStateList startState;
	FGOAPStateList goalState;
	TArray<class UCI_GOAPActionBase_CPP*> actions;
	TArray<int> actionInt;
	UPlan* owner;
	UPlan::PlanData* planData = nullptr;
	bool isFinished = false;
	bool doRun = false;
};