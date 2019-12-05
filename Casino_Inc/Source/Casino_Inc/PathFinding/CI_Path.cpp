// Fill out your copyright notice in the Description page of Project Settings.

#include "CI_Path.h"
#include "TileMap/CI_TileMap_CPP.h"
#include "RunnableThread.h"
#include "Queue.h"

//TODO:DOCUMENT comment/document this file

DECLARE_CYCLE_STAT(TEXT("Calculate Path"), STAT_CalculatePath, STATGROUP_PathFinding);
DECLARE_CYCLE_STAT(TEXT("Calculate Node"), STAT_CalculateNode, STATGROUP_PathFinding);
DECLARE_CYCLE_STAT(TEXT("Create Node"), STAT_CreateNode, STATGROUP_PathFinding);
DECLARE_CYCLE_STAT(TEXT("Get Lowest"), STAT_GetLowest, STATGROUP_PathFinding);
DECLARE_CYCLE_STAT(TEXT("Set Contains"), STAT_SetContains, STATGROUP_PathFinding);

FPathNode::FPathNode(FVector2D location, FVector2D goal, float g, FPathNode* parent)
{
	SCOPE_CYCLE_COUNTER(STAT_CreateNode);

	this->location = location;

	this->g = g;

	h = FVector2D::Distance(goal, location);

	f = this->g + h;

	isEnd = location.Equals(goal, 0.01);

	this->parent = parent;
}

FPathNode* FPathNode::GetLowest(TSet<FPathNode*> set)
{
	SCOPE_CYCLE_COUNTER(STAT_GetLowest);

	FPathNode* output = nullptr;
	
	for (auto& node : set)
	{
		if (output == nullptr)
			output = node;

		if (*node < *output)
			output = node;
	}

	return output;
}

bool FPathNode::SetContains(TSet<FPathNode*> set, FPathNode* test)
{
	SCOPE_CYCLE_COUNTER(STAT_SetContains);

	for (auto& node : set)
	{
		if (test == nullptr || node == nullptr)
			break;


		if (test->location.Equals(node->location, 0.2f))
		{
			return true;
		}
	}

	return false;
}

UPath::UPath(FVector2D start, FVector2D end, ACI_TileMap_CPP* tileMap)
{
	this->start = start;
	this->end = end;
	this->tileMap = tileMap;
	
	outputData = nullptr;
}

UPath::~UPath()
{
	if (outputData)
	{
		outputData->openNodes.Empty();
		outputData->closedNodes.Empty();
		outputData->path.Empty();
		
		delete outputData;
	}
	

	if (task != nullptr)
		task->_Shutdown();
}

void UPath::Init(FVector2D start, FVector2D end, int maxReach, ACI_TileMap_CPP* tileMap)
{
	this->start = start;
	this->end = end;
	this->tileMap = tileMap;
	this->maxReach = maxReach;

	if (task != nullptr)
		task->_Shutdown();

	task = nullptr;
}

void UPath::StartCaluclatePath()
{
	task = FCalculatePathTask::CreatePathCalculator(start, end, maxReach, tileMap, this);
}

void UPath::Stop()
{
	if (task)
		task->_Shutdown();


	task = nullptr;
}

bool UPath::ReadQueue()
{

	if (outputQueue.IsEmpty())
		return false;

	if (outputQueue.Dequeue(outputData))
	{
		isFinished = true;

		task->_Shutdown();
		task = nullptr;

		return true;
	}

	return false;
}

