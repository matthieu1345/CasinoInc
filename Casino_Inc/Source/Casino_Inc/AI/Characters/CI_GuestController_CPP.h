// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Base/CI_BaseAIController_CPP.h"
#include "AI/Guests/CI_GuestStats.h"
#include "CI_GuestController_CPP.generated.h"

//TODO:DOCUMENT comment/document this file

UENUM(BlueprintType)
enum class EGuestState : uint8
{
	GS_Idle,
	GS_Moving,
	GS_Interacting,
	GS_SearchingNewItem,
	GS_Leaving,
	GS_Away
};

/**
 * 
 */
UCLASS(Blueprintable)
class CASINO_INC_API ACI_GuestController_CPP : public ACI_BaseAIController_CPP
{

private:
	GENERATED_BODY()

public:
	ACI_GuestController_CPP();
	~ACI_GuestController_CPP();
	
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent);

	void TickInteracting(float DeltaSeconds);
	virtual void Tick(float DeltaSeconds) override;

	void InteractableDestroyed() { currentInteractionTile = nullptr; }

protected:
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
		void GetRandomPersonality();

	UFUNCTION(BlueprintCallable)
		void ResetStats();

	UFUNCTION(BlueprintCallable)
		void ResetGuest();

	UFUNCTION()
		void HappinessLevelChanged(EStatLevel newLevel);
	UFUNCTION()
		void ChipsLevelChanged(EStatLevel newLevel);
	UFUNCTION()
		void MoneyLevelChanged(EStatLevel newLevel);
	UFUNCTION()
		void HungerLevelChanged(EStatLevel newLevel);
	UFUNCTION()
		void ThirstLevelChanged(EStatLevel newLevel);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FGuestPersonalityPreference GetPreferences();

	UFUNCTION(BlueprintCallable)
		TSubclassOf<class UCI_InteractableTileData_CPP> GetNextInteractableType();

	UFUNCTION(BlueprintCallable)
		class UCI_InteractableTileData_CPP* GetNextGoal(TSubclassOf<class UCI_InteractableTileData_CPP> type);

	UFUNCTION(BlueprintCallable)
		void CalcultateNewGoal();

	UFUNCTION(BlueprintCallable)
		void ReachedGoal(bool successfull);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Guest|Stats")
		FGuestStats stats;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Guest|Preferences")
		FDataTableRowHandle preferenceTableRow;

	UPROPERTY(VisibleAnywhere, Category = "Guest|Preferences")
		FGuestPersonalityPreference preferences;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TArray<TSubclassOf<class UCI_InteractableTileData_CPP>> interactablesList;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Guest|stats|defaults")
		float StartingHappiness = 50.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Guest|stats|defaults")
		float maxStartingMoney = 500.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Guest|stats|defaults")
		float minStartingMoney = 100.0f;

	UPROPERTY(VisibleAnywhere)
	EGuestState state = EGuestState::GS_Idle;
	UPROPERTY(VisibleAnywhere)
	float interactionTimer = 0;
	UPROPERTY(VisibleAnywhere)
	class UCI_InteractableTileData_CPP* currentInteractionTile = nullptr;
	int searchInteractableCount = 0;
	int maxsearchInteractableCount = 10;
	float noInteractableFoundChange = -5.0f;
};
