// Fill out your copyright notice in the Description page of Project Settings.

#include "CI_WeigthedInteractionChance.h"
#include "DebugMacros.h"

//TODO:DOCUMENT comment/document this file

void FWeightedInteractionResult::OnPostDataImport(const UDataTable* inDataTable, const FName inRowName,
												  TArray<FString>& outCollectedImportProblems)
{
	Super::OnPostDataImport(inDataTable, inRowName, outCollectedImportProblems);
	int totalChance = 0;

	for ( const auto &node : inDataTable->GetRowMap())
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

	for (auto const &node : values->GetRowMap())
	{
		totalChance += ((FWeightedInteractionResult*)node.Value)->chance;
	}
}

FWeightedInteractionResult FInteractionResult::GetRandom() const
{
	const int element = FMath::RandRange(0, totalChance);
	int counter = 0;

	for (auto const &node : values->GetRowMap())
	{
		counter += ((FWeightedInteractionResult*)node.Value)->chance;

		if (counter >= element)
			return *(FWeightedInteractionResult*)node.Value;
	}

	return FWeightedInteractionResult();
}
