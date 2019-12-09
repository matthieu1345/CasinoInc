// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AI/CI_AITypes.h"
#include "CI_BaseTask_CPP.generated.h"

//TODO:DOCUMENT comment/document this file

UENUM(BlueprintType)
enum class ETaskType : uint8
{
	TT_BuildWall		UMETA(DisplayName = "Build wall"),
	TT_BuildObject		UMETA(DisplayName = "Build object"),
	TT_BuildFloor		UMETA(DisplayName = "Build floor"),
	TT_DestroyWall		UMETA(DisplayName = "Destroy wall"),
	TT_DestroyObject	UMETA(DisplayName = "Destroy object"),
	TT_DestroyFloor		UMETA(DisplayName = "Destroy floor"),
	TT_DealerTask		UMETA(DisplayName = "Dealer Task"),
	TT_Invalid			UMETA(DisplayName = "INVALID")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTaskFinished, bool, successfull);

/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class CASINO_INC_API UCI_BaseTask_CPP : public UDataAsset
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void InitTask(int x, int y, class ACI_GameStateBase_CPP* gamestate);
	virtual void InitTask_Implementation(int _x, int _y, ACI_GameStateBase_CPP* _gamestate) {}

	virtual void CreatePreview(int x, int y, class ACI_GameStateBase_CPP* gamestate) {}

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void DestroyTask(int x, int y, class ACI_GameStateBase_CPP* gamestate);
	virtual void DestroyTask_Implementation(const int x, const int y, class ACI_GameStateBase_CPP* gamestate)
	{
		FinishTask(false, x, y, gamestate);
		this->MarkPendingKill();
	}

	bool CheckAllowed(const ECharacterType _type) const { return allowedAITypes.Contains(_type); }
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool CheckValidLocation(int _x, int _y, ACI_GameStateBase_CPP* _gamestate);
	virtual bool CheckValidLocation_Implementation(int _x, int _y, ACI_GameStateBase_CPP* _gamestate) { return true; };

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void FinishTask(bool success, int x, int y, class ACI_GameStateBase_CPP* gamestate);
	virtual void FinishTask_Implementation(bool success, int x, int y, class ACI_GameStateBase_CPP* gamestate);

	class UCI_GOAPActionBase_CPP *GetAIStateAction() const;

	int GetMoneyCost() const { return moneyCost; }
	float GetTaskLenght() const { return taskLenght; }

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		ETaskType type = ETaskType::TT_Invalid;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		TSet<ECharacterType> allowedAITypes;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		int moneyCost = 0;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class UCI_GOAPActionBase_CPP* aiStateAction;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		float taskLenght = 5.0f;
};

USTRUCT(BlueprintType)
struct FTask
{
	GENERATED_BODY()


		FTask() {}

	FTask(UCI_BaseTask_CPP* taskType, const int x, const int y, class ACI_BaseTile_CPP* tile = nullptr) : x(x), y(y), tile(tile) { this->taskType = taskType; }
	~FTask() {}

	void Init(ACI_GameStateBase_CPP* gamestate) const { this->taskType->InitTask_Implementation(x, y, gamestate); }
	void Finished(ACI_GameStateBase_CPP* gamestate) const;

	void DoTask(float deltaTime, ACI_GameStateBase_CPP* gamestate);

	void CreatePreview(class ACI_GameStateBase_CPP* gamestate) const { taskType->CreatePreview(x, y, gamestate); }

	FVector2D GetLocation() const { return FVector2D(x, y); }
	class ACI_BaseTile_CPP* GetTileLocation() const { return tile; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCI_BaseTask_CPP*taskType = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int x = -9999;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int y = -9999;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ACI_BaseTile_CPP*tile = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool hasPriority = false;

	bool finished = false;

	friend FORCEINLINE uint32 GetTypeHash(const FTask& s)
	{
		return GetTypeHash(s.x) + GetTypeHash(s.y);
	}

	bool operator ==(FTask other) const { return this->x == other.x && this->y == other.y && this->taskType->StaticClass() == other.taskType->StaticClass(); }
	bool IsValid(ACI_GameStateBase_CPP* gameState) const
	{
		if (x != -9999 && y != -9999)
			return IsValidLocation(gameState);

		return false;
	}

	bool IsValidLocation(ACI_GameStateBase_CPP* gamestate) const;

	float taskTimer = 0.0f;
	FTaskFinished taskFinished;

};