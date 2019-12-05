

#include "CI_GOAPPlan_CPP.h"
#include "RunnableThread.h"

//TODO:DOCUMENT comment/document this file

DECLARE_CYCLE_STAT(TEXT("Calculate Plan"), STAT_CalculatePlan, STATGROUP_GOAPPlanner);
DECLARE_CYCLE_STAT(TEXT("Calculate Plan Node"), STAT_CalculatePlanNode, STATGROUP_GOAPPlanner);
DECLARE_CYCLE_STAT(TEXT("Create Plan Node"), STAT_CreatePlanNode, STATGROUP_GOAPPlanner);

FPlanNode::FPlanNode(FGOAPStateList startState, FGOAPStateList goalState, class UCI_GOAPActionBase_CPP* action, TArray<int> possibleActions, float g, FPlanNode* parent)
{
	SCOPE_CYCLE_COUNTER(STAT_CreatePlanNode);

	this->action = action;
	this->possibleActions = possibleActions;
	this->g = g + this->action->cost;
	this->parent = parent;
	this->startState = startState;
	this->goalState = goalState;

	int temp;
	isValid = this->action->CheckGoalConditions(startState, temp);

	this->action->RevertEffects(startState, endState, this->action->GetOuter());

	isEnd = UGOAPStateUtil::TestConditions(endState, goalState, h);

	f = this->g + this->h;
}

bool FPlanNode::operator==(FPlanNode other) const
{
	int i = 0;
	return UGOAPStateUtil::TestEqual(endState, other.endState, i);
}

bool FPlanNode::operator!=(FPlanNode other) const
{
	int i = 0;
	return !UGOAPStateUtil::TestEqual(endState, other.endState, i);
}

bool FPlanNode::operator<(FPlanNode other) const
{
	return this->f < other.f;
}

bool FPlanNode::operator>(FPlanNode other) const
{
	return this->f > other.f;
}

FPlanNode* FPlanNode::GetLowest(TSet<FPlanNode*> set)
{
	FPlanNode* output = nullptr;

	for (auto& node : set)
	{
		if (output == nullptr)
			output = node;

		if (*node < *output)
			output = node;
	}

	return output;
}

UPlan::UPlan(FGOAPStateList startState, FGOAPStateList goalState,
	TArray<UCI_GOAPActionBase_CPP*> allAllowedActions)
{
	this->startState = startState;
	this->goalState = goalState;
	this->allAllowedActions = allAllowedActions;

	output = nullptr;
}

UPlan::~UPlan()
{
	if (task != nullptr)
		task->_Shutdown();
}

void UPlan::Init(FGOAPStateList startState, FGOAPStateList goalState,
	TArray<UCI_GOAPActionBase_CPP*> allAllowedActions)
{
	this->startState = startState;
	this->goalState = goalState;
	this->allAllowedActions = allAllowedActions;
}

void UPlan::Init(FGOAPStateList startState, UCI_GOAPActionBase_CPP* goalAction,
	TArray<UCI_GOAPActionBase_CPP*> actions)
{
	this->startState = goalAction->preConditions;
	this->goalState = startState;
	this->allAllowedActions = actions;
}

void UPlan::StartCalculation()
{
	task = FCalculatePlanTask::CreatePlanCalculator(startState, goalState, allAllowedActions, this);
}

