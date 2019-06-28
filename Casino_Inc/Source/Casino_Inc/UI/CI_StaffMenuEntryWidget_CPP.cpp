// Fill out your copyright notice in the Description page of Project Settings.

#include "CI_StaffMenuEntryWidget_CPP.h"
#include "AI/Base/CI_BaseStaffController_CPP.h"
#include "AI/Base/CI_BaseAIPawn_CPP.h"
#include "Components/Slider.h"

void UCI_StaffMenuEntryWidget_CPP::NativeConstruct()
{
	Super::NativeConstruct();

	linkedController = Cast<ACI_BaseStaffController_CPP>(linkedPawn->GetBaseController());

	if (CheckSlidersValid())
	{
		workStartSlider->SetValue(hourStep * linkedController->startWorkTime);
		workStartSlider->OnValueChanged.AddDynamic(this, &UCI_StaffMenuEntryWidget_CPP::StartingTimeChanged);
		workHourCountSlider->SetValue(hourStep * linkedController->workStintLength);
		workHourCountSlider->OnValueChanged.AddDynamic(this, &UCI_StaffMenuEntryWidget_CPP::WorkAmountChanged);
		UpdateEndTime();
	}
}

bool UCI_StaffMenuEntryWidget_CPP::CheckSlidersValid()
{
	return workEndSlider != nullptr && workStartSlider != nullptr && workHourCountSlider != nullptr;
}

void UCI_StaffMenuEntryWidget_CPP::StartingTimeChanged(float newValue)
{
	linkedController->startWorkTime = FMath::FloorToInt(newValue * 24);
	if (linkedController->startWorkTime >= 24)
	{
		linkedController->startWorkTime -= 24;
	}
	UpdateEndTime();
}

void UCI_StaffMenuEntryWidget_CPP::WorkAmountChanged(float newValue)
{
	linkedController->workStintLength = FMath::FloorToInt(newValue * 24);

	UpdateEndTime();
}

void UCI_StaffMenuEntryWidget_CPP::UpdateEndTime()
{
	if (!CheckSlidersValid())
		return;

	linkedController->endWorkTime = linkedController->startWorkTime + linkedController->workStintLength;
	if (linkedController->endWorkTime > 24)
		linkedController->endWorkTime -= 24;

	workEndSliderValue = hourStep * linkedController->endWorkTime;

}
