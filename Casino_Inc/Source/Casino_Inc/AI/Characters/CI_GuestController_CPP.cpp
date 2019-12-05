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

//TODO:DOCUMENT comment/document this file

ACI_GuestController_CPP::ACI_GuestController_CPP() : ACI_BaseAIController_CPP()
{
	type = ECharacterType::CT_Guest;
	stats.happinessChanged.AddDynamic(this, &ACI_GuestController_CPP::HappinessLevelChanged);
	stats.chipsChanged.AddDynamic(this, &ACI_GuestController_CPP::ChipsLevelChanged);
	stats.moneyChanged.AddDynamic(this, &ACI_GuestController_CPP::MoneyLevelChanged);
	stats.hungerChanged.AddDynamic(this, &ACI_GuestController_CPP::HungerLevelChanged);
	stats.thirstChanged.AddDynamic(this, &ACI_GuestController_CPP::ThirstLevelChanged);

	if (interactablesList.Num() == 0)
	{
		for (TObjectIterator<UClass> It; It; ++It)
		{
			if (It->IsChildOf(UCI_InteractableTileData_CPP::StaticClass()) && Cast<UCI_InteractableTileData_CPP>(It->GetDefaultObject())->GetRegisterName() != "" && !It->HasAnyClassFlags(CLASS_Abstract))
			{
				auto test = [&](TSubclassOf<UCI_InteractableTileData_CPP> Result) {return Cast<UCI_InteractableTileData_CPP>(It->GetDefaultObject())->GetRegisterName() == Cast<UCI_InteractableTileData_CPP>(Result->GetDefaultObject())->GetRegisterName(); };
				if (!interactablesList.ContainsByPredicate(test))
					interactablesList.Add(*It);
			}
		}
	}

	name = "Guest";
}

ACI_GuestController_CPP::~ACI_GuestController_CPP()
{

}

void ACI_GuestController_CPP::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	FName propertyName = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	preferences = *preferenceTableRow.GetRow<FGuestPersonalityPreference>(preferenceTableRow.RowName.ToString());
}

void ACI_GuestController_CPP::Tick(float DeltaSeconds)
{
	switch (state)
	{
	case EGuestState::GS_Moving:
		break;

	case EGuestState::GS_Interacting: 
		TickInteracting(DeltaSeconds);
		break;


	case EGuestState::GS_SearchingNewItem:
		CalcultateNewGoal();
		break;

	case EGuestState::GS_Leaving:
		if (!pathFollowingComp->IsActiveState() && !isAway)
			LeaveMap();
		break;

	case EGuestState::GS_Away:
		possessedCustomBasePawn->Destroy();
		Destroy();
		break;

	case EGuestState::GS_Idle:
		state = EGuestState::GS_SearchingNewItem;
		break;
	default: ;
	}


}

void ACI_GuestController_CPP::TickInteracting(float DeltaSeconds)
{
	bool finished = false;

	if (IsValid(currentInteractionTile))
	{
		interactionTimer += DeltaSeconds;

		if (interactionTimer >= currentInteractionTile->GetInteractTime())
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
		state = EGuestState::GS_Idle;
		interactionTimer = 0.0f;

		currentInteractionTile = nullptr;
		searchInteractableCount = 0;
	}
}

void ACI_GuestController_CPP::GetRandomPersonality()
{
	TArray<FName> allRows = preferenceTableRow.DataTable->GetRowNames();

	preferenceTableRow.RowName = allRows[FMath::RandRange(0, allRows.Num() - 1)];

	preferences = *preferenceTableRow.GetRow<FGuestPersonalityPreference>(preferenceTableRow.RowName.ToString());
}

