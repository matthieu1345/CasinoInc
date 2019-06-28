// Fill out your copyright notice in the Description page of Project Settings.

#include "CI_Pawn_CPP.h"

#include "Engine/Classes/GameFramework/FloatingPawnMovement.h"
#include "Camera/CameraComponent.h"
#include "CI_GameStateBase_CPP.h"
#include "Engine/Classes/Engine/World.h"
#include "TileMap/CI_BaseTile_CPP.h"
#include "Casino_Inc.h"
#include "CI_PlayerController_CPP.h"
#include "TileMap/CI_TileMap_CPP.h"
#include "TileMap/CI_BaseTileDataComponent_CPP.h"
#include "UI/CI_BuildPreview_CPP.h"
#include "AI/CI_AIManager_CPP.h"
#include "AI/Tasks/BuildTask.h"
#include "Managers/CI_StatsManager_CPP.h"


// Sets default values
ACI_Pawn_CPP::ACI_Pawn_CPP()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	movementComp = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("movement") );

	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	cameraComp->SetupAttachment(RootComponent);
	SetupCamera();
}

void ACI_Pawn_CPP::SetupCamera()
{
	cameraComp->SetProjectionMode(ECameraProjectionMode::Orthographic);
	cameraComp->SetOrthoWidth(1920);
	cameraComp->SetOrthoFarClipPlane(1000.0f);

	FMinimalViewInfo cameraView;
	cameraComp->GetCameraView(0.0f, cameraView);

	viewBounds.Y = cameraView.OrthoWidth / 2 * -1;
	viewBounds.X = cameraView.OrthoWidth / cameraView.AspectRatio * -1;
	viewBounds.W = viewBounds.X * -1;
	viewBounds.Z = viewBounds.Y * -1;	
}

FVector4 ACI_Pawn_CPP::GetViewBounds()
{
	FMinimalViewInfo cameraView;
	cameraComp->GetCameraView(0.0f, cameraView);

	viewBounds.Y = cameraView.OrthoWidth / 2 * -1;
	viewBounds.X = cameraView.OrthoWidth / cameraView.AspectRatio * -1;
	viewBounds.W = viewBounds.X * -1;
	viewBounds.Z = viewBounds.Y * -1;

	FVector4 worldBounds;

	worldBounds.X = viewBounds.X + GetActorTransform().GetLocation().X;
	worldBounds.Z = viewBounds.Z + GetActorTransform().GetLocation().X;
	worldBounds.Y = viewBounds.Y + GetActorTransform().GetLocation().Y;
	worldBounds.W = viewBounds.W + GetActorTransform().GetLocation().Y;

	if (useOverdraw)
	{
		worldBounds.X -= ACI_BaseTile_CPP::TILE_SIZE;
		worldBounds.Z += ACI_BaseTile_CPP::TILE_SIZE;
		worldBounds.Y -= ACI_BaseTile_CPP::TILE_SIZE;
		worldBounds.W += ACI_BaseTile_CPP::TILE_SIZE;
	}

	return worldBounds;
}
	
// Called when the game starts or when spawned
void ACI_Pawn_CPP::BeginPlay()
{
	Super::BeginPlay();
	gameState = Cast<ACI_GameStateBase_CPP>(GetWorld()->GetGameState());
	gameState->currentPawn = this;
	cameraComp->SetWorldRotation(FRotator(-90, -90, 0));

	GetWorld()->SpawnActor<ACI_BuildPreview_CPP>(FVector(0, 0, 0), FRotator(0, 0, 0));
}

void ACI_Pawn_CPP::MoveHorizontal(float axisValue)
{
	bMoveRight = false;
	bMoveLeft = false;

	if (axisValue > 0)
		bMoveRight = true;
	else if (axisValue < 0)
		bMoveLeft = true;
}

