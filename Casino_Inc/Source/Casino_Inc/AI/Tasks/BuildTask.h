// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Tasks/CI_BaseTask_CPP.h"
#include "BuildTask.generated.h"

class ACI_GameStateBase_CPP;
/**
 * 
 */
UCLASS()
class CASINO_INC_API UBuildTask : public UCI_BaseTask_CPP
{
	GENERATED_BODY()

	UBuildTask() { allowedAITypes.Add(ECharacterType::CT_Builder); }
public:
	TSubclassOf<class UCI_BaseTileDataComponent_CPP> GetTileData() { return tileData; }

	virtual void InitTask_Implementation(int x, int y, ACI_GameStateBase_CPP* gamestate) override;
	virtual void CreatePreview(int x, int y, class ACI_GameStateBase_CPP* gamestate) override;
	virtual void DestroyTask_Implementation(int x, int y, class ACI_GameStateBase_CPP* gamestate) override;
	virtual bool CheckValidLocation_Implementation(int x, int y, ACI_GameStateBase_CPP* gamestate) override;
	virtual void FinishTask_Implementation(bool success, int x, int y, class ACI_GameStateBase_CPP* gamestate) override;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<class UCI_BaseTileDataComponent_CPP> tileData;

};
