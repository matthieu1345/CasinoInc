// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CI_PathFollowingComponent_CPP.generated.h"

//TODO:DOCUMENT comment/document this file

/*
 * 0 == don't profile anything
 * 1 == only profile    Should probably never use this one!
 * 2 == profile and print to log
 * 3 == profile and print to screen
*/
#define PROFILE_PATHFINDING_LEVEL 0

/*
 *	0 == don't debug paths
 *	1 == draw debug to next point
 *	2 == draw debug whole path
 */
#define DEBUG_PATH 0



UENUM(BlueprintType)
enum class EPathFollowingState : uint8
{
	PFS_Idle			UMETA(DisplayName = "Idle"),
	PFS_Calculating		UMETA(DisplayName = "Calculating"),
	PFS_Moving			UMETA(DisplayName = "Moving"),
	PFS_ReachedNode		UMETA(DisplayName = "Node Reached"),
	PFS_ReachedGoal		UMETA(DisplayName = "Goal Reached"),
	PFS_InvalidPath		UMETA(DisplayName = "Invalid Path")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPathFinished, bool, successfull);

UCLASS( ClassGroup=(AI), meta=(BlueprintSpawnableComponent) )
class CASINO_INC_API UCI_PathFollowingComponent_CPP : public UActorComponent
{
GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCI_PathFollowingComponent_CPP();

	void BeginDestroy() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	double LastCalcTime() const;
	UFUNCTION(BlueprintCallable)
		void GetNewPath(FVector2D start, FVector2D end);
	UFUNCTION()
		void PathCalculationFinished(bool completePath);
	UFUNCTION(BlueprintCallable)
		FVector GetLastMovement() const { return LastTickMovement; }

	void EnableMovement() { doPathMovement = true; }
	void DisableMovement() { doPathMovement = false; }

	UFUNCTION(BlueprintCallable)
		bool HasFinishedPath() { return currentState == EPathFollowingState::PFS_ReachedGoal; }
	UFUNCTION(BlueprintCallable)
		bool IsActiveState()
	{
		return	currentState == EPathFollowingState::PFS_Calculating ||
				currentState == EPathFollowingState::PFS_Moving ||
				currentState == EPathFollowingState::PFS_ReachedNode;
	}

	UFUNCTION(BlueprintCallable)
		void CancelPath();

	FPathFinished pathFinishedDelegate;

	UFUNCTION(BlueprintCallable)
		void PrintPathState();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	void CheckGoalNodeReached();


#if DEBUG_PATH > 0
	void DrawPathDebug();
#endif

public:	
/*
*	0 == don't debug paths
*	1 == draw debug to next point
*	2 == draw debug whole path
*/
	UPROPERTY(EditAnywhere, Category = "Debug", meta = (DisplayName = "personal debug level"))
		int debugLevel = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug", meta = (DisplayName = "personal debug color"))
		FColor debugColor = FColor(255, 0, 0);

protected:
	class ACI_GameStateBase_CPP *gameState = nullptr;
	class ACI_TileMap_CPP* tileMap = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool doPathMovement = false;

	UPROPERTY()
	class UPath* path = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float maxSpeed = 300;

	EPathFollowingState currentState = EPathFollowingState::PFS_Idle;

	UPROPERTY(EditDefaultsOnly)
		int maxReach = 50000;

private:
	void DoMovement(float deltaTime);

	double lastCalcStart = 0;
	double lastCalcEnd = 0;

	FVector2D start;
	FVector2D end;
	struct FPathNode* nextGoalNode = nullptr;

	FVector LastTickMovement;

	void MoveActor(FVector input);

private:
	bool broadcastedFinished = false;
};