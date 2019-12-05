#pragma once

#include "CI_GuestStats.h"
#include "Managers/CI_StatsManager_CPP.h"

//TODO:DOCUMENT comment/document this file

void FGuestStats::AddHappiness(float change)
{
	happiness += change;
	ACI_StatsManager_CPP::GetInstance()->ChangeHappiness(change);
	if (happiness >= happinessMax)
		happiness = happinessMax;

	if (happiness <= happinessLow)
		happiness = happinessLow;


	if (!happinessChanged.IsBound())
		return;

	if (happiness >= happinessMax)
	{
		if (happinessLevel != EStatLevel::SL_Max)
			happinessChanged.Broadcast(EStatLevel::SL_Max);

		happinessLevel = EStatLevel::SL_Max;
	}
	else if (happiness >= happinessHigh)
	{
		if (happinessLevel != EStatLevel::SL_High)
			happinessChanged.Broadcast(EStatLevel::SL_High);

		happinessLevel = EStatLevel::SL_Max;
	}
	else if (happiness >= happinessMid)
	{
		if (happinessLevel != EStatLevel::SL_Mid)
			happinessChanged.Broadcast(EStatLevel::SL_Mid);

		happinessLevel = EStatLevel::SL_Mid;
	}
	else if (happiness > happinessLow)
	{
		if (happinessLevel != EStatLevel::SL_Low)
			happinessChanged.Broadcast(EStatLevel::SL_Low);

		happinessLevel = EStatLevel::SL_Low;
	}
	else
	{
		if (happinessLevel != EStatLevel::SL_Empty)
			happinessChanged.Broadcast(EStatLevel::SL_Empty);

		happinessLevel = EStatLevel::SL_Empty;
	}
}

void FGuestStats::AddChips(float change)
{
	chips += change;
	if (chips <= chipsLow)
		chips = chipsLow;

	if (!chipsChanged.IsBound())
		return;

	if (chips >= chipsMax)
	{
		if (chipsLevel != EStatLevel::SL_Max)
			chipsChanged.Broadcast(EStatLevel::SL_Max);

		chipsLevel = EStatLevel::SL_Max;
	}
	else if (chips >= chipsHigh)
	{
		if (chipsLevel != EStatLevel::SL_High)
			chipsChanged.Broadcast(EStatLevel::SL_High);

		chipsLevel = EStatLevel::SL_High;
	}
	else if (chips >= chipsMid)
	{
		if (chipsLevel != EStatLevel::SL_Mid)
			chipsChanged.Broadcast(EStatLevel::SL_Mid);

		chipsLevel = EStatLevel::SL_Mid;
	}
	else if (chips > chipsLow)
	{
		if (chipsLevel != EStatLevel::SL_Low)
			chipsChanged.Broadcast(EStatLevel::SL_Low);

		chipsLevel = EStatLevel::SL_Low;
	}
	else
	{
		if (chipsLevel != EStatLevel::SL_Empty)
			chipsChanged.Broadcast(EStatLevel::SL_Empty);

		chipsLevel = EStatLevel::SL_Empty;
	}
}

void FGuestStats::AddMoney(float change)
{
	money += change;
	if (money <= moneyLow)
		money = moneyLow;

	if (!moneyChanged.IsBound())
		return;

	if (money >= moneyMax)
	{
		if (moneyLevel != EStatLevel::SL_Max)
			moneyChanged.Broadcast(EStatLevel::SL_Max);

		moneyLevel = EStatLevel::SL_Max;
	}
	else if (money >= moneyHigh)
	{
		if (moneyLevel != EStatLevel::SL_High)
			moneyChanged.Broadcast(EStatLevel::SL_High);

		moneyLevel = EStatLevel::SL_High;
	}
	else if (money >= moneyMid)
	{
		if (moneyLevel != EStatLevel::SL_Mid)
			moneyChanged.Broadcast(EStatLevel::SL_Mid);

		moneyLevel = EStatLevel::SL_Mid;
	}
	else if (money > moneyLow)
	{
		if (moneyLevel != EStatLevel::SL_Low)
			moneyChanged.Broadcast(EStatLevel::SL_Low);

		moneyLevel = EStatLevel::SL_Low;
	}
	else
	{
		if (moneyLevel != EStatLevel::SL_Empty)
			moneyChanged.Broadcast(EStatLevel::SL_Empty);

		moneyLevel = EStatLevel::SL_Empty;
	}
}

