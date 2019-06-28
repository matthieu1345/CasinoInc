// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AI/GOAP/CI_GOAPStateContainer_CPP.h"
#include "CI_BaseAIPawn_CPP.generated.h"

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

	UFUNCTION(BlueprintGetter)
		class ACI_BaseAIController_CPP* GetBaseController() { return possessingCustomBaseController; }
	UFUNCTION(BlueprintPure)
		class UCI_PathFollowingComponent_CPP* GetPathFollowingComponent();

	FVector2D GetTileCoordinates();

	UFUNCTION(BlueprintCallable)
	FText GetAIName()
	{
		return FText::FromString(aiName);
	}

	UFUNCTION(BlueprintNativeEvent)
		void StateChanged();
	void StateChanged_Implementation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	void UpdateBody();

public:
	virtual void UnPossessed() override;
	virtual void PossessedBy(AController* NewController) override;

	/*
	*	0 == don't debug paths
	*	1 == draw debug to next point
	*	2 == draw debug whole path
	*/
	UPROPERTY(EditAnywhere, Category = "Debug", meta = (DisplayName = "personal debug level"))
		int debugLevel = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug", meta = (DisplayName = "personal debug color"))
		FColor debugColor = FColor(255, 0, 0);

	FString aiName = "AI";

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	class UPaperFlipbookComponent* body = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	class UPaperGroupedSpriteComponent* head;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TArray<class UPaperFlipbook*> bodyBooks;

	UPROPERTY(VisibleAnywhere, BlueprintGetter = GetBaseController)
	class ACI_BaseAIController_CPP* possessingCustomBaseController = nullptr;
};
