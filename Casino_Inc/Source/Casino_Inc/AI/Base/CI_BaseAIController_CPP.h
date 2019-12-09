// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Controller.h"
#include "AI/CI_AITypes.h"
#include "CI_BaseAIController_CPP.generated.h"

/**
 * Base AI controllers for <b>Casino Inc.</b><br>
 * has some basic implementation for leaving/entering the casino, and implementable functions for behaviour<br>
 * includes pathfinder and goapWorker components
 */
UCLASS()
class CASINO_INC_API ACI_BaseAIController_CPP : public AController
{
public:

	GENERATED_BODY()

		ACI_BaseAIController_CPP();
	~ACI_BaseAIController_CPP();

	/**
	 *	\brief Function called every frame on this Actor. Override this function to implement custom logic to be executed every frame.
	 *	\param deltaSeconds	Game time elapsed during last frame modified by the time dilation
	 */
	virtual void Tick(float deltaSeconds) override;

	/**
	 * \brief Handles attaching this controller to the specified pawn.
	 * \param inPawn The Pawn to be possessed.
	 */
	virtual void Possess(APawn* inPawn) override;

	/** 
	 * \brief Called to unpossess our pawn for any reason that is not the pawn being destroyed (destruction handled by PawnDestroyed()). 
	 */
	virtual void UnPossess() override;

	/**
	 * \return The Pathfollowing Component
	 */
	UFUNCTION(BlueprintGetter)
		class UCI_PathFollowingComponent_CPP* GetPathFollowingComponent() const { return pathFollowingComp; }
	/**
	 * \return The possessed BasePawn
	 */
	UFUNCTION(BlueprintGetter)
		class ACI_BaseAIPawn_CPP* GetBasePawn() const { return possessedCustomBasePawn; }

	/**
	 * \brief Register the ai to the AIManager
	 */
	UFUNCTION()
		void RegisterAI();

	/**
	 * \return The type of the character
	 */
	UFUNCTION(BlueprintGetter)
		ECharacterType GetType() const { return type; }

	/**
	 * \brief Ask for a new task from the AIManager
	 */
	virtual void GetNewTask();

	/**
	 * \brief return here if the task has ended
	 * \param success input if the task was a success or a fail
	 */
	UFUNCTION()
		void TaskEndCall(const bool success) { TaskEnded(success); }

	/**
	 * \brief return here if the task has ended
	 * \param success input if the task was a success or a fail
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void TaskEnded(bool success);

	/**
	 * \brief c++ implementation for <b>TaskEnded</b>:<br>
	 * return here if the task has ended
	 * Defaults to asking for a new task from the ai manager, or go to the idle behavior if none available
	 * \param success input if the task was a success or a fail
	 */
	virtual void TaskEnded_Implementation(bool success);

protected:
	/**
	 *\brief native event for when play begins for this actor.
	 */
	virtual void BeginPlay() override;

	/**
	 * \brief Implement to give the character a idle behaviour
	 */
	virtual void DoIdle(){};

	/**
	 * \brief A function that get's called every time the ingame hour changes<br>
	 * implement this to give the ai custom behaviour that needs to be done hourly
	 * \param hourNumber the current hour
	 */
	UFUNCTION()
	virtual void HourChanged(int hourNumber){}


	/**
	 * \brief called when the ai needs to enter the map<br>
	 * makes the character visible and resets the idle and away booleans
	 */
	virtual void EnterMap();

	/**
	 * \brief called when the ai needs to Leave the map<br>
	 * makes the character move towards a leaving spot and set's the idle and away booleans
	 */
	virtual void LeaveMap();

protected:
	/**
	 * \brief this ai's path-following component
	 */
	UPROPERTY(BlueprintReadOnly, Instanced, EditAnywhere)
		class UCI_PathFollowingComponent_CPP* pathFollowingComp;
	/**
	 * \brief this ai's goap worker component
	 */
	UPROPERTY(BlueprintReadOnly, Instanced, EditAnywhere)
		class UCI_GOAPWorkerComponent_CPP* goapWorkerComponent;

	/**
	 * \brief this ai's possesed pawn as <b>ACI_BaseAIPawn_CPP</b> pointer
	 */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class ACI_BaseAIPawn_CPP* possessedCustomBasePawn = nullptr;

	/**
	 * \brief this ai's character type
	 */
	ECharacterType type = ECharacterType::CT_Invalid;

	/**
	 * \brief this ai's current task
	 */
	struct FTask* currentTask;
	/**
	 * \brief if the ai is currently doing a task
	 */
	bool doingTask = false;

	/**
	 * \brief if the ai is away from/leaving the casino
	 */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "state")
		bool isAway = true;

	/**
	 * \brief the name of the ai
	 */
	FString name = "AI";
};
