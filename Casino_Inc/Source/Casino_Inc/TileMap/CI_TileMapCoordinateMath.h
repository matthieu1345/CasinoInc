// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CI_TileMapCoordinateMath.generated.h"

//TODO:DOCUMENT comment/document this file

/**
 * 
 */
UCLASS()
class CASINO_INC_API UCI_TileMapCoordinateMath : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
		static FVector2D WorldVectorToTile(FVector input);

	UFUNCTION(BlueprintCallable)
		static FVector2D WorldFloatsToTile(float x, float y);

	UFUNCTION(BlueprintCallable)
		static FVector TileVectorToWorldCorner(FVector2D input, float height = 0);

	UFUNCTION(BlueprintCallable)
		static FVector TileFloatsToWorldCorner(float x, float y, float height = 0);

	UFUNCTION(BlueprintCallable)
		static FVector TileVectorToWorldCenter(FVector2D input, float height = 0);

	UFUNCTION(BlueprintCallable)
		static FVector TileFloatsToWorldCenter(float x, float y, float height = 0);
};
