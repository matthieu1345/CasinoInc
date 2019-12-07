// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Base/CI_BaseStaffController_CPP.h"
#include "CI_DealerController_CPP.generated.h"

/**
 * Controller for the <b>Dealer Controller</b> for Casino Inc.<br>
 * has all Dealer specific logic
 */
UCLASS(Blueprintable)
class CASINO_INC_API ACI_DealerController_CPP : public ACI_BaseStaffController_CPP
{
	GENERATED_BODY()

		ACI_DealerController_CPP();

public:
	/**
	 *	\brief Function called every frame on this Actor. Override this function to implement custom logic to be executed every frame.
	 *	\param DeltaSeconds	Game time elapsed during last frame modified by the time dilation
	 */
	virtual void Tick(float DeltaSeconds) override;

	/**
	 * \brief called when we need to get a new task
	 */
	virtual void GetNewTask() override;

protected:
	/**
	 *\brief native event for when play begins for this actor.
	 */
	virtual void BeginPlay() override;

	/**
	 * \brief implementation for the idle behaviour of the dealer ai
	 */
	virtual void DoIdle() override;

	/**
	 * \brief called when the ai needs to enter the map<br>
	 * makes the character visible and resets the idle and away booleans
	 */
	virtual void EnterMap() override;

	/**
	 * \brief called when the ai needs to Leave the map<br>
	 * makes the character move towards a leaving spot and set's the idle and away booleans
	 */
	virtual void LeaveMap() override;

public:
	/**
	 * \brief c++ implementation for <b>TaskEnded</b>:<br>
	 * return here if the task has ended
	 * Defaults to asking for a new task from the ai manager, or go to the idle behavior if none available
	 * \param success input if the task was a success or a fail
	 */
	void TaskEnded_Implementation(bool success) override;

protected:
	// flag for the dealer being idle
	bool isIdle = true;

	// reference to the task location
	FVector2D taskLocation;
	// reference to the task interactable
	class ACI_BaseTile_CPP* taskInteractable;
};
