// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Managers/CI_TimeManager_CPP.h"
#include "CI_IngameUserWidget_CPP.generated.h"

#define LOCTEXT_NAMESPACE "MainIngameGUI"

/**
 * 
 */
UCLASS()
class CASINO_INC_API UCI_IngameUserWidget_CPP : public UUserWidget
{
	GENERATED_BODY()

		virtual void NativeConstruct() override;
public:
	UFUNCTION(BlueprintCallable)
		void ClosePopupMenu();
	UFUNCTION(BlueprintCallable)
		TSubclassOf<class UCI_BaseTileDataComponent_CPP> GetSelectBuildObject();

	UFUNCTION()
		void AICountChanged(int staff, int guests);

protected:
	UFUNCTION(BlueprintCallable)
		void OpenPopupMenu();

	UFUNCTION(BlueprintCallable)
		FText GetXCoordinateText();
	UFUNCTION(BlueprintCallable)
		FText GetYCoordinateText();
	UFUNCTION(BlueprintCallable)
		FText GetHoverName();
	UFUNCTION(BlueprintCallable)
		UTexture2D* GetHoverTilePreview();
	UFUNCTION(BlueprintCallable)
		FText GetSelectedBuildObjectName();
	UFUNCTION(BlueprintCallable)
		UTexture2D* GetSelectedBuildObjectPreview();

	UFUNCTION(BlueprintCallable)
		bool GetCornerInfoVisible();
	UFUNCTION(BlueprintCallable)
		FText GetCornerXText();
	UFUNCTION(BlueprintCallable)
		FText GetCornerYText();
	UFUNCTION(BlueprintCallable)
		FText GetCornerSelectText();

	UFUNCTION(BlueprintCallable)
		FText GetCurrentTimeText();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void ChangeSpeed(ESpeedSetting newSpeed);
	void ChangeSpeed_Implementation(ESpeedSetting newSpeed);

	UFUNCTION()
		void MoneyChanged(int newBalance, int newDaily, int change);

	UFUNCTION(BlueprintCallable)
		FText GetHappinessText();

	UFUNCTION(BlueprintCallable)
		void CasinoOpenToggled(bool isChecked);


public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		TSubclassOf<UUserWidget> popupToCreate;
	class ACI_GameStateBase_CPP* gameState;

protected:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		class UMenuAnchor* popupAnchor;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		FText currentMoneyText;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		FText dailyMoneyText;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		FText guestCountText;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		FText staffCountText;


};


#undef LOCTEXT_NAMESPACE