// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CI_Pawn_CPP.generated.h"

//TODO:DOCUMENT comment/document this file

class UFloatingPawnMovement;
class UCameraComponent;

UCLASS()
class CASINO_INC_API ACI_Pawn_CPP : public APawn  // NOLINT
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACI_Pawn_CPP();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void CheckInput();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UCameraComponent* GetCamera() const { return cameraComp; }

	/**
	 * get's the bounds of the camera\n
	 * X = min X\n
	 * Y = min Y\n
	 * Z = max X\n
	 * W = max Y
	 */
	UFUNCTION(BlueprintCallable)
	FVector4 GetViewBounds();

	class UBuildTask* GetDefaultWall() const { return defaultWallTask; }
	class UBuildTask* GetDefaultFloor() const { return defaultFloorTask; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	void MoveHorizontal(float axisValue);
	void MoveVertical(float axisValue);
	void Zoom(float axisValue);
	void CalcInput();

	void ClearSelection();
	void BuildObjectPressed();
	void BuildObjectReleased() { bBuildPressed = false; }
	void BuildObject() const;
	void BuildRoom();
	void DestroyObjectPressed() { bDestroyPressed = true; }
	void DestroyObjectReleased() { bDestroyPressed = false; }
	void DestroyObject() const;

	void CheckBuildDestroy() const;

	void ToggleBuildRoom();

	void PrioritizePressed() { bPrioritizePressed = true; }
	void PrioritizeReleased() { bPrioritizePressed = false; }
	void PrioritizeTask();

private:
	void SetupCamera();

public:	
	
	//movement bools
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bMoveLeft = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bMoveRight = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bMoveUp = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bMoveDown = false;
	
	bool bBuildPressed = false;
	bool bDestroyPressed = false;
	bool bPrioritizePressed = false;

protected:

	UFloatingPawnMovement *movementComp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCameraComponent *cameraComp;

	class ACI_GameStateBase_CPP* gameState;

	/**
	 * X = min X\n
	 * Y = min Y\n
	 * Z = max X\n
	 * W = max Y
	 */
	FVector4 viewBounds;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool useOverdraw = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float zoomSpeedMultiplier;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float maxZoom;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		class UBuildTask* defaultWallTask;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		class UBuildTask* defaultFloorTask;
};