void ACI_Pawn_CPP::MoveVertical(float axisValue)
{
	bMoveUp = false;
	bMoveDown = false;

	if (axisValue > 0)
		bMoveUp = true;
	else if (axisValue < 0)
		bMoveDown = true;
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ACI_Pawn_CPP::Zoom(float axisValue)
{

	cameraComp->OrthoWidth -= axisValue * zoomSpeedMultiplier;

	if (cameraComp->OrthoWidth <= maxZoom)
		cameraComp->OrthoWidth = maxZoom;

}

void ACI_Pawn_CPP::CalcInput()
{
	FVector moveDirection = FVector::ZeroVector;
	
	if (bMoveRight)
	{
		moveDirection.X += 1;
	}
	if (bMoveLeft)
	{
		moveDirection.X -= 1;
	}
	if (bMoveUp)
	{
		moveDirection.Y -= 1;
	}
	if (bMoveDown)
	{
		moveDirection.Y += 1;
	}

	gameState->movedThisTick = moveDirection != FVector::ZeroVector;

	AddMovementInput(moveDirection);

}

// ReSharper disable once CppMemberFunctionMayBeConst
void ACI_Pawn_CPP::ClearSelection()
{
	gameState->ChangeMode(EBuildInputMode::BIM_None);
}

void ACI_Pawn_CPP::BuildObjectPressed()
{
	if (!ACI_StatsManager_CPP::GetInstance(GetWorld())->HasMoneyToBuild())
		return;

	bBuildPressed = true;

	if (gameState->GetCurrentMode() == EBuildInputMode::BIM_BuildRoom)
		BuildRoom();
}

void ACI_Pawn_CPP::BuildObject() const
{
	if (gameState->GetSelectedBuildObject() == nullptr)
		return;

	if (!ACI_StatsManager_CPP::GetInstance(GetWorld())->HasMoneyToBuild())
		return;
	
	int x, y;

	gameState->currentController->GetMouseTilePosition(x, y);

	UE_LOG(PlayerInputLog, Verbose, TEXT("Player tried to build a %s at %d/%d"), *UCI_BaseTileDataComponent_CPP::GetTileTypeName(gameState->GetSelectedBuildObject()), x, y);

	if (gameState->GetCheatMode() || gameState->GetSelectedTask() == nullptr)
	{
		gameState->tileMap->SetTile(x, y, gameState->GetSelectedBuildObject());
	}
	else
	{
		ACI_AIManager_CPP::GetInstance(GetWorld())->CreateNewTask(gameState->GetSelectedTask(), x, y);
	}
}

void ACI_Pawn_CPP::BuildRoom()
{
	if (!gameState->isSelecting2NdCorner)
	{
		int x, y;
		gameState->currentController->GetMouseTilePosition(x, y);
		gameState->selectedBuildCorner = FVector2D(x, y);

		gameState->isSelecting2NdCorner = true;
	}
	else
	{
		int x2NdCorner, y2NdCorner;
		gameState->currentController->GetMouseTilePosition(x2NdCorner, y2NdCorner);

		const int xStart = FMath::Min((int)gameState->selectedBuildCorner.X, x2NdCorner);
		const int xEnd = FMath::Max((int)gameState->selectedBuildCorner.X, x2NdCorner);
		const int yStart = FMath::Min((int)gameState->selectedBuildCorner.Y, y2NdCorner);
		const int yEnd = FMath::Max((int)gameState->selectedBuildCorner.Y, y2NdCorner);

		for (int x = xStart; x <= xEnd; x++)
		{
			for (int y = yStart; y <= yEnd; y++)
			{
				if (x == xStart || x == xEnd || y == yStart || y == yEnd)
				{
					if (gameState->GetCheatMode())
					{
						gameState->tileMap->SetTile(x, y, defaultWallTask->GetTileData());
					}
					else
					{
						if (ACI_AIManager_CPP::GetInstance(GetWorld()).IsValid())
							ACI_AIManager_CPP::GetInstance(GetWorld())->CreateNewTask(defaultWallTask, x, y);
					}
				}

				if (gameState->GetCheatMode())
				{
					gameState->tileMap->SetTile(x, y, defaultFloorTask->GetTileData());
				}
				else
				{
					if (ACI_AIManager_CPP::GetInstance(GetWorld()).IsValid())
						ACI_AIManager_CPP::GetInstance(GetWorld())->CreateNewTask(defaultFloorTask, x, y);
				}

			}
		}

		gameState->isSelecting2NdCorner = false;
	}
}

void ACI_Pawn_CPP::PrioritizeTask()
{
	int x, y;
	gameState->currentController->GetMouseTilePosition(x, y);

	auto task = ACI_AIManager_CPP::GetInstance(GetWorld())->GetTask(x, y);
	
	if (task)
	{
		if (!task->hasPriority)
		{
			ACI_AIManager_CPP::GetInstance(GetWorld())->PrioritiseTask(task);
		}
	}
}

void ACI_Pawn_CPP::DestroyObject() const
{
	int x, y;

	gameState->currentController->GetMouseTilePosition(x, y);

	UE_LOG(PlayerInputLog, Verbose, TEXT("Player tried to destroy at %d/%d"), x, y);
	
	if (gameState->GetCheatMode())
	{
		gameState->tileMap->DestroyUpperTile(x, y);
	}
	else
	{
		gameState->tileMap->CreateDestroyUpperTask(x,y);
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ACI_Pawn_CPP::ToggleBuildRoom()
{
	if (gameState->GetCurrentMode() == EBuildInputMode::BIM_BuildRoom)
		gameState->ChangeMode(EBuildInputMode::BIM_None);
	else
		gameState->ChangeMode(EBuildInputMode::BIM_BuildRoom);
}

// Called every frame
void ACI_Pawn_CPP::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CalcInput();

	CheckInput();
}

void ACI_Pawn_CPP::CheckInput()
{
	/*
	 * bBuildPressed;
	 * bDestroyPressed;
	 * bPrioritizePressed;
	 */

	if (bBuildPressed && gameState->GetCurrentMode() == EBuildInputMode::BIM_Build)
	{
		BuildObject();
	}
	if (bDestroyPressed && (gameState->GetCurrentMode() == EBuildInputMode::BIM_Destroy || !bBuildPressed))
	{
		DestroyObject();
	}
	if (bBuildPressed && gameState->GetCurrentMode() == EBuildInputMode::BIM_BuildRoom)
	{
		//checked on the press
	}
	if (bPrioritizePressed && gameState->GetCurrentMode() == EBuildInputMode::BIM_Priority)
	{
		PrioritizeTask();
	}

}

// Called to bind functionality to input
void ACI_Pawn_CPP::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// ReSharper disable CppInitializedValueIsAlwaysRewritten
	FInputAxisBinding moveHorizontalBinding = InputComponent->BindAxis("Move Horizontal", this, &ACI_Pawn_CPP::MoveHorizontal);
	FInputAxisBinding moveVerticalBinding = InputComponent->BindAxis("Move Vertical", this, &ACI_Pawn_CPP::MoveVertical);
	FInputAxisBinding zoomBinding = InputComponent->BindAxis("Zoom", this, &ACI_Pawn_CPP::Zoom);
	FInputActionBinding clearSelectionBinding = InputComponent->BindAction("Clear Selection", IE_Pressed, this, &ACI_Pawn_CPP::ClearSelection);
	FInputActionBinding buildObjectPressedBinding = InputComponent->BindAction("Build Object", IE_Pressed, this, &ACI_Pawn_CPP::BuildObjectPressed);
	FInputActionBinding buildObjectReleasedBinding = InputComponent->BindAction("Build Object", IE_Released, this, &ACI_Pawn_CPP::BuildObjectReleased);
	FInputActionBinding destroyObjectPressedBinding = InputComponent->BindAction("Destroy Object", IE_Pressed, this, &ACI_Pawn_CPP::DestroyObjectPressed);
	FInputActionBinding destroyObjectReleasedBinding = InputComponent->BindAction("Destroy Object", IE_Released, this, &ACI_Pawn_CPP::DestroyObjectReleased);
	FInputActionBinding toggleRoomModeBinding = InputComponent->BindAction("Toggle Room Mode", IE_Pressed, this, &ACI_Pawn_CPP::ToggleBuildRoom);
	FInputActionBinding prioritiseTaskPressedBinding = InputComponent->BindAction("Prioritize Task", IE_Pressed, this, &ACI_Pawn_CPP::PrioritizePressed);
	FInputActionBinding prioritiseTaskReleasedBinding = InputComponent->BindAction("Prioritize Task", IE_Released, this, &ACI_Pawn_CPP::PrioritizeReleased);
	// ReSharper restore CppInitializedValueIsAlwaysRewritten

	moveHorizontalBinding.bConsumeInput = false;
	moveVerticalBinding.bConsumeInput = false;
	zoomBinding.bConsumeInput = false;
	clearSelectionBinding.bConsumeInput = false;
	buildObjectPressedBinding.bConsumeInput = false;
	buildObjectReleasedBinding.bConsumeInput = false;
	destroyObjectPressedBinding.bConsumeInput = false;
	destroyObjectReleasedBinding.bConsumeInput = false;
	toggleRoomModeBinding.bConsumeInput = false;
	prioritiseTaskPressedBinding.bConsumeInput = false;
	prioritiseTaskReleasedBinding.bConsumeInput = false;
}

