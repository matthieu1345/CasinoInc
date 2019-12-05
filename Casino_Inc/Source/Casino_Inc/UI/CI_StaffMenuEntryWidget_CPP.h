// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CI_StaffMenuEntryWidget_CPP.generated.h"

//TODO:DOCUMENT comment/document this file

/**
 * 
 */
UCLASS()
class CASINO_INC_API UCI_StaffMenuEntryWidget_CPP : public UUserWidget
{
	GENERATED_BODY()


protected:
	virtual void NativeConstruct() override;

	bool CheckSlidersValid();

	UFUNCTION(BlueprintCallable)
		void StartingTimeChanged(float newValue);
	UFUNCTION(BlueprintCallable)
		void WorkAmountChanged(float newValue);

	void UpdateEndTime();

protected:
	UPROPERTY(BlueprintReadOnly, Meta=(ExposeOnSpawn=true))
		class ACI_BaseAIPawn_CPP* linkedPawn;
	UPROPERTY(BlueprintReadOnly)
	class ACI_BaseStaffController_CPP* linkedController;
	UPROPERTY(BlueprintReadOnly, Meta = (ExposeOnSpawn = true))
		int index;

	UPROPERTY(BlueprintReadWrite, Category = "Elements")
	class USlider* workEndSlider = nullptr;
	UPROPERTY(BlueprintReadWrite, Category = "Elements")
	class USlider* workStartSlider = nullptr;
	UPROPERTY(BlueprintReadWrite, Category = "Elements")
	class USlider* workHourCountSlider = nullptr;
	UPROPERTY(BlueprintReadWrite, Category = "Elements")
	float workEndSliderValue;

	float hourStep = 1.0f / 24.0f;
};