void FCalculatePathTask::DoWorkInternal()
{
	/*if (!CheckNext(end.X, end.Y))
	{
		pathData->isComplete = false;
		ReturnCalculationFinished();
		isFinished = true;
		return;
	}*/

	FPathNode* nextNode = new FPathNode(start, end, 0, nullptr);
	pathData->openNodes.Add(nextNode);

	int counter = 0;

	while (pathData->openNodes.Num() > 0 && !nextNode->isEnd && stopTaskCounter.GetValue() == 0 && counter < maxReach)
	{
		counter++;

		FPathNode* tempNode;

		// add new nodes to the open nodes
		if (CheckNext(nextNode->location.X + 1, nextNode->location.Y))
		{
			tempNode = new FPathNode(FVector2D(nextNode->location.X + 1, nextNode->location.Y), end, nextNode->g + 1, nextNode);

			if (!FPathNode::SetContains(pathData->openNodes, tempNode) && !FPathNode::SetContains(pathData->closedNodes, tempNode))
				pathData->openNodes.Add(tempNode);

			if (tempNode->isEnd)
			{
				nextNode = tempNode;
				break;
			}
		}

		if (CheckNext(nextNode->location.X - 1, nextNode->location.Y))
		{
			tempNode = new FPathNode(FVector2D(nextNode->location.X - 1, nextNode->location.Y), end, nextNode->g + 1, nextNode);

			if (!FPathNode::SetContains(pathData->openNodes, tempNode) && !FPathNode::SetContains(pathData->closedNodes, tempNode))
				pathData->openNodes.Add(tempNode);

			if (tempNode->isEnd)
			{
				nextNode = tempNode;
				break;
			}
		}

		if (CheckNext(nextNode->location.X, nextNode->location.Y + 1))
		{
			tempNode = new FPathNode(FVector2D(nextNode->location.X, nextNode->location.Y + 1), end, nextNode->g + 1, nextNode);

			if (!FPathNode::SetContains(pathData->openNodes, tempNode) && !FPathNode::SetContains(pathData->closedNodes, tempNode))
				pathData->openNodes.Add(tempNode);

			if (tempNode->isEnd)
			{
				nextNode = tempNode;
				break;
			}
		}

		if (CheckNext(nextNode->location.X, nextNode->location.Y - 1))
		{
			tempNode = new FPathNode(FVector2D(nextNode->location.X, nextNode->location.Y - 1), end, nextNode->g + 1, nextNode);

			if (!FPathNode::SetContains(pathData->openNodes, tempNode) && !FPathNode::SetContains(pathData->closedNodes, tempNode))
				pathData->openNodes.Add(tempNode);

			if (tempNode->isEnd)
			{
				nextNode = tempNode;
				break;
			}
		}

		if (stopTaskCounter.GetValue() == 0)
		{
			pathData->closedNodes.Add(nextNode);
			pathData->openNodes.Remove(nextNode);

			nextNode = FPathNode::GetLowest(pathData->openNodes);
		}
		else
			break;
	}

	if (stopTaskCounter.GetValue() != 0 || counter >= maxReach)
	{
		pathData->isComplete = false;
		isFinished = true;
		ReturnCalculationFinished();
		return;
	}

	//No Path possible!
	if (nextNode == nullptr)
	{
		pathData->isComplete = false;
		ReturnCalculationFinished();
		isFinished = true;
		return;
	}

	if (!nextNode->isEnd)
	{
		pathData->isComplete = false;
		ReturnCalculationFinished();
		isFinished = true;
		return;
	}

	while (nextNode->parent)
	{
		pathData->path.Insert(nextNode, 0);
		nextNode->parent->child = nextNode;
		nextNode = nextNode->parent;
	}

	pathData->path.Insert(nextNode, 0);

	pathData->isComplete = true;
	isFinished = true;
	ReturnCalculationFinished();
}

bool FCalculatePathTask::CheckNext(int x, int y)
{
	if (IsValid(tileMap))
		return tileMap->CheckWalkable(x, y) || (x == round(end.X) && y == round(end.Y));

	return false;
}

FCalculatePathTask::FCalculatePathTask(FVector2D start, FVector2D end, int maxReach, ACI_TileMap_CPP* tileMap, UPath* owner) : stopTaskCounter(0), start(start), end(end), maxReach(maxReach), tileMap(tileMap), owner(owner)
{
	thread = FRunnableThread::Create(this, TEXT("CalculatePath"), 0, TPri_BelowNormal);

	runnable.Add(this);
}

FCalculatePathTask::~FCalculatePathTask()
{
	owner->task = nullptr;
	if (!isFinished)
	{
		for (auto node : pathData->openNodes)
		{
			delete node;
		}
		pathData->openNodes.Empty();

		for (auto node : pathData->closedNodes)
		{
			delete node;
		}
		pathData->closedNodes.Empty();

		for (auto node : pathData->path)
		{
			delete node;
		}
		pathData->path.Empty();
	}

	if (IsValid(owner))
		owner->task = nullptr;

	thread->Kill();
	delete thread;
	thread = nullptr;
	runnable.Remove(this);
}

FCalculatePathTask* FCalculatePathTask::CreatePathCalculator(FVector2D start, FVector2D end, int maxReach, ACI_TileMap_CPP* tileMap, UPath* owner)
{
	FCalculatePathTask* output = nullptr;
	if (FPlatformProcess::SupportsMultithreading())
	{
		output = new FCalculatePathTask(start, end, maxReach, tileMap, owner);
		//runnable.Add(output);
	}

	return output;
}

bool FCalculatePathTask::Init()
{
	pathData = new UPath::PathData();

	return true;
}

uint32 FCalculatePathTask::Run()
{
	//Initial wait before starting
	FPlatformProcess::Sleep(0.03);

	int counter = 0;
	for (int i = 0; i < runnable.Num(); i++ )
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

	DoWorkInternal();

	isFinished = true;
	return 0;
}

void FCalculatePathTask::Stop()
{
	stopTaskCounter.Increment();
}

void FCalculatePathTask::EnsureCompletion()
{
	Stop();
	thread->WaitForCompletion();
}

void FCalculatePathTask::Shutdown()
{
	while (runnable.Num() > 0)
	{
		runnable[0]->_Shutdown();
	}
}

void FCalculatePathTask::_Shutdown()
{
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

bool FCalculatePathTask::IsThreadFinished()
{
	for (auto task : runnable)
	{
		if (!task->_IsThreadFinished())
		{
			return false;
		}
	}
	return true;
}

bool FCalculatePathTask::_IsThreadFinished()
{
	return isFinished;
}

void FCalculatePathTask::ReturnCalculationFinished()
{
	if (stopTaskCounter.GetValue() == 0)
		owner->outputQueue.Enqueue(pathData);

	isFinished = true;
}

TArray<FCalculatePathTask*> FCalculatePathTask::runnable;