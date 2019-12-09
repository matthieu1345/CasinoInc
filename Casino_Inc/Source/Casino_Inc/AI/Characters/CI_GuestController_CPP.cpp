// Fill out your copyright notice in the Description page of Project Settings.

#include "CI_GuestController_CPP.h"
#include "DebugMacros.h"
#include "TileMap/CI_InteractableTileData_CPP.h"
#include "AI/CI_AIManager_CPP.h"
#include "PathFinding/CI_PathFollowingComponent_CPP.h"
#include "TileMap/CI_TileMapCoordinateMath.h"
#include "AI/GOAP/CI_GOAPWorkerComponent_CPP.h"
#include "AI/Base/CI_BaseAIPawn_CPP.h"
#include "UObjectIterator.h"

ACI_GuestController_CPP::ACI_GuestController_CPP() : ACI_BaseAIController_CPP()
{
	// set defaults for the guest
	type = ECharacterType::CT_Guest;
	name = "Guest";

	// register the stat level change functions
	stats.happinessChanged.AddDynamic(this, &ACI_GuestController_CPP::HappinessLevelChanged);
	stats.chipsChanged.AddDynamic(this, &ACI_GuestController_CPP::ChipsLevelChanged);
	stats.moneyChanged.AddDynamic(this, &ACI_GuestController_CPP::MoneyLevelChanged);
	stats.hungerChanged.AddDynamic(this, &ACI_GuestController_CPP::HungerLevelChanged);
	stats.thirstChanged.AddDynamic(this, &ACI_GuestController_CPP::ThirstLevelChanged);

	// find all interactable types
	if (interactablesList.Num() == 0)
	{
		for (TObjectIterator<UClass> it; it; ++it)
		{
			if (it->IsChildOf(UCI_InteractableTileData_CPP::StaticClass()) && Cast<UCI_InteractableTileData_CPP>(it->GetDefaultObject())->GetRegisterName() != "" && !it->HasAnyClassFlags(CLASS_Abstract))
			{
				const auto test = [&](TSubclassOf<UCI_InteractableTileData_CPP> result) {return Cast<UCI_InteractableTileData_CPP>(it->GetDefaultObject())->GetRegisterName() == Cast<UCI_InteractableTileData_CPP>(result->GetDefaultObject())->GetRegisterName(); };
				if (!interactablesList.ContainsByPredicate(test))
					interactablesList.Add(*it);
			}
		}
	}
}

ACI_GuestController_CPP::~ACI_GuestController_CPP()
{

}

void ACI_GuestController_CPP::PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent)
{
	// FName propertyName = (propertyChangedEvent.Property != nullptr) ? propertyChangedEvent.Property->GetFName() : NAME_None;

	// get the preferences out of the table
	preferences = *preferenceTableRow.GetRow<FGuestPersonalityPreference>(preferenceTableRow.RowName.ToString());
}

void ACI_GuestController_CPP::Tick(const float deltaSeconds)
{
	switch (state)
	{
	case EGuestState::GS_Moving: // don't do anything when the guest is moving
		break;

	case EGuestState::GS_Interacting: 
		TickInteracting(deltaSeconds); // tick the interacting behaviour
		break;


	case EGuestState::GS_SearchingNewItem:
		CalcultateNewGoal();
		break;

	case EGuestState::GS_Leaving:
		if (!pathFollowingComp->IsActiveState() && !isAway)
			LeaveMap(); // call the leaving map logic when not doing that yet
		break;

	case EGuestState::GS_Away:
		possessedCustomBasePawn->Destroy();
		Destroy(); // destroy the pawn when we're gone from the casino
		break;

	case EGuestState::GS_Idle:
		state = EGuestState::GS_SearchingNewItem; // change to searching for a new goal when we're not doing anything
		break;
	default: ;
	}


}

void ACI_GuestController_CPP::TickInteracting(const float deltaSeconds)
{
	bool finished = false;

	if (IsValid(currentInteractionTile)) // if the current interaction goal is still valid tick the interaction
	{
		interactionTimer += deltaSeconds;

		if (interactionTimer >= currentInteractionTile->GetInteractTime()) // if we're finished interacting, call the interact logic to update stats
		{
			currentInteractionTile->Interact(stats, possessedCustomBasePawn, GetPreferences());
			finished = true;
		}
	}
	else
	{
		finished = true;
	}



	if (finished)
	{
		// reset the flags and references when we're done with this interaction
		state = EGuestState::GS_Idle;
		interactionTimer = 0.0f;
		currentInteractionTile = nullptr;
		searchInteractableCount = 0;
	}
}

