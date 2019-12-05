// Fill out your copyright notice in the Description page of Project Settings.

#include "CI_GOAPActionBase_CPP.h"
#include "DebugMacros.h"
#include "Engine/World.h"
#include "AI/GOAP/StateNodeTypes/CI_GOAPStateNodeTypeAll_CPP.h"
#include "AI/GOAP/CI_GOAPWorkerComponent_CPP.h"

//TODO:DOCUMENT comment/document this file

bool UCI_GOAPActionBase_CPP::CheckPreConditions(FGOAPStateList startState, int &difference)
{
	return UGOAPStateUtil::TestConditions(preConditions, startState, difference);
}

bool UCI_GOAPActionBase_CPP::CheckGoalConditions(FGOAPStateList goalState, int &difference)
{
	return UGOAPStateUtil::TestConditions(effects, goalState, difference);
}

bool UCI_GOAPActionBase_CPP::AddEffects(FGOAPStateList startState, FGOAPStateList& resultState, UObject* outer)
{
	if (resultState != startState)
	{
		resultState = FGOAPStateList();
		resultState.Copy(startState);
	}
	else
	{
		resultState = startState;
	}

	for (auto effect : effects.stateNodes)
	{
		if (resultState.stateNodes.Contains(effect.Key))
		{
			resultState.stateNodes.FindRef(effect.Key)->AddValue(effect.Value);
		}
		else
		{
			auto newNode = NewObject<UGOAPStateNodeType>(outer, effect.Value->GetClass(),NAME_None,RF_NoFlags,effect.Value);
			resultState.stateNodes.Add(effect.Key) = newNode;
		}
	}

	return false;
}

bool UCI_GOAPActionBase_CPP::RevertEffects(FGOAPStateList goalState, FGOAPStateList& resultState, UObject* outer)
{
	if (resultState != goalState)
	{
		resultState = FGOAPStateList();
		resultState.Copy(goalState);
	}
	else
	{
		resultState = goalState;
	}

	for (auto effect : effects.stateNodes)
	{
		if (resultState.stateNodes.Contains(effect.Key))
		{
			resultState.stateNodes.FindRef(effect.Key)->RemoveValue(effect.Value);
		}
		else
		{
			auto newNode = NewObject<UGOAPStateNodeType>(outer, effect.Value->GetClass(), NAME_None, RF_NoFlags, effect.Value);
			resultState.stateNodes.Add(effect.Key) = newNode;
			newNode->RemoveValue(effect.Value);
			newNode->RemoveValue(effect.Value);
		}
	}

	for (auto condition : preConditions.stateNodes)
	{
		if (resultState.stateNodes.Contains(condition.Key))
		{
			resultState.stateNodes.FindRef(condition.Key)->AddValue(condition.Value);
		}
		else
		{
			auto newNode = NewObject<UGOAPStateNodeType>(outer, condition.Value->GetClass(), NAME_None, RF_NoFlags, condition.Value);
			resultState.stateNodes.Add(condition.Key) = newNode;
		}
	}

	return false;
}

void UCI_GOAPActionBase_CPP::InitAction(FVector2D goalLocation, bool needsMove)
{
	location = goalLocation;
	this->needsMove = needsMove;
}

bool UCI_GOAPActionBase_CPP::DoAction(float currentTime, FGOAPStateList& startState, UCI_GOAPWorkerComponent_CPP* workerComponent)
{
	if (currentTime >= excecuteTime)
	{
		AddEffects(startState, startState, workerComponent);
		return true;
	}
	
	return false;
}
