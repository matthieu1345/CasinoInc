// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Base/CI_BaseAIController_CPP.h"
#include "AI/Guests/CI_GuestStats.h"
#include "CI_GuestController_CPP.generated.h"

/**
 * \brief Enumerator for the different states a guest can be in
 */
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
 * Controller for the <b>Guest Controller</b> for Casino Inc.<br>
 * has all Guest specific logic
 */
UCLASS(Blueprintable)
class CASINO_INC_API ACI_GuestController_CPP : public ACI_BaseAIController_CPP
{
	GENERATED_BODY()

public:
	ACI_GuestController_CPP();
	~ACI_GuestController_CPP();
	
	/**
	 * \brief called when you change a property in the editor
	 * \param propertyChangedEvent The property that's changed
	 */
	void PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent) override;

	/**
	 * \brief custom tick for when the ai is in the <b>GS_Interacting</b> state
	 */
	void TickInteracting(float deltaSeconds);
	
	/**
	 *	\brief Function called every frame on this Actor. Override this function to implement custom logic to be executed every frame.
	 *	\param deltaSeconds	Game time elapsed during last frame modified by the time dilation
	 */
	virtual void Tick(float deltaSeconds) override;


	/**
	 * \brief a function that's called when the interactable tile is being destroyed to reset references
	 */
	void InteractableDestroyed() { currentInteractionTile = nullptr; }

protected:
	/**
	 *\brief native event for when play begins for this actor.
	 */
	virtual void BeginPlay() override;

	/**
	 * \brief function to randomize the guest's personality
	 */
	UFUNCTION(BlueprintCallable)
		void GetRandomPersonality();

	/**
	 * \brief reset's the guests stats to the default values
	 */
	UFUNCTION(BlueprintCallable)
		void ResetStats();

	/**
	 * \brief reset everything about the guest
	 */
	UFUNCTION(BlueprintCallable)
		void ResetGuest();

	/**
	 * \brief callback for when the level of <b>happiness</b> has changed
	 * \param newLevel the new level that the <b>happiness</b> is at
	 */
	UFUNCTION()
		void HappinessLevelChanged(EStatLevel newLevel);
	/**
	 * \brief callback for when the level of <b>Chips</b> has changed
	 * \param newLevel the new level that the <b>Chips</b> is at
	 */
	UFUNCTION()
		void ChipsLevelChanged(EStatLevel newLevel);
	/**
	 * \brief callback for when the level of <b>Money</b> has changed
	 * \param newLevel the new level that the <b>Money</b> is at
	 */
	UFUNCTION()
		void MoneyLevelChanged(EStatLevel newLevel);
	/**
	 * \brief callback for when the level of <b>Hunger</b> has changed
	 * \param newLevel the new level that the <b>Hunger</b> is at
	 */
	UFUNCTION()
		void HungerLevelChanged(EStatLevel newLevel);
	/**
	 * \brief callback for when the level of <b>Thirst</b> has changed
	 * \param newLevel the new level that the <b>Thirst</b> is at
	 */
	UFUNCTION()
		void ThirstLevelChanged(EStatLevel newLevel);

	/**
	 * \return the preference of this guest
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
		FGuestPersonalityPreference GetPreferences() const;

	/**
	 * \brief select a random interactable type that this guest want's to use next
	 * \return the selected interactable type
	 */
	UFUNCTION(BlueprintCallable)
		TSubclassOf<class UCI_InteractableTileData_CPP> GetNextInteractableType();

	/**
	 * \brief get a random next goal of the specified type
	 * \param type the type of the next goal
	 * \return the selected goal
	 */
	UFUNCTION(BlueprintCallable)
		class UCI_InteractableTileData_CPP* GetNextGoal(TSubclassOf<class UCI_InteractableTileData_CPP> type) const;

	/**
	 * \brief find a new goal for the guest to go toward
	 */
	UFUNCTION(BlueprintCallable)
		void CalcultateNewGoal();

	/**
	 * \brief callback for when the guest has reached it's goal
	 * \param successfull has the goal been reached successfull?
	 */
	UFUNCTION(BlueprintCallable)
		void ReachedGoal(bool successfull);

protected:
	// The stats of the guest
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Guest|Stats")
		FGuestStats stats;

	// The preference table row of the guest
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Guest|Preferences")
		FDataTableRowHandle preferenceTableRow;

	// The preferences of the guest as neat struct
	UPROPERTY(VisibleAnywhere, Category = "Guest|Preferences")
		FGuestPersonalityPreference preferences;

	// List of interactable tiles
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TArray<TSubclassOf<class UCI_InteractableTileData_CPP>> interactablesList;

	// The starting happiness level of Guests
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Guest|stats|defaults")
		float startingHappiness = 50.0f;

	// The max money a guest can start with
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Guest|stats|defaults")
		float maxStartingMoney = 500.0f;

	// The minimum money a guest can start with
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Guest|stats|defaults")
		float minStartingMoney = 100.0f;

	// The current state of the guest
	UPROPERTY(VisibleAnywhere)
	EGuestState state = EGuestState::GS_Idle;

	// The current time the guest has interacted with it's current goal
	UPROPERTY(VisibleAnywhere)
	float interactionTimer = 0;

	// The current tile the guest is interacting with
	UPROPERTY(VisibleAnywhere)
	class UCI_InteractableTileData_CPP* currentInteractionTile = nullptr;

	// The amount of interactables checked
	int searchInteractableCount = 0;
	// The max amount of interactables to check
	int maxsearchInteractableCount = 10;
	// Happiness change when there's no interactable found
	float noInteractableFoundChange = -5.0f;
};
