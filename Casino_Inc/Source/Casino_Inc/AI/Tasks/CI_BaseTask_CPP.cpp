// Fill out your copyright notice in the Description page of Project Settings.

#include "CI_BaseTask_CPP.h"
#include "AI/Base/CI_BaseAIController_CPP.h"
#include "DebugMacros.h"
#include "TileMap/CI_BaseTile_CPP.h"

void UCI_BaseTask_CPP::FinishTask_Implementation(bool success, int x, int y, class ACI_GameStateBase_CPP* gamestate)
{
	
}

void FTask::Finished(ACI_GameStateBase_CPP* gamestate)
{
	taskType->DestroyTask(x, y ,gamestate);
}

void FTask::DoTask(float DeltaTime, ACI_GameStateBase_CPP* gamestate)
{
	taskTimer += DeltaTime;

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

bool FTask::isValidLocation(ACI_GameStateBase_CPP* gamestate)
{
	return taskType->CheckValidLocation(x, y, gamestate);
}
