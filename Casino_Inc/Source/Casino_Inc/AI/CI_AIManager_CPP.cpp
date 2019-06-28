// Fill out your copyright notice in the Description page of Project Settings.

#include "CI_AIManager_CPP.h"
#include "Casino_Inc.h"
#include "Components/BillboardComponent.h"
#include "MainGameMode/CI_GameStateBase_CPP.h"
#include "DebugMacros.h"
#include "Tasks/BuildTask.h"
#include "TileMap/CI_BaseTileDataComponent_CPP.h"
#include "TileMap/CI_BaseTile_CPP.h"
#include "CI_RegisterAbleInterface_CPP.h"
#include "EngineUtils.h"
#include "TileMap/CI_TileMap_CPP.h"
#include "PathFinding/CI_Path.h"
#include "MainGameMode/CI_GameModeBase_CPP.h"
#include <limits>
#include "TileMap/CI_TileMapCoordinateMath.h"
#include "Base/CI_BaseAIController_CPP.h"
#include "UI/CI_IngameUserWidget_CPP.h"
#include "Managers/CI_StatsManager_CPP.h"
#include "TimerManager.h"
#include "Curves/CurveFloat.h"

// Sets default values
ACI_AIManager_CPP::ACI_AIManager_CPP()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	editorObject = CreateDefaultSubobject<UBillboardComponent>(TEXT("billboard"));
	editorObject->SetupAttachment(RootComponent);
	editorObject->SetVisibility(true);
	editorObject->SetHiddenInGame(true);
}

ACI_AIManager_CPP::~ACI_AIManager_CPP()
{
	if (instance.IsValid() && instance.Get() == this)
	{
		instance.Reset();
	}
}

TWeakObjectPtr<ACI_AIManager_CPP> ACI_AIManager_CPP::GetInstance(UWorld* world)
{
	if (instance == nullptr)
	{
		for (TActorIterator<ACI_AIManager_CPP> actorItr(world); actorItr;)
		{
			instance = *actorItr;
			break;
		}
	}
	
	return instance;
}

// Called when the game starts or when spawned
void ACI_AIManager_CPP::BeginPlay()
{
	Super::BeginPlay();
	
	if (instance.IsValid() && instance.Get() != this)
	{
		UE_LOG(AIManagement, Warning, TEXT("Trying to make a 2nd AIManager! destroying the new one!"))
		this->MarkPendingKill();
		return;
	}

	instance = this;
	_instance = this;

	gamestate = Cast<ACI_GameStateBase_CPP>(GetWorld()->GetGameState());
	//gamestate->mainUI->AICountChanged(staffActors.Num(), guestActors.Num());

	SetGuestSpawnTimer();
	DisableGuestSpawning();
}

// Called every frame
void ACI_AIManager_CPP::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	spawnCounter += DeltaTime;
	if (spawnCounter >= spawnTimer)
	{
		spawnCounter -= spawnTimer;
		if (!spawnQueue.IsEmpty())
		{
			TSubclassOf<APawn> ai;
			spawnQueue.Dequeue(ai);
			SpawnAIInternal(ai);
		}
	}

	if (taskQueue.IsValid())
	{
		int counter = 0;
		FTaskLinkedListNode* node = taskQueue.GetHead();
		while (node != nullptr)
		{
			counter++;
			node = node->next;
		}
	}

	if (priorityTaskQueue.IsValid())
	{
		int counter = 0;
		FTaskLinkedListNode* node = priorityTaskQueue.GetHead();
		while (node != nullptr)
		{
			counter++;
			node = node->next;
		}
	}
}

void ACI_AIManager_CPP::PostActorCreated()
{
	Super::PostActorCreated();

	if (!instance.IsValid())
	{
		instance = this;
		SetActorLocation(FVector(-200, -200, 0));
		
#if WITH_EDITOR
		SetFolderPath("Managers");
#endif

	}
	else if (instance.Get() != this)
	{
		UE_LOG(AIManagement, Warning, TEXT("Trying to make a 2nd AIManager! destroying the new one!"))
#if WITH_EDITOR
			this->SetActorLabel(TEXT("THIS OBJECT WILL BE DELETED FOR EXISTING 2 TIMES!"));
#endif

		SetActorLocation(FVector(-200, -200, -100));

#if WITH_EDITOR
		SetFolderPath("DELETE");
#endif

	}

	_instance = instance;
	
}

