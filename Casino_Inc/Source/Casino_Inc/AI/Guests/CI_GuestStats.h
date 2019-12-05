#pragma once

#include "CoreMinimal.h"
#include "Curves/CurveFloat.h"
#include "Engine/DataTable.h"
#include "CI_GuestStats.generated.h"

//TODO:DOCUMENT comment/document this file

UENUM(BlueprintType)
enum class EStatLevel : uint8
{
	SL_Max		UMETA(DisplayName = "Max"),
	SL_High		UMETA(DisplayName = "High"),
	SL_Mid		UMETA(DisplayName = "Mid"),
	SL_Low		UMETA(DisplayName = "Low"),
	SL_Empty	UMETA(DisplayName = "Empty")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGuestStatChanged, EStatLevel, newLevel);

USTRUCT(BlueprintType)
struct FGuestStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float happiness = 0.0f;
	UPROPERTY(BlueprintReadOnly)
		EStatLevel happinessLevel = EStatLevel::SL_Mid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float chips = 0.0f;
	UPROPERTY(BlueprintReadOnly)
	EStatLevel chipsLevel = EStatLevel::SL_Mid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float money = 0.0f;
	UPROPERTY(BlueprintReadOnly)
	EStatLevel moneyLevel = EStatLevel::SL_Mid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float hunger = 0.0f;
	UPROPERTY(BlueprintReadOnly)
	EStatLevel hungerLevel = EStatLevel::SL_Mid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float thirst = 0.0f;
	UPROPERTY(BlueprintReadOnly)
	EStatLevel thirstLevel = EStatLevel::SL_Mid;

	UPROPERTY(BlueprintAssignable)
		FGuestStatChanged happinessChanged;

	UPROPERTY(BlueprintAssignable)
		FGuestStatChanged chipsChanged;

	UPROPERTY(BlueprintAssignable)
		FGuestStatChanged moneyChanged;

	UPROPERTY(BlueprintAssignable)
		FGuestStatChanged hungerChanged;

	UPROPERTY(BlueprintAssignable)
		FGuestStatChanged thirstChanged;

	void AddHappiness(float change);
	void AddChips(float change);
	void AddMoney(float change);
	void AddHunger(float change);
	void AddThirst(float change);

private:
	float happinessMax = 100.0f;
	float happinessHigh = 80.0f;
	float happinessMid = 30.0f;
	float happinessLow = 0.0f;

	float chipsMax = 500.0f;
	float chipsHigh = 250.0f;
	float chipsMid = 100.0f;
	float chipsLow = 0.0f;

	float moneyMax = 100.0f;
	float moneyHigh = 80.0f;
	float moneyMid = 30.0f;
	float moneyLow = 0.0f;

	float hungerMax = 100.0f;
	float hungerHigh = 80.0f;
	float hungerMid = 30.0f;
	float hungerLow = 0.0f;

	float thirstMax = 100.0f;
	float thirstHigh = 80.0f;
	float thirstMid = 30.0f;
	float thirstLow = 0.0f;
};

UENUM(BlueprintType)
enum class EInteractables : uint8
{
	INT_Slotmachine		UMETA(DisplayName = "Slotmachine"),
	INT_Blackjack		UMETA(DisplayName = "Blackjack"),
	INT_Poker			UMETA(DisplayName = "Poker")
};

USTRUCT(BlueprintType)
struct FGuestPersonalityPreference : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Meta=(DisplayName="Happiness Multiplier Slotmachine"))
		float happinessMultiplierSlotMachine;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Meta = (DisplayName = "Happiness Multiplier Blackjack"))
		float happinessMultiplierBlackJack;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Meta = (DisplayName = "Happiness Multiplier Poker"))
		float happinessMultiplierPoker;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Meta = (DisplayName = "Utility Value Slotmachine"))
		float utilityValueSlotmachine;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Meta = (DisplayName = "Utility Value BlackJack"))
		float utilityValueBlackjack;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Meta = (DisplayName = "Utility Value Poker"))
		float utilityValuePoker;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Meta = (DisplayName = "Happiness Multiplier Values"))
	TMap<EInteractables, float> happinessMultiplierValues;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Meta = (DisplayName = "Base Utility Values"))
	TMap<EInteractables, float> baseUtilityValues;

	virtual void OnPostDataImport(const UDataTable* InDataTable, const FName InRowName,
	                              TArray<FString>& OutCollectedImportProblems) override;
};