void UPlan::LogPlan(bool printTested)
{
	UE_LOG(GOAP, Log, TEXT("----------------------------------"));

	if (!output->isComplete)
	{
		UE_LOG(GOAP, Log, TEXT("- Plan is incomplete!"));
		UE_LOG(GOAP, Log, TEXT("----------------------------------"));


		if (printTested)
		{
			int counter = 0;
			for (auto node : output->closedNodes)
			{
				UE_LOG(GOAP, Log, TEXT("------ "));
				UE_LOG(GOAP, Log, TEXT("- %d: %s"), counter, *node->action->GetName());
				UE_LOG(GOAP, Log, TEXT("- StartState:"));
				UGOAPStateUtil::Print(node->startState);
				UE_LOG(GOAP, Log, TEXT("- "));
				UE_LOG(GOAP, Log, TEXT("- EndState:"));
				UGOAPStateUtil::Print(node->endState);
				UE_LOG(GOAP, Log, TEXT("- "));
				UE_LOG(GOAP, Log, TEXT("- GoalState:"));
				UGOAPStateUtil::Print(node->goalState);
				UE_LOG(GOAP, Log, TEXT("- "));
				UE_LOG(GOAP, Log, TEXT("- actions: %d"), node->possibleActions.Num());
				UE_LOG(GOAP, Log, TEXT("------ "));
				UE_LOG(GOAP, Log, TEXT("- "));
				counter++;

			}
		}



		return;
	}

	UE_LOG(GOAP, Log, TEXT("- Plan is as follow:"));
	UE_LOG(GOAP, Log, TEXT("- "));

	UE_LOG(GOAP, Log, TEXT("- Start State:"));
	UGOAPStateUtil::Print(goalState);
	UE_LOG(GOAP, Log, TEXT("- "));

	UE_LOG(GOAP, Log, TEXT("- Goal State: "));
	UGOAPStateUtil::Print(startState);
	UE_LOG(GOAP, Log, TEXT("- "));

	UE_LOG(GOAP, Log, TEXT("- This will be reached via:"));

	int counter = 0;
	for (auto node : output->plan)
	{
		UE_LOG(GOAP, Log, TEXT("- %d: %s"), counter, *node->action->GetName());
		counter++;
	}

	UE_LOG(GOAP, Log, TEXT("- "));

	UE_LOG(GOAP, Log, TEXT("----------------------------------"));
}

bool UPlan::ReadQueue()
{
	if (!outputQueue.IsEmpty())
	{
		outputQueue.Dequeue(output);

		task->_Shutdown();
		task = nullptr;

		return true;
	}

	return false;
}

bool UPlan::HasValidPlan()
{
	if (output == nullptr)
		return false;
	
	return output->isComplete;
}

bool UPlan::StartNextAction()
{
	if (currentAction == nullptr)
	{
		if (output->plan.Num() == 0)
			return false;

		currentAction = output->plan[0];
		return true;
	}
	else
	{

		if (currentAction->parent != nullptr)
		{
			currentAction = currentAction->parent;
			return true;

		}

		currentAction = nullptr;

	}

	return false;
}

bool UPlan::DoCurrentAction(float deltaTime, UPARAM(ref)FGOAPStateList& startState, UCI_GOAPWorkerComponent_CPP* workerComponent)
{
	currentActionTimer += deltaTime;


	if (currentAction->action->DoAction(currentActionTimer, startState, workerComponent))
	{
		currentActionTimer = 0;
		return true;
	}

	return false;
}

void UPlan::Stop()
{
	if (task)
		task->Stop();

	task = nullptr;
}

FCalculatePlanTask::FCalculatePlanTask(FGOAPStateList startState, FGOAPStateList goalState,
	TArray<class UCI_GOAPActionBase_CPP*> actions, UPlan* owner)
	: stopTaskCounter(0), startState(startState), goalState(goalState), actions(actions), owner(owner)
{
	runnable.Add(this);

	thread = FRunnableThread::Create(this, TEXT("CalculatePlan"), 0, TPri_BelowNormal);
}


FCalculatePlanTask::~FCalculatePlanTask()
{
	delete thread;
	thread = nullptr;
	runnable.Remove(this);
}

bool FCalculatePlanTask::Init()
{
	planData = new UPlan::PlanData();

	for (int i = 0; i < actions.Num(); i++)
	{
		actionInt.Add(i);
	}

	return true;
}

