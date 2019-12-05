// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Base/CI_BaseAIController_CPP.h"
#include "CI_BaseStaffController_CPP.generated.h"

//TODO:DOCUMENT comment/document this file

/**
 * 
 */
UCLASS()
class CASINO_INC_API ACI_BaseStaffController_CPP : public ACI_BaseAIController_CPP
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaSeconds) override;
	
protected:
	virtual void HourChanged(int hourNumber) override;

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "WorkTime")
		int startWorkTime = 8;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "WorkTime")
		int endWorkTime = 16;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "WorkTime")
		int workStintLength = 8;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "cost")
		int hourlySalary = 10;

protected:

};