void FGuestStats::AddHunger(float change)
{
	hunger += change;
	if (hunger >= hungerMax)
		hunger = hungerMax;

	if (hunger <= hungerLow)
		hunger = hungerLow;

	if (!hungerChanged.IsBound())
		return;

	if (hunger >= hungerMax)
	{
		if (hungerLevel != EStatLevel::SL_Max)
			hungerChanged.Broadcast(EStatLevel::SL_Max);

		hungerLevel = EStatLevel::SL_Max;
	}
	else if (hunger >= hungerHigh)
	{
		if (hungerLevel != EStatLevel::SL_High)
			hungerChanged.Broadcast(EStatLevel::SL_High);

		hungerLevel = EStatLevel::SL_High;
	}
	else if (hunger >= hungerMid)
	{
		if (hungerLevel != EStatLevel::SL_Mid)
			hungerChanged.Broadcast(EStatLevel::SL_Mid);

		hungerLevel = EStatLevel::SL_Mid;
	}
	else if (hunger > hungerLow)
	{
		if (hungerLevel != EStatLevel::SL_Low)
			hungerChanged.Broadcast(EStatLevel::SL_Low);

		hungerLevel = EStatLevel::SL_Low;
	}
	else
	{
		if (hungerLevel != EStatLevel::SL_Empty)
			hungerChanged.Broadcast(EStatLevel::SL_Empty);

		hungerLevel = EStatLevel::SL_Empty;
	}
}

void FGuestStats::AddThirst(float change)
{
	thirst += change;
	if (thirst >= thirstMax)
		thirst = thirstMax;

	if (thirst <= thirstLow)
		thirst = thirstLow;

	if (!thirstChanged.IsBound())
		return;

	if (thirst >= thirstMax)
	{
		if (thirstLevel != EStatLevel::SL_Max)
			thirstChanged.Broadcast(EStatLevel::SL_Max);

		thirstLevel = EStatLevel::SL_Max;
	}
	else if (thirst >= thirstHigh)
	{
		if (thirstLevel != EStatLevel::SL_High)
			thirstChanged.Broadcast(EStatLevel::SL_High);

		thirstLevel = EStatLevel::SL_High;
	}
	else if (thirst >= thirstMid)
	{
		if (thirstLevel != EStatLevel::SL_Mid)
			thirstChanged.Broadcast(EStatLevel::SL_Mid);

		thirstLevel = EStatLevel::SL_Mid;
	}
	else if (thirst > thirstLow)
	{
		if (thirstLevel != EStatLevel::SL_Low)
			thirstChanged.Broadcast(EStatLevel::SL_Low);

		thirstLevel = EStatLevel::SL_Low;
	}
	else
	{
		if (thirstLevel != EStatLevel::SL_Empty)
			thirstChanged.Broadcast(EStatLevel::SL_Empty);

		thirstLevel = EStatLevel::SL_Empty;
	}
}

void FGuestPersonalityPreference::OnPostDataImport(const UDataTable* InDataTable, const FName InRowName,
	TArray<FString>& OutCollectedImportProblems)
{
	happinessMultiplierValues.Add(EInteractables::INT_Slotmachine, happinessMultiplierSlotMachine);
	happinessMultiplierValues.Add(EInteractables::INT_Poker, happinessMultiplierBlackJack);
	happinessMultiplierValues.Add(EInteractables::INT_Blackjack, happinessMultiplierPoker);

	baseUtilityValues.Add(EInteractables::INT_Slotmachine, utilityValueSlotmachine);
	baseUtilityValues.Add(EInteractables::INT_Poker, utilityValuePoker);
	baseUtilityValues.Add(EInteractables::INT_Blackjack, utilityValueBlackjack);
}
