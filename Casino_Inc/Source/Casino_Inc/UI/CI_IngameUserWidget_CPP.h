// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Managers/CI_TimeManager_CPP.h"
#include "CI_IngameUserWidget_CPP.generated.h"

//TODO:DOCUMENT comment/document this file

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
		void ClosePopupMenu() const;
	UFUNCTION(BlueprintCallable)
		TSubclassOf<class UCI_BaseTileDataComponent_CPP> GetSelectBuildObject() const;

	UFUNCTION()
		void AICountChanged(int staff, int guests);

protected:
	UFUNCTION(BlueprintCallable)
		void OpenPopupMenu() const;

	UFUNCTION(BlueprintCallable)
		FText GetXCoordinateText() const;
	UFUNCTION(BlueprintCallable)
		FText GetYCoordinateText() const;
	UFUNCTION(BlueprintCallable)
		FText GetHoverName() const;
	UFUNCTION(BlueprintCallable)
		UTexture2D* GetHoverTilePreview() const;
	UFUNCTION(BlueprintCallable)
		FText GetSelectedBuildObjectName() const;
	UFUNCTION(BlueprintCallable)
		UTexture2D* GetSelectedBuildObjectPreview() const;

	UFUNCTION(BlueprintCallable)
		bool GetCornerInfoVisible() const;
	UFUNCTION(BlueprintCallable)
		FText GetCornerXText() const;
	UFUNCTION(BlueprintCallable)
		FText GetCornerYText() const;
	UFUNCTION(BlueprintCallable)
		FText GetCornerSelectText() const;

	UFUNCTION(BlueprintCallable)
		FText GetCurrentTimeText() const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void ChangeSpeed(ESpeedSetting newSpeed) const;

	UFUNCTION()
		void MoneyChanged(int newBalance, int newDaily, int change);

	UFUNCTION(BlueprintCallable)
		FText GetHappinessText() const;

	UFUNCTION(BlueprintCallable)
		void CasinoOpenToggled(bool isChecked) const;


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