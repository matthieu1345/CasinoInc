// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AI/Tasks/CI_BaseTask_CPP.h"
#include "Queue.h"
#include "CI_AIManager_CPP.generated.h"

struct FTaskLinkedListNode
{
public:

	FTaskLinkedListNode(FTask* task) : task(task) { ; }
	~FTaskLinkedListNode() {}

	FTaskLinkedListNode* previous = nullptr;
	FTaskLinkedListNode* next = nullptr;

	FTask* task = nullptr;

	FTask* GetValue() { return task; }
};

struct FTaskLinkedList
{

	~FTaskLinkedList()
	{
		FTaskLinkedListNode* node = head;

		while (node != nullptr)
		{
			RemoveNode(node);

			delete node;

			node = head;
		}
	}

		FTaskLinkedListNode* head = nullptr;

		FTaskLinkedListNode* tail = nullptr;

	bool IsValid() { return head != nullptr && tail != nullptr; }

	bool Contains(FTask* task)
	{
		if (!IsValid())
			return false;

		FTaskLinkedListNode* check = head;

		if (check == nullptr)
			return false;

		while (check != nullptr)
		{
			if (*check->task == *task)
			{
				return true;
			}

			check = check->next;
		}

		return false;
	}

	bool Contains(FTaskLinkedListNode* node)
	{
		return Contains(node->task);
	}


	FTaskLinkedListNode* GetHead() { return head; }
	FTaskLinkedListNode* GetTail() { return tail; }

	void RemoveNode(FTaskLinkedListNode* task)
	{
		if (!IsValid())
			return;

		if (task->previous != nullptr)
		{
			task->previous->next = task->next;
		}
		else
		{
			head = task->next;
		}

		if (task->next != nullptr)
		{
			task->next->previous = task->previous;
		}
		else
		{
			tail = task->previous;
		}

		task->next = nullptr;
		task->previous = nullptr;
	}


	void AddTail(FTask* task)
	{
		FTaskLinkedListNode *newNode = new FTaskLinkedListNode(task);

		if (!IsValid())
		{
			tail = newNode;
			head = newNode;
			return;
		}

		newNode->previous = tail;
		tail->next = newNode;
		tail = newNode;
	}

	void AddHead(FTask* task)
	{
		FTaskLinkedListNode *newNode = new FTaskLinkedListNode(task);
		
		if (!IsValid())
		{
			tail = newNode;
			head = newNode;
			return;
		}

		newNode->next = head;
		head->previous = newNode;
		head = newNode;
	}

	void AddTail(FTaskLinkedListNode * node)
	{
		if (!IsValid())
		{
			tail = node;
			head = node;
			return;
		}

		node->previous = tail;
		tail->next = node;
		tail = node;
	}

	void AddHead(FTaskLinkedListNode * node)
	{
		if (!IsValid())
		{
			tail = node;
			head = node;
			return;
		}

		node->next = head;
		head->previous = node;
		head = node;
	}

	FTaskLinkedListNode* FindNode(FTask* task)
	{
		if (!IsValid())
			return nullptr;

		FTaskLinkedListNode* check = head;

		while (check != nullptr)
		{
			if (*check->task == *task)
			{
				return check;
			}

			check = check->next;
		}

		return nullptr;
	}
};

USTRUCT()
struct FTileRegister
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	TArray<UObject*> values;
};

UCLASS()
class CASINO_INC_API ACI_AIManager_CPP : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACI_AIManager_CPP();
	~ACI_AIManager_CPP();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = AIManager)
		static ACI_AIManager_CPP* GetAIManagerInstance() { return instance.Get(); }

	static TWeakObjectPtr<ACI_AIManager_CPP> GetInstance(UWorld *world);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostActorCreated() override;

	//UFUNCTION(BlueprintCallable)
	bool GetNextTask(ECharacterType characterType, FTask*& outTask);
	UFUNCTION(BlueprintCallable)
		bool CreateNewTask(UCI_BaseTask_CPP* taskType, int x, int y, class ACI_BaseTile_CPP* tile = nullptr);

	bool PrioritiseTask(FTask* task);
	bool DePrioritiseTask(FTask* task);

	bool SwitchPriority(FTask* task);
	FTask* GetTask(int x, int y);

	void FinishTask(FTask* task);
	void CancelTask(FTask*& task);

	bool RegisterObject(UObject* object);
	bool RegisterObject(FString dataType, UObject* tile);
	bool DeRegisterObject(FString dataType, UObject* tile);
	bool DeRegisterObject(UObject* object);

	FVector2D GetClosestItem(FVector2D startLocation, FString dataType);

	void SpawnAI(TSubclassOf<APawn> pawnToSpawn);
	void SpawnAIInternal(TSubclassOf<APawn> pawnToSpawn);
	void RegisterAI(APawn* spawnedPawn, class ACI_BaseAIController_CPP* spawnedController);
	void RemoveAI(APawn* pawn, ECharacterType type);

	TArray<UObject*> GetRegisteredTile(FString dataType);
	UObject* GetRandomRegisteredTile(FString dataType);

	FVector2D GetRandomSpawnTile();

	UFUNCTION(BlueprintCallable)
		TArray<class APawn*> GetStaffList() { return staffActors; }

	UFUNCTION()
		void SpawnGuest();

	void SetGuestSpawnTimer();

	UFUNCTION(BlueprintCallable)
		void ToggleGuestSpawning();

	void EnableGuestSpawning();
	void DisableGuestSpawning();

	UFUNCTION(BlueprintCallable)
		bool IsGuestSpawningEnabled();

protected:

	UPROPERTY(VisibleAnywhere)
	TWeakObjectPtr<ACI_AIManager_CPP> _instance;

	UPROPERTY(EditAnywhere, Category="Editor")
		class UBillboardComponent* editorObject;

	FTaskLinkedList taskQueue;
	FTaskLinkedList priorityTaskQueue;
	FTaskLinkedList usedTasks;

	class ACI_GameStateBase_CPP* gamestate;

	UPROPERTY(EditAnywhere)
	TMap<FString, FTileRegister> registeredTiles;

	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<class UCI_BaseTileDataComponent_CPP>> tilesToSpawnFrom;

	UPROPERTY(EditAnywhere)
		TMap<ECharacterType, int> maxCountPerAIType;

	FTimerHandle guestSpawnTimerHandle;

	UPROPERTY(EditAnywhere)
		class UCurveFloat* guestSpawnRate;
	UPROPERTY(EditAnywhere)
		TSubclassOf<APawn> guestPawn;

	TQueue<TSubclassOf<APawn>> spawnQueue;

private:

	static TWeakObjectPtr<ACI_AIManager_CPP> instance;

	int runnableCounter = 0;
	int frameCounter = 0;

	float spawnTimer = 0.5;
	float spawnCounter = 0;

	TArray<class APawn*> staffActors;
	TArray<class APawn*> guestActors;
	TMap<ECharacterType, int> countPerAIType;
};