bool ACI_AIManager_CPP::GetNextTask(ECharacterType characterType, FTask*& outTask)
{

	for (FTaskLinkedListNode* task = priorityTaskQueue.GetHead(); task != nullptr; task = task->next)
	{
		if (task->GetValue()->taskType->CheckAllowed(characterType))
		{
			outTask = task->GetValue();

			priorityTaskQueue.RemoveNode(task);
			usedTasks.AddTail(task);
			return true;
		}
	}

	for (FTaskLinkedListNode* task = taskQueue.GetHead(); task != nullptr; task = task->next)
	{
		if (task->GetValue()->taskType->CheckAllowed(characterType))
		{
			outTask = task->GetValue();

			taskQueue.RemoveNode(task);
			usedTasks.AddTail(task);
			return true;
		}
	}

	return false;
}

bool ACI_AIManager_CPP::CreateNewTask(UCI_BaseTask_CPP* taskType, int x, int y, class ACI_BaseTile_CPP* tile)
{
	if (!ACI_StatsManager_CPP::GetInstance(GetWorld())->HasMoneyToBuild())
		return false;

	FTask* newTask = new FTask(taskType, x, y, tile);

	if (!newTask->isValid(gamestate))
	{
		return false;
	}

	if (taskQueue.Contains(newTask))
	{
		return false;

	}
	if (priorityTaskQueue.Contains(newTask))
	{
		return false;
	}
	if (usedTasks.Contains(newTask))
	{
		return false;
	}

	if (!newTask->hasPriority)
	{
		taskQueue.AddTail(newTask);
		newTask->Init(gamestate);
		ACI_StatsManager_CPP::GetInstance(GetWorld())->AddMoney(-newTask->taskType->GetMoneyCost());
		return true;
	}
	else
	{
		priorityTaskQueue.AddTail(newTask);
		newTask->Init(gamestate);
		ACI_StatsManager_CPP::GetInstance(GetWorld())->AddMoney(-newTask->taskType->GetMoneyCost());
		return true;
	}
}

bool ACI_AIManager_CPP::PrioritiseTask(FTask* task)
{
	auto taskNode = taskQueue.FindNode(task);

	if (taskNode == nullptr)
		return false;

	taskQueue.RemoveNode(taskNode);
	priorityTaskQueue.AddTail(taskNode);
	taskNode->task->hasPriority = true;

	gamestate->tileMap->AddPriorityPreview(task->x, task->y);

	return true;
}

bool ACI_AIManager_CPP::DePrioritiseTask(FTask* task)
{
	auto taskNode = priorityTaskQueue.FindNode(task);

	if (taskNode == nullptr)
		return false;

	priorityTaskQueue.RemoveNode(taskNode);
	taskQueue.AddTail(taskNode);
	taskNode->task->hasPriority = false;

	task->CreatePreview(gamestate);

	return true;
}

bool ACI_AIManager_CPP::SwitchPriority(FTask* task)
{
	if (task->hasPriority)
	{
		DePrioritiseTask(task);
		return false;
	}
	else
	{
		PrioritiseTask(task);
		return true;
	}
}

FTask* ACI_AIManager_CPP::GetTask(int x, int y)
{
	FTaskLinkedListNode* task;
	if (taskQueue.IsValid())
	{

		task = taskQueue.GetHead();

		while (task != nullptr)
		{
			if (task->GetValue()->x == x && task->GetValue()->y == y)
				return task->GetValue();

			task = task->next;
		}
	}

	if (priorityTaskQueue.IsValid())
	{
		task = priorityTaskQueue.GetHead();
		while (task != nullptr)
		{
			if (task->GetValue()->x == x && task->GetValue()->y == y)
				return task->GetValue();

			task = task->next;
		}
	}

	if (usedTasks.IsValid())
	{
		task = usedTasks.GetHead();
		while (task != nullptr)
		{
			if (task->GetValue()->x == x && task->GetValue()->y == y)
				return task->GetValue();

			task = task->next;
		}
	}

	return nullptr;
}

void ACI_AIManager_CPP::FinishTask(FTask* task)
{
	usedTasks.RemoveNode(usedTasks.FindNode(task));
}

void ACI_AIManager_CPP::CancelTask(FTask*& task)
{
	if (usedTasks.Contains(task))
	{
		usedTasks.RemoveNode(usedTasks.FindNode(task));
	}

	if (task->hasPriority)
		priorityTaskQueue.AddTail(task);
	else
		taskQueue.AddTail(task);
}

bool ACI_AIManager_CPP::RegisterObject(UObject* object)
{
	ICI_RegisterAbleInterface_CPP* TheInterface = Cast<ICI_RegisterAbleInterface_CPP>(object);

	if (TheInterface)
	{
		return RegisterObject(TheInterface->Execute_GetRegisterName(object), object);
	}
	else return false;
}

