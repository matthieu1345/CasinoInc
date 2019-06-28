// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AI/Guests/CI_GuestStats.h"
#include "CI_WeigthedInteractionChance.generated.h"

USTRUCT(BlueprintType)
struct FWeightedInteractionResult : public FTableRowBase
{
	GENERATED_BODY()

	virtual void OnPostDataImport(const UDataTable* InDataTable, const FName InRowName,
		TArray<FString>& OutCollectedImportProblems) override;

	UPROPERTY(EditAnywhere)
		int chance = 1;

	UPROPERTY(VisibleAnywhere)
		float percentageChange = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float happiness = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float chips = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float money = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float hunger = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float thirst = 0.0f;
};

USTRUCT(BlueprintType)
struct FInteractionResult
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Result")
		UDataTable* values;

	UPROPERTY(EditAnywhere, Category = "Condition")
		bool changesHappiness = false;
	UPROPERTY(EditAnywhere, Category = "Condition")
		bool changesChips = false;
	UPROPERTY(EditAnywhere, Category = "Condition")
		bool changesMoney = false;
	UPROPERTY(EditAnywhere, Category = "Condition")
		bool changesHunger = false;
	UPROPERTY(EditAnywhere, Category = "Condition")
		bool changesThirst = false;

	UPROPERTY(EditAnywhere, Category = "Condition")
		bool moneyInfluencesCasino = true;

	UPROPERTY(VisibleAnywhere, Category = Chance)
		int totalChance = 0;

	void RecalculateChance();

	FWeightedInteractionResult GetRandom();
};