void ACI_GuestController_CPP::GetRandomPersonality()
{
	TArray<FName> allRows = preferenceTableRow.DataTable->GetRowNames(); // get all possible personalitys

	preferenceTableRow.RowName = allRows[FMath::RandRange(0, allRows.Num() - 1)]; // get a random personality from the list

	preferences = *preferenceTableRow.GetRow<FGuestPersonalityPreference>(preferenceTableRow.RowName.ToString()); // update it to the preference structure
}

void ACI_GuestController_CPP::ResetStats()
{
	// set all stats to their defaults
	stats.happiness = startingHappiness;
	stats.money = FMath::RandRange(minStartingMoney, maxStartingMoney);
	stats.chips = 0.0f;
	stats.hunger = 0.0f;
	stats.thirst = 0.0f;
}

void ACI_GuestController_CPP::ResetGuest()
{
	ResetStats();
	GetRandomPersonality();
	isAway = false;
}

void ACI_GuestController_CPP::HappinessLevelChanged(const EStatLevel newLevel)
{
	switch (newLevel)
	{
	case EStatLevel::SL_Max: break;
	case EStatLevel::SL_High: break;
	case EStatLevel::SL_Mid: break;
	case EStatLevel::SL_Low: break;
	case EStatLevel::SL_Empty: 
		state = EGuestState::GS_Leaving; // leave when we're not happy anymore
		break;
	default: ;
	}
}

// ReSharper disable once CppMemberFunctionMayBeStatic this will never be static, cause it's been called on a specific ai who's state has changed
// ReSharper disable once CppMemberFunctionMayBeConst this will not be const, ai state changes can and will directly change stuff about them
void ACI_GuestController_CPP::ChipsLevelChanged(const EStatLevel newLevel)
{
	// we don't really do anything with this instantly
	switch (newLevel)
	{
	case EStatLevel::SL_Max: break;
	case EStatLevel::SL_High: break;
	case EStatLevel::SL_Mid: break;
	case EStatLevel::SL_Low: break;
	case EStatLevel::SL_Empty: break;
	default:;
	}
}

// ReSharper disable once CppMemberFunctionMayBeStatic this will never be static, cause it's been called on a specific ai who's state has changed
// ReSharper disable once CppMemberFunctionMayBeConst this will not be const, ai state changes can and will directly change stuff about them
void ACI_GuestController_CPP::MoneyLevelChanged(const EStatLevel newLevel)
{
	// we don't really do anything with this instantly
	switch (newLevel)
	{
	case EStatLevel::SL_Max: break;
	case EStatLevel::SL_High: break;
	case EStatLevel::SL_Mid: break;
	case EStatLevel::SL_Low: break;
	case EStatLevel::SL_Empty: break;
	default:;
	}
}

// ReSharper disable once CppMemberFunctionMayBeStatic this will never be static, cause it's been called on a specific ai who's state has changed
// ReSharper disable once CppMemberFunctionMayBeConst this will not be const, ai state changes can and will directly change stuff about them
void ACI_GuestController_CPP::HungerLevelChanged(const EStatLevel newLevel)
{
	// we don't really do anything with this instantly
	switch (newLevel)
	{
	case EStatLevel::SL_Max: break;
	case EStatLevel::SL_High: break;
	case EStatLevel::SL_Mid: break;
	case EStatLevel::SL_Low: break;
	case EStatLevel::SL_Empty: break;
	default:;
	}
}

// ReSharper disable once CppMemberFunctionMayBeStatic this will never be static, cause it's been called on a specific ai who's state has changed
// ReSharper disable once CppMemberFunctionMayBeConst this will not be const, ai state changes can and will directly change stuff about them
void ACI_GuestController_CPP::ThirstLevelChanged(const EStatLevel newLevel)
{
	// we don't really do anything with this instantly
	switch (newLevel)
	{
	case EStatLevel::SL_Max: break;
	case EStatLevel::SL_High: break;
	case EStatLevel::SL_Mid: break;
	case EStatLevel::SL_Low: break;
	case EStatLevel::SL_Empty: break;
	default:;
	}
}

FGuestPersonalityPreference ACI_GuestController_CPP::GetPreferences() const
{
	return *preferenceTableRow.GetRow<FGuestPersonalityPreference>(preferenceTableRow.RowName.ToString());
}

