// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CI_GameStateBase_CPP.generated.h"

//TODO:DOCUMENT comment/document this file

UENUM(BlueprintType)
enum class EBuildInputMode : uint8
{
	BIM_None		UMETA(DisplayName = "None"),
	BIM_Build		UMETA(DisplayName = "Build"),
	BIM_BuildRoom	UMETA(DisplayName = "BuildRoom"),
	BIM_Destroy		UMETA(DisplayName = "Destroy"),
	BIM_Priority	UMETA(DisplayName = "Priority")
};

/**
 * 
 */
UCLASS()
class CASINO_INC_API ACI_GameStateBase_CPP : public AGameStateBase
{
	GENERATED_BODY()

	virtual void Tick(float deltaSeconds) override;
public:

	class ACI_Pawn_CPP *currentPawn;
	class ACI_HUD_CPP *currentHUD;
	class ACI_PlayerController_CPP *currentController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool movedThisTick = false;

	UPROPERTY(BlueprintReadOnly)
	class ACI_TileMap_CPP *tileMap;

	class UCI_IngameUserWidget_CPP *mainUI;

	UFUNCTION(BlueprintSetter)
	void SetSelectedBuildObject(const TSubclassOf<class UCI_BaseTileDataComponent_CPP> newObject)
	{
		selectedBuildObject = newObject;
		changedSelectedBuildObject = true;
	}
	UFUNCTION(BlueprintGetter)
		TSubclassOf<class UCI_BaseTileDataComponent_CPP> GetSelectedBuildObject() const
		{
		return selectedBuildObject;
	}

	UFUNCTION(BlueprintSetter)
	void SetSelectedTask(class UCI_BaseTask_CPP* newTask)
	{
		selectedTask = newTask;
		changedSelectedBuildObject = true;
	}
	UFUNCTION(BlueprintGetter)
		class UCI_BaseTask_CPP* GetSelectedTask() const
		{
		return selectedTask;
	}

	UFUNCTION(Exec)
		void ToggleCheatMode();

	bool GetCheatMode() const { return cheatmode; }
		

	bool changedSelectedBuildObject = false;


	FVector2D selectedBuildCorner;
	bool isSelecting2NdCorner = false;

	UFUNCTION(BlueprintCallable)
	void ChangeMode(EBuildInputMode newMode);
	UFUNCTION(BlueprintCallable)
	EBuildInputMode GetCurrentMode() const { return currentMode; }
	EBuildInputMode currentMode = EBuildInputMode::BIM_None;

private:
	TSubclassOf<class UCI_BaseTileDataComponent_CPP> selectedBuildObject;
	class UCI_BaseTask_CPP *selectedTask;

	bool cheatmode = false;
};