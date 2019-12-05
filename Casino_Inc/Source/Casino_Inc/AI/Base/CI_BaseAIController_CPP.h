// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Controller.h"
#include "AI/CI_AITypes.h"
#include "CI_BaseAIController_CPP.generated.h"

//TODO:DOCUMENT comment/document this file

/**
 * 
 */
UCLASS()
class CASINO_INC_API ACI_BaseAIController_CPP : public AController
{
public:

	GENERATED_BODY()

		ACI_BaseAIController_CPP();
	~ACI_BaseAIController_CPP();

	virtual void Tick(float DeltaSeconds) override;
	virtual void Possess(APawn* InPawn) override;
	virtual void UnPossess() override;


	UFUNCTION(BlueprintGetter)
		class UCI_PathFollowingComponent_CPP* GetPathFollowingComponent() const { return pathFollowingComp; }
	UFUNCTION(BlueprintGetter)
		class ACI_BaseAIPawn_CPP* GetBasePawn() const { return possessedCustomBasePawn; }

	UFUNCTION()
		void RegisterAI();

	UFUNCTION(BlueprintGetter)
		ECharacterType GetType() const { return type; }

	virtual void GetNewTask();

	UFUNCTION()
		void TaskEndCall(bool success) { TaskEnded(success); }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void TaskEnded(bool success);
	virtual void TaskEnded_Implementation(bool success);

protected:
	virtual void BeginPlay() override;
	virtual void DoIdle(){};

	UFUNCTION()
	virtual void HourChanged(int hourNumber){}

	virtual void EnterMap();
	virtual void LeaveMap();

protected:
	UPROPERTY(BlueprintReadOnly, Instanced, EditAnywhere)
		class UCI_PathFollowingComponent_CPP* pathFollowingComp;
	UPROPERTY(BlueprintReadOnly, Instanced, EditAnywhere)
		class UCI_GOAPWorkerComponent_CPP* goapWorkerComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class ACI_BaseAIPawn_CPP* possessedCustomBasePawn = nullptr;

	ECharacterType type = ECharacterType::CT_Invalid;

	struct FTask* currentTask;
	bool doingTask = false;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "state")
		bool isAway = true;

	FString name = "AI";
};