TSubclassOf<UCI_InteractableTileData_CPP> ACI_GuestController_CPP::GetNextInteractableType()
{
	// a map with score for each interactable type
	TMap<TSubclassOf<UCI_InteractableTileData_CPP>, float> interactableMap;

	// the total score
	float totalScore = 0.0f;

	for ( const TSubclassOf<UCI_InteractableTileData_CPP> interactable : interactablesList) // loop over every interactable
	{
		float score = UCI_InteractableTileData_CPP::GetUtilityScoreDefault(interactable, stats, GetPreferences()); // calculate the score for this interactable
		interactableMap.Add(interactable, score); // add interactable and score to the map

		totalScore += score; // add the score to the total score
	}

	if (FMath::IsNearlyZero(totalScore, 0.1f))
		return TSubclassOf<UCI_InteractableTileData_CPP>(); // don't do anything if the total score ~ 0 this means there's no valid interactable

	float random = FMath::RandRange(0.0f, totalScore); // select a random score

	for ( const TTuple<TSubclassOf<UCI_InteractableTileData_CPP>, float> element : interactableMap) // loop over every interactable
	{
		random -= element.Value; // remove the element's score from the random score

		if (random <= 0.0f) // if the random score has gotten below 0, we found the randomly selected interactable!
		{
			return element.Key;
		}
	}

	return TSubclassOf<UCI_InteractableTileData_CPP>();
}

UCI_InteractableTileData_CPP* ACI_GuestController_CPP::GetNextGoal(TSubclassOf<UCI_InteractableTileData_CPP> type) const
{
	// get the name of the type
	const FString typeName = type->GetDefaultObject<UCI_InteractableTileData_CPP>()->GetRegisterName();
	// get a random interactable with that name
	const auto result = Cast<UCI_InteractableTileData_CPP>(ACI_AIManager_CPP::GetInstance(GetWorld())->GetRandomRegisteredTile(typeName));

	return result;
}

void ACI_GuestController_CPP::CalcultateNewGoal()
{
	const TSubclassOf<UCI_InteractableTileData_CPP> type = GetNextInteractableType();

	// if there's no valid interactable type leave the casino
	if (type == nullptr)
	{
		state = EGuestState::GS_Leaving;
		return;
	}

	currentInteractionTile = GetNextGoal(type);

	if (currentInteractionTile != nullptr && currentInteractionTile->IsFree()) // when we can use the interactable
	{
		// reserve the interactable
		const auto location = currentInteractionTile->Reserve(possessedCustomBasePawn);

		// if we're not at the location yet, move there
		if (!UCI_TileMapCoordinateMath::WorldVectorToTile(GetPawn()->GetActorLocation()).Equals(location, 1.0f))
		{
			pathFollowingComp->GetNewPath(UCI_TileMapCoordinateMath::WorldVectorToTile(GetPawn()->GetActorLocation()), location);
			state = EGuestState::GS_Moving;
		}
		else // directly start interacting
		{
			state = EGuestState::GS_Interacting;
		}
	}
	else //if the interactable is invalid or in use
	{
		if (searchInteractableCount > maxsearchInteractableCount && !isAway) // if we reached the max amount of checked interactables, leave the casino
		{
			stats.AddHappiness(noInteractableFoundChange);
			state = EGuestState::GS_Leaving;
		}

		searchInteractableCount++;
	}
}

void ACI_GuestController_CPP::ReachedGoal(bool successfull)
{

	// what kind of goal did we reach?
	switch (state)
	{
	case EGuestState::GS_Moving: // moving goal -> change state to interacting
		state = EGuestState::GS_Interacting;
		break;
	case EGuestState::GS_Leaving: // leaving goal -> change state to away
		state = EGuestState::GS_Away;
		break;
	default: // anything else -> change state to idle
		state = EGuestState::GS_Idle;
		break;
	}
}

void ACI_GuestController_CPP::BeginPlay()
{
	Super::BeginPlay();
	ResetGuest();

	// reset the different components
	goapWorkerComponent->startPath.Clear();
	goapWorkerComponent->finishedCallback.Clear();
	pathFollowingComp->pathFinishedDelegate.Clear();

	// register the reached goal function to the path following component
	pathFollowingComp->pathFinishedDelegate.AddDynamic(this, &ACI_GuestController_CPP::ReachedGoal);
}
