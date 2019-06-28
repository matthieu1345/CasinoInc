// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileMap/CI_BaseTileDataComponent_CPP.h"
#include "AI/Guests/CI_GuestStats.h"
#include "Data/CI_WeigthedInteractionChance.h"
#include "CI_InteractableTileData_CPP.generated.h"

/**
 * 
 */
UCLASS()
class CASINO_INC_API UCI_InteractableTileData_CPP : public UCI_BaseTileDataComponent_CPP
{
	GENERATED_BODY()


public:

	UCI_InteractableTileData_CPP();

	void DestroyComponent_Implementation(ACI_TileMap_CPP* tileMap, int x, int y) override;
	
	virtual void SetInfo_Implementation(int32 x, int32 y, ACI_TileMap_CPP* tileMap) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		float GetUtilityScore(const FGuestStats stats, const FGuestPersonalityPreference preference);
	virtual float GetUtilityScore_Implementation(const FGuestStats stats, const FGuestPersonalityPreference preference);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		static float GetUtilityScoreDefault(TSubclassOf<UCI_InteractableTileData_CPP> defaultClass, const FGuestStats stats, const FGuestPersonalityPreference preference);

	UFUNCTION(BlueprintCallable)
		void Interact(UPARAM(Ref) FGuestStats &stats, class ACI_BaseAIPawn_CPP* askingPawn, FGuestPersonalityPreference preference);

	UFUNCTION(BlueprintImplementableEvent)
	// ReSharper disable once CppFunctionIsNotImplemented  this is a blueprint only function
		FWeightedInteractionResult GetOverridenResult(FGuestStats stats, FGuestPersonalityPreference preference);

	float GetInteractTime() { return interactTime; }

	bool IsFree();
	FVector2D Reserve(class ACI_BaseAIPawn_CPP* askingPawn);

	void DealerArrived() { hasDealer = true; }
	UFUNCTION()
	void DealerLeft();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void UpdateInteractLocations();


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FInteractionResult results;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		EInteractables type;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float defaultUtilityScore = 5;

	UPROPERTY(EditDefaultsOnly)
		class UCI_BaseTask_CPP* dealerTask;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		bool hasOverridenResult = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TMap<FVector2D, class ACI_BaseAIPawn_CPP*> interactionOffsets;

	UPROPERTY(EditDefaultsOnly, BlueprintreadOnly)
		FVector2D dealerOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float interactTime;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		bool inUse = false;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		bool hasDealer = false;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		bool needsDealer = false;

	UPROPERTY(EditDefaultsOnly)
		bool interactsInternal = true;

private:
	TArray<FVector2D> defaultInteractOffsets;
	FTimerHandle updateLocationsHandle;
};

