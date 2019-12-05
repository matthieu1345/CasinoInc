// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CI_PlayerController_CPP.generated.h"

//TODO:DOCUMENT comment/document this file

/**
 * 
 */
UCLASS()
class CASINO_INC_API ACI_PlayerController_CPP : public APlayerController
{
	GENERATED_BODY()
	
	virtual void BeginPlay() override;

public:
	void GetMouseTilePosition(int32& x, int32& y);
	void GetMouseWorldPosition(int32& x, int32& y);
};
