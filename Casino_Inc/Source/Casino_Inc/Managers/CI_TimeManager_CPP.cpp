// Fill out your copyright notice in the Description page of Project Settings.

#include "CI_TimeManager_CPP.h"
#include "Casino_Inc.h"
#include "EngineUtils.h"
#include "MainGameMode/CI_Pawn_CPP.h"
#include "MainGameMode/CI_GameStateBase_CPP.h"
#include "Camera/CameraComponent.h"
#include "Components/BillboardComponent.h"
#include "DebugMacros.h"

// Sets default values
ACI_TimeManager_CPP::ACI_TimeManager_CPP()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	editorObject = CreateDefaultSubobject<UBillboardComponent>(TEXT("billboard"));
	editorObject->SetupAttachment(RootComponent);
	editorObject->SetVisibility(true);
	editorObject->SetHiddenInGame(true);
}

ACI_TimeManager_CPP::~ACI_TimeManager_CPP()
{
	if (instance.IsValid() && instance.Get() == this)
	{
		instance.Reset();
	}
}

TWeakObjectPtr<ACI_TimeManager_CPP> ACI_TimeManager_CPP::GetInstance(UWorld* world)
{
	if (instance == nullptr)
	{
		for (TActorIterator<ACI_TimeManager_CPP> actorItr(world); actorItr;)
		{
			instance = *actorItr;
			break;
		}
	}

	return instance;
}

// Called when the game starts or when spawned
void ACI_TimeManager_CPP::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetInstance(GetWorld()).IsValid() && GetInstance(GetWorld()) != this)
	{
		UE_LOG(TimeLog, Warning, TEXT("Trying to make a 2nd TimeManager! destroying the new one!"));
		this->MarkPendingKill();
		return;
	}

	instance = this;
	_instance = this;

	gamePawn = Cast<ACI_GameStateBase_CPP>(GetWorld()->GetGameState())->currentPawn;

	currentTime = dayLenght * 0.3f;
}



void ACI_TimeManager_CPP::PostActorCreated()
{
	Super::PostActorCreated();

	if (!GetInstance(GetWorld()).IsValid())
	{
		instance = this;
		SetActorLocation(FVector(-300, -200, 0));

#if WITH_EDITOR
		SetFolderPath("Managers");
#endif

	}
	else if (GetInstance(GetWorld()) != this)
	{
		UE_LOG(TimeLog, Warning, TEXT("Trying to make a 2nd TimeManager! destroying the new one!"))
#if WITH_EDITOR
			this->SetActorLabel(TEXT("THIS OBJECT WILL BE DELETED FOR EXISTING 2 TIMES!"));
#endif

		SetActorLocation(FVector(-300, -200, -100));

#if WITH_EDITOR
		SetFolderPath("DELETE");
#endif

	}

	_instance = GetInstance(GetWorld());
}

bool ACI_TimeManager_CPP::SetGameSpeed(ESpeedSetting newSpeed)
{
	if (currentSpeed == newSpeed)
		return false;

	switch (newSpeed)
	{
	case ESpeedSetting::SS_Slowmo: 
		GetWorldSettings()->SetTimeDilation(0.5f);
		Cast<ACI_GameStateBase_CPP>(GetWorld()->GetGameState())->currentPawn->CustomTimeDilation = 2;
		break;

	case ESpeedSetting::SS_Quick: 
		GetWorldSettings()->SetTimeDilation(2.0f); 
		Cast<ACI_GameStateBase_CPP>(GetWorld()->GetGameState())->currentPawn->CustomTimeDilation = 0.5f;
		break;

	case ESpeedSetting::SS_Max: 
		GetWorldSettings()->SetTimeDilation(dayLenght / 300); 
		Cast<ACI_GameStateBase_CPP>(GetWorld()->GetGameState())->currentPawn->CustomTimeDilation = 1 / (dayLenght / 300);
		break;

	case ESpeedSetting::SS_Normal:
	default:
		GetWorldSettings()->SetTimeDilation(1.0f);
		Cast<ACI_GameStateBase_CPP>(GetWorld()->GetGameState())->currentPawn->CustomTimeDilation = 1;
		break;
	}

	currentSpeed = newSpeed;
	return true;
}

FVector ACI_TimeManager_CPP::GetCurrentTime()
{
	FVector output;

	output.X = currentDay;
	output.Y = currentHour;
	double temp;
	output.Z = modf(currentHour, &temp) * 60;

	return output;
}

// Called every frame
void ACI_TimeManager_CPP::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	currentTime += DeltaTime;

	if (currentTime >= dayLenght)
	{
		currentTime -= dayLenght;
		currentDay++;
		if (nextDayDelegate.IsBound())
		{
			nextDayDelegate.Broadcast(currentDay);
		}
		SET_WARN_COLOR(COLOR_PURPLE);
		UE_LOG(TimeLog, Log, TEXT("The day has changed! Its now day %i"), currentDay);
		CLEAR_WARN_COLOR()
	}

	int oldHour = FMath::FloorToInt(currentHour);
	currentHour = currentTime * 24 / dayLenght;
	int newHour = FMath::FloorToInt(currentHour);
	
	if (newHour != oldHour)
	{
		if (nextHourDelegate.IsBound())
		{
			nextHourDelegate.Broadcast(newHour);
		}
		SET_WARN_COLOR(COLOR_PURPLE);
		UE_LOG(TimeLog, Log, TEXT("The hour has changed! Its now %i o'clock"), newHour);
		CLEAR_WARN_COLOR()
	}

	if (!IsValid(gamePawn))
	{
		gamePawn = Cast<ACI_GameStateBase_CPP>(GetWorld()->GetGameState())->currentPawn;
	}

	float curveTime = currentTime / dayLenght;
	float newGain = cameraGainCurve->GetFloatValue(curveTime);
	FVector4 newColorGain = FVector4(newGain, newGain, newGain,1);
	gamePawn->GetCamera()->PostProcessSettings.ColorGain = newColorGain;
}

TWeakObjectPtr<ACI_TimeManager_CPP> ACI_TimeManager_CPP::instance;