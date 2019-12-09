// Fill out your copyright notice in the Description page of Project Settings.

#include "CI_BaseTask_CPP.h"
#include "AI/Base/CI_BaseAIController_CPP.h"
#include "TileMap/CI_BaseTile_CPP.h"

//TODO:DOCUMENT comment/document this file

void UCI_BaseTask_CPP::FinishTask_Implementation(bool success, int x, int y, class ACI_GameStateBase_CPP* gamestate)
{
	
}

UCI_GOAPActionBase_CPP *UCI_BaseTask_CPP::GetAIStateAction() const
{
	return aiStateAction;
}

void FTask::Finished(ACI_GameStateBase_CPP* gamestate) const
{
	taskType->DestroyTask(x, y ,gamestate);
}

void FTask::DoTask(const float deltaTime, ACI_GameStateBase_CPP* gamestate)
{
	taskTimer += deltaTime;

	if (taskTimer >= taskType->GetTaskLenght())
	{
		if (!finished)
		{
			if (tile != nullptr)
			{
				taskType->FinishTask(true, tile->GetX(), tile->GetY(), gamestate);
			}
			else
			{
				taskType->FinishTask(true, x, y, gamestate);
			}
		}

		if (taskFinished.IsBound())
		{
			taskFinished.Broadcast(true);
		}

		finished = true;
	}
}

bool FTask::IsValidLocation(ACI_GameStateBase_CPP* gamestate) const
{
	return taskType->CheckValidLocation(x, y, gamestate);
}