bool ACI_AIManager_CPP::RegisterObject(FString dataType, UObject* object)
{
	if (dataType == "")
	{
		UE_LOG(AIManagement, Warning, TEXT("Trying to add a null string datatype!") )
		return false;
	}

	if (!registeredTiles.Contains(dataType))
		registeredTiles.Add(dataType);

	registeredTiles.Find(dataType)->values.Add(object);
	return true;
}

bool ACI_AIManager_CPP::DeRegisterObject(FString dataType, UObject* object)
{
	FTileRegister *array = &registeredTiles.FindOrAdd(dataType);

	if (!array->values.Contains(object))
		return false;

	array->values.Remove(object);
	return true;
}

bool ACI_AIManager_CPP::DeRegisterObject(UObject* object)
{
	ICI_RegisterAbleInterface_CPP* TheInterface = Cast<ICI_RegisterAbleInterface_CPP>(object);

	if (TheInterface)
	{
		return DeRegisterObject(TheInterface->Execute_GetRegisterName(object), object);
	}
	else return false;
}

FVector2D ACI_AIManager_CPP::GetClosestItem(FVector2D startLocation, FString dataType)
{
	float distance = std::numeric_limits<float>::max();
	float tempDist;
	TArray<UObject*> endTile;

	auto array = registeredTiles.FindRef(dataType);

	for (auto element : array.values)
	{
		tempDist = FVector2D::DistSquared(ICI_RegisterAbleInterface_CPP::Execute_GetRegisterLocation(element), startLocation);
		if (FMath::IsNearlyEqual(tempDist, distance, 200))
		{
			endTile.Add(element);
		}

		if (tempDist < distance - 200)
		{
			endTile.Empty();
			endTile.Add(element);
			distance = tempDist;
		}
	}

	if (endTile.Num() > 0)
	{
		int random = FMath::RandRange(0, endTile.Num() - 1);

		return ICI_RegisterAbleInterface_CPP::Execute_GetRegisterLocation(endTile[random]);
	}
	else
	{
		return FVector2D::ZeroVector;
	}
}

void ACI_AIManager_CPP::SpawnAI(TSubclassOf<APawn> pawnToSpawn)
{
	spawnQueue.Enqueue(pawnToSpawn);
}

void ACI_AIManager_CPP::SpawnAIInternal(TSubclassOf<APawn> pawnToSpawn)
{
	switch (Cast<ACI_BaseAIController_CPP>(Cast<APawn>(pawnToSpawn->GetDefaultObject())->AIControllerClass->GetDefaultObject())->GetType())
	{
	case ECharacterType::CT_Builder: 
		if (countPerAIType.FindOrAdd(ECharacterType::CT_Builder) >= maxCountPerAIType.FindOrAdd(ECharacterType::CT_Builder))
			return;
		break;

	case ECharacterType::CT_Guest: 
		if (countPerAIType.FindOrAdd(ECharacterType::CT_Guest) >= maxCountPerAIType.FindOrAdd(ECharacterType::CT_Guest))
			return;
		break;

	case ECharacterType::CT_Dealer: 
		if (countPerAIType.FindOrAdd(ECharacterType::CT_Dealer) >= maxCountPerAIType.FindOrAdd(ECharacterType::CT_Dealer))
			return;
		break;

	case ECharacterType::CT_Invalid: break;
	default: ;
	}

	FVector spawnLocation = UCI_TileMapCoordinateMath::TileVectorToWorldCenter(GetRandomSpawnTile(), ACI_GameModeBase_CPP::CHARACTER_LAYER);

	APawn* spawnedPawn = (APawn*)GetWorld()->SpawnActor<APawn>(pawnToSpawn, spawnLocation, FRotator::ZeroRotator);

	ACI_BaseAIController_CPP* spawnedController = Cast<ACI_BaseAIController_CPP>(spawnedPawn->GetController());

	
}

void ACI_AIManager_CPP::RegisterAI(APawn* spawnedPawn, ACI_BaseAIController_CPP* spawnedController)
{
	if (IsValid(spawnedController))
	{
		switch (spawnedController->GetType())
		{

		case ECharacterType::CT_Builder:
			staffActors.Add(spawnedPawn);
			countPerAIType.FindOrAdd(ECharacterType::CT_Builder)++;
			break;
		case ECharacterType::CT_Guest:
			guestActors.Add(spawnedPawn);
			countPerAIType.FindOrAdd(ECharacterType::CT_Guest)++;
			break;
		case ECharacterType::CT_Dealer:
			staffActors.Add(spawnedPawn);
			countPerAIType.FindOrAdd(ECharacterType::CT_Dealer)++;
			break;

		case ECharacterType::CT_Invalid: break;
		default:;
		}
	}

	if (gamestate->mainUI != nullptr)
		gamestate->mainUI->AICountChanged(staffActors.Num(), guestActors.Num());
}

