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

// ReSharper disable once CppMemberFunctionMayBeStatic this will never be static, cause it's been called on a specific ai who's state has changed
// ReSharper disable once CppMemberFunctionMayBeConst this will not be const, ai state changes can and will directly change stuff about them
void ACI_BaseAIPawn_CPP::StateChanged_Implementation()
{
	
}

void ACI_BaseAIPawn_CPP::UnPossessed()
{
	// reset the controller reference
	possessingCustomBaseController = nullptr;
}

void ACI_BaseAIPawn_CPP::PossessedBy(AController* newController)
{
	Super::PossessedBy(newController);

	// keep a reference to a casted custom base for quicker access
	if (newController->GetClass()->IsChildOf(ACI_BaseAIController_CPP::StaticClass()))
	{
		possessingCustomBaseController = Cast<ACI_BaseAIController_CPP>(newController);
	}
}

void ACI_BaseAIPawn_CPP::Tick(const float deltaTime)
{
	Super::Tick(deltaTime);

	// update the body if we have one
	if (body != nullptr)
		UpdateBody();

}

void ACI_BaseAIPawn_CPP::SetupPlayerInputComponent(UInputComponent* playerInputComponent)
{
	Super::SetupPlayerInputComponent(playerInputComponent);
}

UCI_PathFollowingComponent_CPP* ACI_BaseAIPawn_CPP::GetPathFollowingComponent() const
{
	return possessingCustomBaseController->GetPathFollowingComponent();
}

FVector2D ACI_BaseAIPawn_CPP::GetTileCoordinates() const
{
	return UCI_TileMapCoordinateMath::WorldVectorToTile(GetActorLocation());
}
