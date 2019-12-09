// Fill out your copyright notice in the Description page of Project Settings.

#include "CI_GameStateBase_CPP.h"

//TODO:DOCUMENT comment/document this file

void ACI_GameStateBase_CPP::Tick(float deltaSeconds)
{

	changedSelectedBuildObject = false;

}

void ACI_GameStateBase_CPP::ToggleCheatMode()
{
	cheatmode = !cheatmode;
}

void ACI_GameStateBase_CPP::ChangeMode(const EBuildInputMode newMode)
{
	switch (currentMode)
	{
	case EBuildInputMode::BIM_None: 
		
		break;

	case EBuildInputMode::BIM_Destroy: 
		
		break;

	case EBuildInputMode::BIM_Priority: 
		
		break;

	case EBuildInputMode::BIM_Build: 
		SetSelectedBuildObject(nullptr);
		SetSelectedTask(nullptr);
		break;

	case EBuildInputMode::BIM_BuildRoom:
		isSelecting2NdCorner = false;
		break;

	default: ;
	}



	switch (newMode)
	{
	case EBuildInputMode::BIM_None: 
		currentMode = EBuildInputMode::BIM_None;
		break;

	case EBuildInputMode::BIM_Destroy: 
		currentMode = EBuildInputMode::BIM_Destroy;
		break;

	case EBuildInputMode::BIM_Priority:
		currentMode = EBuildInputMode::BIM_Priority;
		break;

	case EBuildInputMode::BIM_Build: 
		currentMode = EBuildInputMode::BIM_Build;
		break;

	case EBuildInputMode::BIM_BuildRoom:
		currentMode = EBuildInputMode::BIM_BuildRoom;
		break;

	default: ;
	}



}
