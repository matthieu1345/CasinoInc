// Fill out your copyright notice in the Description page of Project Settings.

#include "CI_BaseStaffController_CPP.h"
#include "TileMap/CI_TileMapCoordinateMath.h"
#include "AI/CI_AIManager_CPP.h"
#include "CI_BaseAIPawn_CPP.h"
#include "AI/GOAP/CI_GOAPWorkerComponent_CPP.h"
#include "PathFinding/CI_PathFollowingComponent_CPP.h"
#include "Managers/CI_StatsManager_CPP.h"

void ACI_BaseStaffController_CPP::HourChanged(int hourNumber)
{
	Super::HourChanged(hourNumber);

	// if the staff member does not work past midnight
	if (startWorkTime + workStintLength <= 24)
	{
		if (hourNumber >= startWorkTime && hourNumber < endWorkTime) // if the worker is supposed to be at work tell him to come and pay him his salary
		{
			if (isAway)
				EnterMap();

			ACI_StatsManager_CPP::GetInstance(GetWorld())->AddMoney(-hourlySalary);
		}
			

		if ((hourNumber < startWorkTime || hourNumber >= endWorkTime) && !isAway) // if the worker is in the casino, but not supposed to be here, tell him to leave
			LeaveMap();
	}
	else if (startWorkTime + workStintLength > 24) // if the staff member does work past midnight
	{
		if (hourNumber > endWorkTime && !isAway) // if the worker is in the casino, but not supposed to be here, tell him to leave
			LeaveMap();

		if (hourNumber >= startWorkTime) // if the worker is supposed to be at work tell him to come and pay him his salary
		{
			if (isAway)
				EnterMap();
			ACI_StatsManager_CPP::GetInstance(GetWorld())->AddMoney(-hourlySalary);
		}
	}
}

void ACI_BaseStaffController_CPP::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// if the worker is supposed to be away, and doesn't have a active path, turn him invisible
	if (!pathFollowingComp->IsActiveState() && isAway)
		possessedCustomBasePawn->SetActorHiddenInGame(true);
}
