// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Tasks/CI_BaseTask_CPP.h"
#include "TileMap/CI_TileMapEnums.h"
#include "DestroyTask.generated.h"

//TODO:DOCUMENT comment/document this file

/**
 * 
 */
UCLASS()
class CASINO_INC_API UDestroyTask : public UCI_BaseTask_CPP
{
	GENERATED_BODY()

		UDestroyTask() { allowedAITypes.Add(ECharacterType::CT_Builder); }

public:
	virtual void InitTask_Implementation(int x, int y, ACI_GameStateBase_CPP* gamestate) override;
	virtual void CreatePreview(int x, int y, class ACI_GameStateBase_CPP* gamestate) override;
	virtual void DestroyTask_Implementation(int x, int y, class ACI_GameStateBase_CPP* gamestate) override;
	virtual bool CheckValidLocation_Implementation(int x, int y, ACI_GameStateBase_CPP* gamestate) override;
	virtual void FinishTask_Implementation(bool success, int x, int y, class ACI_GameStateBase_CPP* gamestate) override;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		ETileLayer tileLayer;
};
