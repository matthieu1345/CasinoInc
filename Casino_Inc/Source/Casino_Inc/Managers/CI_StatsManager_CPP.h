// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CI_StatsManager_CPP.generated.h"

//TODO:DOCUMENT comment/document this file

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FMoneyChanged, int, newBalance, int, newDaily, int, change);

UCLASS()
class CASINO_INC_API ACI_StatsManager_CPP : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACI_StatsManager_CPP();
	~ACI_StatsManager_CPP();
	// Called every frame
	virtual void Tick(float deltaTime) override;
	virtual void PostActorCreated() override;

	static TWeakObjectPtr<ACI_StatsManager_CPP> GetInstance(UWorld *world);
	static TWeakObjectPtr<ACI_StatsManager_CPP> GetInstance() { return instance; }

	UFUNCTION()
		void AddMoney(int change);

	UFUNCTION()
		void ChangeHappiness(float change);

	float GetAverageHappiness() const { return averageHappiness; }

	bool HasMoneyToBuild() const { return money > minimumMoney; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void ResetDaily(int newDay);
	UFUNCTION()
		int GetMoney() const { return money; }
	UFUNCTION()
		int GetDailyMoney() const { return dailyMoney; }

public:
	UPROPERTY(BlueprintAssignable)
		FMoneyChanged moneyChangedDelegate;
	

protected:
	UPROPERTY(EditAnywhere, Category = "Editor")
		class UBillboardComponent *editorObject;
	UPROPERTY(VisibleAnywhere)
		TWeakObjectPtr<ACI_StatsManager_CPP> local_instance;

	UPROPERTY(EditAnywhere)
		int money;
	UPROPERTY(EditAnywhere)
		int dailyMoney;

	UPROPERTY(EditAnywhere)
		float averageHappiness = 50;
	
	UPROPERTY(EditAnywhere)
		float happinessChangeMultipler = 0.1;

	UPROPERTY(EditAnywhere)
		int minimumMoney = -1000;

private:	
	static TWeakObjectPtr<ACI_StatsManager_CPP> instance;

};
