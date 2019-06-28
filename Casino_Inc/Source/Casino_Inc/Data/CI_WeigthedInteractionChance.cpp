// Fill out your copyright notice in the Description page of Project Settings.

#include "CI_WeigthedInteractionChance.h"
#include "DebugMacros.h"

void FWeightedInteractionResult::OnPostDataImport(const UDataTable* InDataTable, const FName InRowName,
	TArray<FString>& OutCollectedImportProblems)
{
	int totalChance = 0;

	for (auto node : InDataTable->GetRowMap())
	{
		totalChance += ((FWeightedInteractionResult*)node.Value)->chance;
	}

	percentageChange = (1.0f / totalChance) * chance;
}

void FInteractionResult::RecalculateChance()
{
	if (!IsValid(values))
		return;

	totalChance = 0;

	for (auto node : values->GetRowMap())
	{
		totalChance += ((FWeightedInteractionResult*)node.Value)->chance;
	}
}

FWeightedInteractionResult FInteractionResult::GetRandom()
{
	int element = FMath::RandRange(0, totalChance);
	int counter = 0;

	for (auto node : values->GetRowMap())
	{
		counter += ((FWeightedInteractionResult*)node.Value)->chance;

		if (counter >= element)
			return *(FWeightedInteractionResult*)node.Value;
	}

	return FWeightedInteractionResult();
}
