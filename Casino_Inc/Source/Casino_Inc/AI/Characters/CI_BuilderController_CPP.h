// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Base/CI_BaseStaffController_CPP.h"
#include "CI_BuilderController_CPP.generated.h"

/**
 * Controller for the <b>Builder Controller</b> for Casino Inc.<br>
 * has all builder specific logic
 */
UCLASS(Blueprintable)
class CASINO_INC_API ACI_BuilderController_CPP : public ACI_BaseStaffController_CPP
{
	GENERATED_BODY()

		ACI_BuilderController_CPP();

protected:
	// called when the controller begin's
	virtual void BeginPlay() override;
public:
	// called every tick to update the controller
	virtual void Tick(float deltaSeconds) override;

	// call when a new task is needed
	virtual void GetNewTask() override;

	/**
	 * \brief called when the current task has ended
	 * \param success returns if the task was a success or a fail
	 */
	virtual void TaskEnded_Implementation(bool success) override;

	/**
	 * \brief implementation of the idle behaviour for this ai
	 */
	virtual void DoIdle() override;
};
