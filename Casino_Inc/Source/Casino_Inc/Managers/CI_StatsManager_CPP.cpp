// Fill out your copyright notice in the Description page of Project Settings.

#include "CI_StatsManager_CPP.h"
#include "Components/BillboardComponent.h"
#include "Casino_Inc.h"
#include "EngineUtils.h"
#include "CI_TimeManager_CPP.h"

//TODO:DOCUMENT comment/document this file

// Sets default values
ACI_StatsManager_CPP::ACI_StatsManager_CPP()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	editorObject = CreateDefaultSubobject<UBillboardComponent>(TEXT("billboard"));
	editorObject->SetupAttachment(RootComponent);
	editorObject->SetVisibility(true);
	editorObject->SetHiddenInGame(true);
}

ACI_StatsManager_CPP::~ACI_StatsManager_CPP()
{
	if (instance.IsValid() && instance.Get() == this)
	{
		instance.Reset();
	}
}

void ACI_StatsManager_CPP::PostActorCreated()
{
	Super::PostActorCreated();

	if (!GetInstance(GetWorld()).IsValid())
	{
		instance = this;
		SetActorLocation(FVector(-200, -300, 0));

#if WITH_EDITOR
		SetFolderPath("Managers");
#endif

	}
	else if (GetInstance(GetWorld()) != this)
	{
		UE_LOG(CasinoStats, Warning, TEXT("Trying to make a 2nd StatManager! destroying the new one!"))
#if WITH_EDITOR
			this->SetActorLabel(TEXT("THIS OBJECT WILL BE DELETED FOR EXISTING 2 TIMES!"));
#endif

		SetActorLocation(FVector(-200, -300, -100));

#if WITH_EDITOR
		SetFolderPath("DELETE");
#endif

	}

	_instance = GetInstance(GetWorld());
}

TWeakObjectPtr<ACI_StatsManager_CPP> ACI_StatsManager_CPP::GetInstance(UWorld* world)
{
	if (instance == nullptr)
	{
		for (TActorIterator<ACI_StatsManager_CPP> actorItr(world); actorItr;)
		{
			instance = *actorItr;
			break;
		}
	}

	return instance;
}

// Called when the game starts or when spawned
void ACI_StatsManager_CPP::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetInstance(GetWorld()).IsValid() && GetInstance(GetWorld()) != this)
	{
		UE_LOG(CasinoStats, Warning, TEXT("Trying to make a 2nd TimeManager! destroying the new one!"));
		this->MarkPendingKill();
		return;
	}

	instance = this;
	_instance = this;

	ACI_TimeManager_CPP::GetInstance(GetWorld())->nextDayDelegate.AddDynamic(this, &ACI_StatsManager_CPP::ResetDaily);
}

void ACI_StatsManager_CPP::ResetDaily(int newDay)
{
	dailyMoney = 0;
	moneyChangedDelegate.Broadcast(money, dailyMoney, 0);
}

void ACI_StatsManager_CPP::AddMoney(int change)
{
	bool underflow = false;
	bool overflow = false;

	if ((money > 0) && (change > INT_MAX - money)) 
		overflow = true;

	if ((money < 0) && (change < INT_MIN - money))
		underflow = true;

	if (!underflow && !overflow)
		money += change;


	if ((dailyMoney > 0) && (change > INT_MAX - dailyMoney))
		overflow = true;

	if ((dailyMoney < 0) && (change < INT_MIN - dailyMoney))
		underflow = true;

	if (!underflow && !overflow)
		dailyMoney += change;

	if (moneyChangedDelegate.IsBound())
	{
		moneyChangedDelegate.Broadcast(money, dailyMoney, change);
	}
}

void ACI_StatsManager_CPP::ChangeHappiness(float change)
{
	averageHappiness += change * happinessChangeMultipler;

	if (averageHappiness >= 100.0f)
		averageHappiness = 100.0f;
	else if (averageHappiness <= 0.0f)
		averageHappiness = 0.0f;
}

// Called every frame
void ACI_StatsManager_CPP::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


TWeakObjectPtr<ACI_StatsManager_CPP> ACI_StatsManager_CPP::instance;