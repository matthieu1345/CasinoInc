// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Base/CI_BaseStaffController_CPP.h"
#include "CI_BuilderController_CPP.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class CASINO_INC_API ACI_BuilderController_CPP : public ACI_BaseStaffController_CPP
{
	GENERATED_BODY()

		ACI_BuilderController_CPP();

protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaSeconds) override;
	virtual void GetNewTask() override;
	virtual void TaskEnded_Implementation(bool success) override;
	virtual void DoIdle() override;
};
