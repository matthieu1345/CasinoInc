// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Base/CI_BaseAIController_CPP.h"
#include "CI_BaseStaffController_CPP.generated.h"

/**
 * Base AI controllers for Casino Inc. <b>Staff Members</b><br>
 * besides the basic functionality of CI_BaseAIController_CPP.h it also has implementation for leaving/entering the casino based on working times
 */
UCLASS()
class CASINO_INC_API ACI_BaseStaffController_CPP : public ACI_BaseAIController_CPP
{
	GENERATED_BODY()

public:
	/**
	 *	\brief Function called every frame on this Actor. Override this function to implement custom logic to be executed every frame.
	 *	\param DeltaSeconds	Game time elapsed during last frame modified by the time dilation
	 */
	virtual void Tick(float DeltaSeconds) override;
	
protected:
	/**
	 * \brief A function that get's called every time the ingame hour changes<br>
	 * Checks if the Staff ai needs to enter or leave the casino based on it's working times
	 * \param hourNumber the current hour
	 */
	virtual void HourChanged(int hourNumber) override;

public:
	// the hour that the staff member starts it's work day
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "WorkTime")
		int startWorkTime = 8;
	// the hour that the staff member ends it's work day
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "WorkTime")
		int endWorkTime = 16;
	// the amount of hours the staff member works each day
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "WorkTime")
		int workStintLength = 8;

	// the amount of money the staff member get's paid daily
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "cost")
		int hourlySalary = 10;

protected:

};
