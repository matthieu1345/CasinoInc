// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CI_GameModeBase_CPP.generated.h"

//TODO:DOCUMENT comment/document this file

/**
 * 
 */
UCLASS()
class CASINO_INC_API ACI_GameModeBase_CPP : public AGameModeBase
{
	GENERATED_BODY()

	ACI_GameModeBase_CPP();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "GUI")
		void ChangeMenuWidget(TSubclassOf<UUserWidget> newWidgetClass);

	void Logout(AController* Exiting) override;

	//TODO:XML write xml preview for SpeedUp, SlowDown, ResetSpeed, and AddMoney
	UFUNCTION(Exec)
		void SpeedUp();
	UFUNCTION(Exec)
		void SlowDown();
	UFUNCTION(Exec)
		void ResetSpeed();
	UFUNCTION(Exec)
		void AddMoney();

	static const bool LOG_PATHFINDING_TIMES = false;

public:
	static const float CHARACTER_LAYER;

	int pathFindingQuick = 0;
	int pathFindingMedium = 0;
	int pathFindingLong = 0;

	class ACI_GameStateBase_CPP *currentGameState = nullptr;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GUI")
		TSubclassOf<UUserWidget> defaultWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "GUI")
		UUserWidget* currentWidget = nullptr;
	
};