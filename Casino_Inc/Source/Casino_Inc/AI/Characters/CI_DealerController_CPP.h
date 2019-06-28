// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Base/CI_BaseStaffController_CPP.h"
#include "CI_DealerController_CPP.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class CASINO_INC_API ACI_DealerController_CPP : public ACI_BaseStaffController_CPP
{
	GENERATED_BODY()

		ACI_DealerController_CPP();

public:
	virtual void Tick(float DeltaSeconds) override;
	virtual void GetNewTask() override;

protected:
	virtual void BeginPlay() override;
	virtual void DoIdle() override;
	virtual void EnterMap() override;
	virtual void LeaveMap() override;

public:
	void TaskEnded_Implementation(bool success) override;
protected:

	bool isIdle = true;
	FVector2D taskLocation;
	class ACI_BaseTile_CPP* taskInteractable;
};
