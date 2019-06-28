// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Curves/CurveFloat.h"
#include "CI_TimeManager_CPP.generated.h"

UENUM(BlueprintType)
enum class ESpeedSetting : uint8
{
	SS_Slowmo,
	SS_Normal,
	SS_Quick,
	SS_Max
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNextDay, int, dayNumber);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNextHour, int, hourNumber);

UCLASS()
class CASINO_INC_API ACI_TimeManager_CPP : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACI_TimeManager_CPP();
	~ACI_TimeManager_CPP();

	static TWeakObjectPtr<ACI_TimeManager_CPP> GetInstance(UWorld *world);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostActorCreated() override;

	UFUNCTION(BlueprintCallable)
	bool SetGameSpeed(ESpeedSetting newSpeed);

	//returns a vector where:
	// X = day
	// Y = hour
	// Z = minute
	UFUNCTION(BlueprintCallable)
		FVector GetCurrentTime();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable)
		FNextDay nextDayDelegate;
	UPROPERTY(BlueprintAssignable)
		FNextHour nextHourDelegate;

protected:
	UPROPERTY(EditAnywhere, Category = "Editor")
		class UBillboardComponent *editorObject;

	UPROPERTY(VisibleAnywhere)
		float currentTime = 100;
	UPROPERTY(VisibleAnywhere)
		float currentHour = 0;
	UPROPERTY(VisibleAnywhere)
		int currentDay = 0;
	UPROPERTY(EditDefaultsOnly)
		float dayLenght = 1200;

	UPROPERTY(EditAnywhere, Category = Curve)
		UCurveFloat* cameraGainCurve;

	UPROPERTY(VisibleAnywhere)
		TWeakObjectPtr<ACI_TimeManager_CPP> _instance;

private:
	static TWeakObjectPtr<ACI_TimeManager_CPP> instance;
	class ACI_Pawn_CPP* gamePawn;

	ESpeedSetting currentSpeed = ESpeedSetting::SS_Normal;

};