void ACI_GuestController_CPP::ResetStats()
{
	stats.happiness = StartingHappiness;
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

void ACI_GuestController_CPP::HappinessLevelChanged(EStatLevel newLevel)
{
	switch (newLevel)
	{
	case EStatLevel::SL_Max: break;
	case EStatLevel::SL_High: break;
	case EStatLevel::SL_Mid: break;
	case EStatLevel::SL_Low: break;
	case EStatLevel::SL_Empty: 
		state = EGuestState::GS_Leaving;
		break;
	default: ;
	}
}

void ACI_GuestController_CPP::ChipsLevelChanged(EStatLevel newLevel)
{
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

void ACI_GuestController_CPP::MoneyLevelChanged(EStatLevel newLevel)
{
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

void ACI_GuestController_CPP::HungerLevelChanged(EStatLevel newLevel)
{
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

void ACI_GuestController_CPP::ThirstLevelChanged(EStatLevel newLevel)
{
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

FGuestPersonalityPreference ACI_GuestController_CPP::GetPreferences()
{
	return *preferenceTableRow.GetRow<FGuestPersonalityPreference>(preferenceTableRow.RowName.ToString());
}

TSubclassOf<UCI_InteractableTileData_CPP> ACI_GuestController_CPP::GetNextInteractableType()
{
	TMap<TSubclassOf<UCI_InteractableTileData_CPP>, float> interactableMap;

	float totalScore = 0.0f;

	for (TSubclassOf<UCI_InteractableTileData_CPP> interactable : interactablesList)
	{
		float score = UCI_InteractableTileData_CPP::GetUtilityScoreDefault(interactable, stats, GetPreferences());
		interactableMap.Add(interactable, score);

		totalScore += score;
	}

	if (FMath::IsNearlyZero(totalScore, 0.1f))
		return TSubclassOf<UCI_InteractableTileData_CPP>();

	float random = FMath::RandRange(0.0f, totalScore);

	for (TTuple<TSubclassOf<UCI_InteractableTileData_CPP>, float> element : interactableMap)
	{
		random -= element.Value;

		if (random <= 0.0f)
		{
			return element.Key;
		}
	}

	return TSubclassOf<UCI_InteractableTileData_CPP>();
}

UCI_InteractableTileData_CPP* ACI_GuestController_CPP::GetNextGoal(TSubclassOf<UCI_InteractableTileData_CPP> type)
{
	FString typeName = type->GetDefaultObject<UCI_InteractableTileData_CPP>()->GetRegisterName();
	auto result = Cast<UCI_InteractableTileData_CPP>(ACI_AIManager_CPP::GetInstance(GetWorld())->GetRandomRegisteredTile(typeName));

	return result;
}

void ACI_GuestController_CPP::CalcultateNewGoal()
{
	TSubclassOf<UCI_InteractableTileData_CPP> type = GetNextInteractableType();

	if (type == nullptr)
	{
		state = EGuestState::GS_Leaving;
		return;
	}

	currentInteractionTile = GetNextGoal(type);
	if (currentInteractionTile != nullptr && currentInteractionTile->IsFree())
	{
		auto location = currentInteractionTile->Reserve(possessedCustomBasePawn);
		if (!UCI_TileMapCoordinateMath::WorldVectorToTile(GetPawn()->GetActorLocation()).Equals(location, 1.0f))
		{
			pathFollowingComp->GetNewPath(UCI_TileMapCoordinateMath::WorldVectorToTile(GetPawn()->GetActorLocation()), location);
			state = EGuestState::GS_Moving;
		}
		else
		{
			state = EGuestState::GS_Interacting;
		}
	}
	else
	{
		if (searchInteractableCount > maxsearchInteractableCount && !isAway)
		{
			stats.AddHappiness(noInteractableFoundChange);
			state = EGuestState::GS_Leaving;
		}

		searchInteractableCount++;
	}
}

void ACI_GuestController_CPP::ReachedGoal(bool successfull)
{
	switch (state)
	{
	case EGuestState::GS_Moving:
		state = EGuestState::GS_Interacting;
		break;
	case EGuestState::GS_Leaving:
		state = EGuestState::GS_Away;
		break;
	default:
		state = EGuestState::GS_Idle;
		break;
	}
}

void ACI_GuestController_CPP::BeginPlay()
{
	Super::BeginPlay();
	ResetGuest();

	goapWorkerComponent->startPath.Clear();
	goapWorkerComponent->finishedCallback.Clear();
	pathFollowingComp->pathFinishedDelegate.Clear();

	pathFollowingComp->pathFinishedDelegate.AddDynamic(this, &ACI_GuestController_CPP::ReachedGoal);
}
