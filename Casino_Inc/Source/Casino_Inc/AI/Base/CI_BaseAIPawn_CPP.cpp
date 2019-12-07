// Fill out your copyright notice in the Description page of Project Settings.

#include "CI_BaseAIPawn_CPP.h"
#include "PathFinding/CI_PathFollowingComponent_CPP.h"
#include "PaperFlipbookComponent.h"
#include "PaperGroupedSpriteComponent.h"
#include "CI_BaseAIController_CPP.h"
#include "TileMap/CI_BaseTile_CPP.h"
#include "TileMap/CI_TileMapCoordinateMath.h"
#include "DebugMacros.h"

ACI_BaseAIPawn_CPP::ACI_BaseAIPawn_CPP()
{
 	// Set this pawn to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessAI = EAutoPossessAI::Disabled;
	AutoPossessPlayer = EAutoReceiveInput::Disabled;

	// Create a root object
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	// Add a head component
	head = CreateDefaultSubobject<UPaperGroupedSpriteComponent>(TEXT("Head"));
	head->SetupAttachment(RootComponent);
}

void ACI_BaseAIPawn_CPP::BeginPlay()
{
	Super::BeginPlay();
	
#if WITH_EDITOR // set the world outliner path for the pawn
	SetFolderPath("AICharacter");
#endif
}

void ACI_BaseAIPawn_CPP::UpdateBody()
{
	if (possessingCustomBaseController == nullptr)
		return; // nothing needs to be done when this pawn isn't possessed by a casino inc controller


	if (bodyBooks.Num() == 4) // if we have the correct amount of body's (1 for each direction)
	{
		if (possessingCustomBaseController->GetPathFollowingComponent()->GetLastMovement().IsNearlyZero())
		{
			body->Stop(); // stop animating the body when the pawn is not moving
			return;
		}
		else
			body->Play();

		// find which side we're moving the most towards, horizontal or vertical
		if (FMath::Abs(possessingCustomBaseController->GetPathFollowingComponent()->GetLastMovement().X) > FMath::Abs(possessingCustomBaseController->GetPathFollowingComponent()->GetLastMovement().Y))
		{
			if (possessingCustomBaseController->GetPathFollowingComponent()->GetLastMovement().X > 0) // moving left or right?
			{
				body->SetFlipbook(bodyBooks[0]);
			}
			else
			{
				body->SetFlipbook(bodyBooks[1]);
			}
		}
		else
		{
			if (possessingCustomBaseController->GetPathFollowingComponent()->GetLastMovement().Y > 0) // moving up or down
			{
				body->SetFlipbook(bodyBooks[2]);
			}
			else
			{
				body->SetFlipbook(bodyBooks[3]);
			}
		}
	}
}

void ACI_BaseAIPawn_CPP::StateChanged_Implementation()
{
	
}

void ACI_BaseAIPawn_CPP::UnPossessed()
{
	// reset the controller reference
	possessingCustomBaseController = nullptr;
}

void ACI_BaseAIPawn_CPP::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// keep a reference to a casted custom base for quicker access
	if (NewController->GetClass()->IsChildOf(ACI_BaseAIController_CPP::StaticClass()))
	{
		possessingCustomBaseController = Cast<ACI_BaseAIController_CPP>(NewController);
	}
}

void ACI_BaseAIPawn_CPP::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// update the body if we have one
	if (body != nullptr)
		UpdateBody();

}

void ACI_BaseAIPawn_CPP::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

UCI_PathFollowingComponent_CPP* ACI_BaseAIPawn_CPP::GetPathFollowingComponent()
{
	return possessingCustomBaseController->GetPathFollowingComponent();
}

FVector2D ACI_BaseAIPawn_CPP::GetTileCoordinates()
{
	return UCI_TileMapCoordinateMath::WorldVectorToTile(GetActorLocation());
}
