// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AI/GOAP/CI_GOAPStateContainer_CPP.h"
#include "CI_BaseAIPawn_CPP.generated.h"

/**
 * base AI pawn for <b>Casino Inc.</b><br>
 * has some basic functionality for changing different body's and heads
 */
UCLASS()
class CASINO_INC_API ACI_BaseAIPawn_CPP : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACI_BaseAIPawn_CPP();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/**
	 * \return The base controller as ACI_BaseAIController_CPP
	 */
	UFUNCTION(BlueprintGetter)
		class ACI_BaseAIController_CPP* GetBaseController() { return possessingCustomBaseController; }
	/**
	 * \return the pathfollowing component on the controller
	 */
	UFUNCTION(BlueprintPure)
		class UCI_PathFollowingComponent_CPP* GetPathFollowingComponent();

	/**
	 * \return the tile coordinates that this pawn is currently located at
	 */
	FVector2D GetTileCoordinates();

	/**
	 * \return the name of the ai
	 */
	UFUNCTION(BlueprintCallable)
	FText GetAIName()
	{
		return FText::FromString(aiName);
	}

	/**
	 * called when the ai state is changed
	 */
	UFUNCTION(BlueprintNativeEvent)
		void StateChanged();
	void StateChanged_Implementation();

protected:
	/**
	 * \brief Called when the game starts or when spawned
	 */
	virtual void BeginPlay() override;

private:
	/**
	 * \brief updates which body flipbook to use
	 */
	void UpdateBody();

public:
	/**
	 * \brief called when the pawn is unpossessed by it's controller
	 */
	virtual void UnPossessed() override;
	/**
	 * \brief called when the pawn is possessed by a controller
	 * \param NewController the controller that possesses us now
	 */
	virtual void PossessedBy(AController* NewController) override;

	/**
	 * \brief set's the level for how much needs to be debugged<br>
	 *	0 == don't debug paths<br>
	 *	1 == draw debug to next point<br>
	 *	2 == draw debug whole path<br>
	 */
	UPROPERTY(EditAnywhere, Category = "Debug", meta = (DisplayName = "personal debug level"))
		int debugLevel = 2;
	/**
	 * \brief the collor to debug with
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug", meta = (DisplayName = "personal debug color"))
		FColor debugColor = FColor(255, 0, 0);

	// the name of the ai
	FString aiName = "AI";

protected:
	// The body of the pawn
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	class UPaperFlipbookComponent* body = nullptr;

	// The head of the pawn
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	class UPaperGroupedSpriteComponent* head;

	// The different body's of the pawn
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TArray<class UPaperFlipbook*> bodyBooks;

	// The controller that possesses the pawn as <b>ACI_BaseAIController_CPP</b>
	UPROPERTY(VisibleAnywhere, BlueprintGetter = GetBaseController)
	class ACI_BaseAIController_CPP* possessingCustomBaseController = nullptr;
};