void ACI_AIManager_CPP::RemoveAI(APawn* pawn, ECharacterType type)
{
	switch (type)
	{

	case ECharacterType::CT_Builder:
		staffActors.Remove(pawn);
		countPerAIType[ECharacterType::CT_Builder]--;
		break;
	case ECharacterType::CT_Guest:
		guestActors.Remove(pawn);
		countPerAIType[ECharacterType::CT_Guest]--;
		break;
	case ECharacterType::CT_Dealer:
		staffActors.Remove(pawn);
		countPerAIType[ECharacterType::CT_Dealer]--;
		break;
			
	case ECharacterType::CT_Invalid:
	default:
		if (guestActors.Contains(pawn))
		{
			guestActors.Remove(pawn);
		}
		else if (staffActors.Contains(pawn))
		{
			staffActors.Remove(pawn);
		}
	}

	if (gamestate->mainUI != nullptr)
		gamestate->mainUI->AICountChanged(staffActors.Num(), guestActors.Num());
}

TArray<UObject*> ACI_AIManager_CPP::GetRegisteredTile(FString dataType)
{
	if (registeredTiles.Contains(dataType))
		return registeredTiles.Find(dataType)->values;

	return TArray<UObject*>();
}

UObject* ACI_AIManager_CPP::GetRandomRegisteredTile(FString dataType)
{
	TArray<UObject*> tiles = GetRegisteredTile(dataType);
	if (tiles.Num() == 0)
		return nullptr;

	int random = FMath::RandRange(0, tiles.Num() - 1);

	return tiles[random];
}

FVector2D ACI_AIManager_CPP::GetRandomSpawnTile()
{
	TArray<UCI_BaseTileDataComponent_CPP*> spawnLocations;
	FString typeName;
	for (TSubclassOf<UCI_BaseTileDataComponent_CPP> tileType : tilesToSpawnFrom)
	{
		typeName = ICI_RegisterAbleInterface_CPP::Execute_GetRegisterName(tileType->GetDefaultObject());
		if (registeredTiles.Contains(typeName))
		{
			for (auto tile : registeredTiles.Find(typeName)->values)
			{
				spawnLocations.Add(Cast<UCI_BaseTileDataComponent_CPP>(tile));
			}
		}
	}

	return spawnLocations[FMath::RandRange(0, spawnLocations.Num() - 1)]->GetRegisterLocation();
}

void ACI_AIManager_CPP::SpawnGuest()
{
	UE_LOG(AIManagement, Verbose, TEXT("Spawned Guest"));
	if (IsValid(guestPawn))
		SpawnAI(guestPawn);
	SetGuestSpawnTimer();
}

void ACI_AIManager_CPP::SetGuestSpawnTimer()
{
	float delay = 600.0f;
	if (IsValid(guestSpawnRate))
	{
		float happiness = ACI_StatsManager_CPP::GetInstance(GetWorld())->GetAverageHappiness();
		delay = guestSpawnRate->GetFloatValue(happiness);
		UE_LOG(AIManagement, Verbose, TEXT("New delay set to: %f"), delay);
		if (delay == 0)
			delay = 10;
		GetWorldTimerManager().SetTimer(guestSpawnTimerHandle, this, &ACI_AIManager_CPP::SpawnGuest, delay, false, -1);
	}
	else
	{
		GetWorldTimerManager().PauseTimer(guestSpawnTimerHandle);
		UE_LOG(AIManagement, Warning, TEXT("The AI Manager does not have a guest spawn rate set!"));
	}
}

void ACI_AIManager_CPP::ToggleGuestSpawning()
{
	if (IsGuestSpawningEnabled())
		GetWorldTimerManager().PauseTimer(guestSpawnTimerHandle);
	else
		GetWorldTimerManager().UnPauseTimer(guestSpawnTimerHandle);
}

void ACI_AIManager_CPP::EnableGuestSpawning()
{
	GetWorldTimerManager().UnPauseTimer(guestSpawnTimerHandle);
}

void ACI_AIManager_CPP::DisableGuestSpawning()
{
	GetWorldTimerManager().PauseTimer(guestSpawnTimerHandle);
}

bool ACI_AIManager_CPP::IsGuestSpawningEnabled()
{
	return GetWorldTimerManager().IsTimerActive(guestSpawnTimerHandle);
}


TWeakObjectPtr<ACI_AIManager_CPP> ACI_AIManager_CPP::instance;