uint32 FCalculatePlanTask::Run()
{
	//Initial wait before starting
	FPlatformProcess::Sleep(0.03);

	int counter = 0;
	for (int i = 0; i < runnable.Num(); i++)
	{
		if (runnable[i]->doRun)
		{
			counter++;
		}
	}
	if (counter < FPlatformMisc::NumberOfCoresIncludingHyperthreads() * 10)
	{
		doRun = true;
	}

	while (!doRun)
		FPlatformProcess::Sleep(0.03);

	DoWork();

	isFinished = true;
	return 0;
}

void FCalculatePlanTask::Stop()
{
	stopTaskCounter.Increment();
}

void FCalculatePlanTask::EnsureCompletion()
{
	Stop();
	thread->WaitForCompletion();
}

void FCalculatePlanTask::DoWork()
{
	int temp;
	if (UGOAPStateUtil::TestConditions(startState, goalState, temp))
	{
		planData->isComplete = true;
		isFinished = true;
		ReturnCalculationFinished();
		return;
	}

	FPlanNode* nextNode;

	for (auto action : actions)
	{
		TArray<int> newActions = actionInt;

		int actionNum;
		actions.Find(action, actionNum);
		newActions.Remove(actionNum);

		nextNode = new FPlanNode(startState, goalState, action, newActions, 0, nullptr);

		if (nextNode->isValid)
			planData->openNodes.Add(nextNode);
	}

	nextNode = FPlanNode::GetLowest(planData->openNodes);

	while (planData->openNodes.Num() > 0 && !nextNode->isEnd && stopTaskCounter.GetValue() == 0)
	{
		for (auto actionNum : nextNode->possibleActions)
		{
			TArray<int> newActions = nextNode->possibleActions;
			newActions.Remove(actionNum);

			FPlanNode* newNode = new FPlanNode(nextNode->endState, goalState, actions[actionNum], newActions, 0, nextNode);

			if (newNode->isValid)
			{
				planData->openNodes.Add(newNode);
			}
		}

		planData->closedNodes.Add(nextNode);
		planData->openNodes.Remove(nextNode);

		nextNode = FPlanNode::GetLowest(planData->openNodes);
	}


	if (stopTaskCounter.GetValue() != 0)
	{
		planData->isComplete = false;
		isFinished = true;
		ReturnCalculationFinished();
		return;
	}

	//No valid path!
	if (nextNode == nullptr || !nextNode->isEnd)
	{
		planData->isComplete = false;
		ReturnCalculationFinished();
		isFinished = true;
		return;
	}

	while (nextNode->parent != nullptr)
	{
		planData->plan.Add(nextNode);
		nextNode->parent->child = nextNode;
		nextNode = nextNode->parent;
	}

	planData->plan.Add(nextNode);

	planData->isComplete = true;
	isFinished = true;
	ReturnCalculationFinished();
}

void FCalculatePlanTask::ReturnCalculationFinished()
{
	if (stopTaskCounter.GetValue() == 0)
		owner->outputQueue.Enqueue(planData);

	isFinished = true;
}

void FCalculatePlanTask::_Shutdown()
{
	EnsureCompletion();
	runnable.Remove(this);

	for (int i = 0; i < runnable.Num(); i++)
	{
		if (!runnable[i]->doRun)
		{
			runnable[i]->doRun = true;
			break;
		}
	}

	delete this;
}

void FCalculatePlanTask::ShutDown()
{
	while (runnable.Num() > 0)
	{
		runnable[0]->_Shutdown();
	}
}

FCalculatePlanTask* FCalculatePlanTask::CreatePlanCalculator(FGOAPStateList startState, FGOAPStateList goalState,
	TArray<UCI_GOAPActionBase_CPP*> actions, UPlan* owner)
{
	FCalculatePlanTask* output = nullptr;
	if (FPlatformProcess::SupportsMultithreading())
	{
		output = new FCalculatePlanTask(startState, goalState, actions, owner);
	}

	return output;
}

TArray<FCalculatePlanTask*> FCalculatePlanTask::runnable;
