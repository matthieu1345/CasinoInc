// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AI/GOAP/CI_GOAPStateContainer_CPP.h"
#include "CI_GOAPActionBase_CPP.generated.h"

//TODO:DOCUMENT comment/document this file

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class CASINO_INC_API UCI_GOAPActionBase_CPP : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FGOAPStateList preConditions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FGOAPStateList effects;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float cost = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float excecuteTime = 1.0f;

	UFUNCTION(BlueprintCallable)
		bool CheckPreConditions(FGOAPStateList startState, int &difference) const;
	UFUNCTION(BlueprintCallable)
		bool CheckGoalConditions(FGOAPStateList goalState, int &difference) const;

	UFUNCTION(BlueprintCallable)
		bool AddEffects(FGOAPStateList startState, FGOAPStateList &resultState, UObject* outer);
	UFUNCTION(BlueprintCallable)
		bool RevertEffects(FGOAPStateList goalState, FGOAPStateList &resultState, UObject* outer);

	UFUNCTION(BlueprintCallable)
		void InitAction(FVector2D goalLocation, bool needsMove);

	//returns true if finished
	virtual bool DoAction(float deltaTime, FGOAPStateList& startState, class UCI_GOAPWorkerComponent_CPP* workerComponent);

	virtual bool IsPostLoadThreadSafe() const override { return true; }

	UPROPERTY(EditAnywhere)
		bool needsMove;

	UPROPERTY(EditAnywhere)
		FString registeryItem;

protected:
	FVector2D location;

	float counter = 0.0f;
};
