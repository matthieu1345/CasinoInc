// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Interface.h"
#include "CI_RegisterAbleInterface_CPP.generated.h"

//TODO:DOCUMENT comment/document this file

UINTERFACE(BlueprintType)
class CASINO_INC_API UCI_RegisterAbleInterface_CPP : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CASINO_INC_API ICI_RegisterAbleInterface_CPP
{
	GENERATED_BODY()

public:
	// ReSharper disable CppHiddenFunction interface functions can never be virtual in UE4
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Registerable")
		FString GetRegisterName();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Registerable")
		FVector2D GetRegisterLocation();
	// ReSharper restore CppHiddenFunction
};
