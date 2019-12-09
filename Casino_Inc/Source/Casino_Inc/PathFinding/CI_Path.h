// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface.h"
#include "Runnable.h"
#include "Queue.h"
#include "CI_Path.generated.h"

//TODO:DOCUMENT comment/document this file

DECLARE_STATS_GROUP(TEXT("PathFinding"), STATGROUP_PathFinding, STATCAT_Advanced);

//USTRUCT(BlueprintType)
struct FPathNode
{
	//GENERATED_BODY()


		FPathNode() = default;
		FPathNode(FVector2D location, FVector2D goal, float g, FPathNode* parent);

	explicit FPathNode(FPathNode* node) : location(node->location), g(node->g), h(node->h), f(node->f), isEnd(node->isEnd) {}

	//UPROPERTY()
		FVector2D location;

	//cost from start
	//UPROPERTY()
		float g = 0;

	// heuristic
	//UPROPERTY()
		float h = 0;

	//final cost
	//UPROPERTY()
		float f = 0;

	//UPROPERTY()
		bool isEnd = false;

	//previous node in the path
		FPathNode* parent = nullptr;

	//next node in the path
		FPathNode* child = nullptr;

	bool operator ==(const FPathNode other) const { return this->location.Equals(other.location, 0.1); }
	bool operator !=(const FPathNode other) const { return !this->location.Equals(other.location, 0.1); }

	bool operator ==(const FVector2D location) const { return location == this->location; }
	bool operator !=(const FVector2D location) const { return location == this->location; }

	bool operator <(const FPathNode other) const { return this->f < other.f; }
	bool operator >(const FPathNode other) const { return this->f > other.f; }

	friend FORCEINLINE uint32 GetTypeHash(const FPathNode& s)
	{
		return GetTypeHash(s.location);
	}
	

	static FPathNode* GetLowest(TSet<FPathNode*> set);
	static bool SetContains(TSet<FPathNode*> set, FPathNode* test);
};

UCLASS()
class UPath : public UObject
{
	GENERATED_BODY()

public:
	friend class FCalculatePathTask;

	struct PathData
	{
		PathData(const TSet<struct FPathNode*> openNodes, const TSet<FPathNode*> closedNodes, const TArray<FPathNode*> path, const bool isComplete) :
		openNodes(openNodes), closedNodes(closedNodes), path(path), isComplete(isComplete) {}
		PathData() {}
		
		TSet<struct FPathNode*> openNodes;
		TSet<FPathNode*> closedNodes;
		TArray<FPathNode*> path;
		bool isComplete = false;
	};

	UPath() = default;
	UPath(FVector2D start, FVector2D end, class ACI_TileMap_CPP* tileMap);
	~UPath();

	void Init(FVector2D start, FVector2D end, int maxReach, class ACI_TileMap_CPP* tileMap);

	void StartCaluclatePath();

	void Stop();

	FVector2D start;
	FVector2D end;
	int maxReach = 0;

	ACI_TileMap_CPP* tileMap = nullptr;

	bool isFinished = false;

	class UObject* owner = nullptr;

	class FCalculatePathTask* task = nullptr;

	bool ReadQueue();

	PathData* outputData = nullptr;

	TQueue<PathData*, EQueueMode::Mpsc> outputQueue;
};



class FCalculatePathTask
	: public FRunnable
{

	FCalculatePathTask(FVector2D start, FVector2D end, int maxReach, ACI_TileMap_CPP* tileMap, UPath* owner);
	~FCalculatePathTask();

public:
	static TArray<FCalculatePathTask*> runnable;



	FRunnableThread * thread;
	FThreadSafeCounter stopTaskCounter;

	static FCalculatePathTask* CreatePathCalculator(FVector2D start, FVector2D end, int maxReach, ACI_TileMap_CPP* tileMap, UPath* owner);

	// Begin FRunnable interface.
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	// End FRunnable interface


	/** Makes sure this thread has stopped properly */
	void EnsureCompletion();

	static void Shutdown();
	void _Shutdown();
	static bool IsThreadFinished();
	bool _IsThreadFinished() const;

	bool isFinished = false;
	
	UPath::PathData* pathData = nullptr;

	FVector2D start;
	FVector2D end;
	int maxReach;

	ACI_TileMap_CPP* tileMap;

	UPath* owner;

	bool doRun = false;

	bool CheckNext(int x, int y) const;
	void DoWorkInternal();
	
	void ReturnCalculationFinished();
};
