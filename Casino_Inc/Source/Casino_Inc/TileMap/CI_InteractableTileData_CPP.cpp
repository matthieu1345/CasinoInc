// Fill out your copyright notice in the Description page of Project Settings.

#include "CI_InteractableTileData_CPP.h"
#include "Data/CI_WeigthedInteractionChance.h"
#include "Managers/CI_StatsManager_CPP.h"
#include "CI_TileMap_CPP.h"
#include "TimerManager.h"
#include "AI/CI_AIManager_CPP.h"
#include "AI/Base/CI_BaseAIPawn_CPP.h"
#include "AI/Characters/CI_GuestController_CPP.h"

//TODO:DOCUMENT comment/document this file

UCI_InteractableTileData_CPP::UCI_InteractableTileData_CPP() : Super()
{
	results.RecalculateChance();
}

void UCI_InteractableTileData_CPP::DestroyComponent_Implementation(ACI_TileMap_CPP* tileMap, const int x, const int y)
{
	Super::DestroyComponent_Implementation(tileMap, x, y);

	for (auto &it : interactionOffsets)
	{
		if (it.Value != nullptr)
		{
			Cast<ACI_GuestController_CPP>(it.Value->GetBaseController())->InteractableDestroyed();
			it.Value = nullptr;
		}
	}
}

void UCI_InteractableTileData_CPP::SetInfo_Implementation(const int32 x, const int32 y, ACI_TileMap_CPP* tileMap)
{
	Super::SetInfo_Implementation(x, y, tileMap);

	if (!interactsInternal)
	{
		UpdateInteractLocations();

		GetOwner()->GetWorldTimerManager().SetTimer(updateLocationsHandle, this, &UCI_InteractableTileData_CPP::UpdateInteractLocations, 5.0f, true);
	}

	if (needsDealer)
		DealerLeft();
}


void UCI_InteractableTileData_CPP::Interact(FGuestStats& stats, class ACI_BaseAIPawn_CPP* askingPawn, FGuestPersonalityPreference preference)
{
	FWeightedInteractionResult result;
	
	for (auto &it : interactionOffsets)
	{
		if (it.Value == askingPawn)
		{
			it.Value = nullptr;
			break;
		}
	}

	if (hasOverridenResult)
		result = GetOverridenResult(stats, preference);
	else
		result = results.GetRandom();

	if (hasOverridenResult || results.changesHappiness)
	{
		stats.AddHappiness(result.happiness * preference.happinessMultiplierValues[type]);
	}

	if (hasOverridenResult || results.changesChips)
	{
		stats.AddChips(result.chips);
	}

	if (hasOverridenResult || results.changesMoney)
	{
		stats.AddMoney(result.money);

		ACI_StatsManager_CPP::GetInstance(GetWorld())->AddMoney(-result.money); //TODO: only when the casino should get money
	}

	if (hasOverridenResult || results.changesHunger)
	{
		stats.AddHunger(result.hunger);
	}

	if (hasOverridenResult || results.changesThirst)
	{
		stats.AddThirst(result.thirst);
	}
}

bool UCI_InteractableTileData_CPP::IsFree()
{
	if (needsDealer && !hasDealer)
		return false;

	for (auto &it : interactionOffsets)
	{
		if (it.Value == nullptr)
			return true;
	}

	return false;
}

FVector2D UCI_InteractableTileData_CPP::Reserve(class ACI_BaseAIPawn_CPP* askingPawn)
{
	for (auto &it : interactionOffsets)
	{
		if (it.Value == nullptr)
		{
			it.Value = askingPawn;
			return it.Key + GetRegisterLocation();
		}
	}

	return GetRegisterLocation();
}

void UCI_InteractableTileData_CPP::DealerLeft()
{
	hasDealer = false;

	ACI_AIManager_CPP::GetAIManagerInstance()->CreateNewTask(dealerTask, GetTileX() + dealerOffset.X, GetTileY() + dealerOffset.Y, GetTile());
}

void UCI_InteractableTileData_CPP::BeginPlay()
{
	Super::BeginPlay();

	interactionOffsets.GenerateKeyArray(defaultInteractOffsets);

	if (results.values == nullptr)
		return;

	static TMap<UDataTable*, FInteractionResult> lookupTable;
	const auto ref = lookupTable.Find(results.values);
	if (ref != nullptr)
	{
		results = *ref;
	}
	else
	{
		results.RecalculateChance();
		lookupTable.Add(results.values, results);
	}
}

void UCI_InteractableTileData_CPP::UpdateInteractLocations()
{
	if (interactsInternal)
		return;

	TArray<FVector2D> usedLocations;

	for (auto &it : defaultInteractOffsets)
	{
		const FVector2D location = GetRegisterLocation() + it;
		if (!tileMap->CheckWalkable(location.X, location.Y))
		{
			if (interactionOffsets.Contains(it))
				usedLocations.Add(it);
			continue;
		}

		if (interactionOffsets.Contains(it))
			continue;
			
		interactionOffsets.Add(it, nullptr);
	}

	for ( const auto location : usedLocations)
		interactionOffsets.Remove(location);
}

float UCI_InteractableTileData_CPP::GetUtilityScore_Implementation(const FGuestStats stats,
	const FGuestPersonalityPreference preference)
{
	return defaultUtilityScore + preference.baseUtilityValues[type];
}

float UCI_InteractableTileData_CPP::GetUtilityScoreDefault(TSubclassOf<UCI_InteractableTileData_CPP> defaultClass,
	const FGuestStats stats, const FGuestPersonalityPreference preference)
{
	return defaultClass->GetDefaultObject<UCI_InteractableTileData_CPP>()->GetUtilityScore(stats, preference);
}
