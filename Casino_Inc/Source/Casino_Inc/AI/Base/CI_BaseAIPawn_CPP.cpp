// Fill out your copyright notice in the Description page of Project Settings.

#include "CI_BaseAIPawn_CPP.h"
#include "PathFinding/CI_PathFollowingComponent_CPP.h"
#include "PaperFlipbookComponent.h"
#include "PaperGroupedSpriteComponent.h"
#include "CI_BaseAIController_CPP.h"
#include "TileMap/CI_BaseTile_CPP.h"
#include "TileMap/CI_TileMapCoordinateMath.h"
#include "DebugMacros.h"

//TODO:DOCUMENT comment/document this file

// Sets default values
ACI_BaseAIPawn_CPP::ACI_BaseAIPawn_CPP()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessAI = EAutoPossessAI::Disabled;
	AutoPossessPlayer = EAutoReceiveInput::Disabled;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	head = CreateDefaultSubobject<UPaperGroupedSpriteComponent>(TEXT("Head"));
	head->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACI_BaseAIPawn_CPP::BeginPlay()
{
	Super::BeginPlay();
	
#if WITH_EDITOR
	SetFolderPath("AICharacter");
#endif
}

void ACI_BaseAIPawn_CPP::UpdateBody()
{
	if (possessingCustomBaseController == nullptr)
		return;


	if (bodyBooks.Num() == 4)
	{
		if (possessingCustomBaseController->GetPathFollowingComponent()->GetLastMovement().IsNearlyZero())
		{
			body->Stop();
			return;
		}
		else
			body->Play();

		if (FMath::Abs(possessingCustomBaseController->GetPathFollowingComponent()->GetLastMovement().X) > FMath::Abs(possessingCustomBaseController->GetPathFollowingComponent()->GetLastMovement().Y))
		{
			if (possessingCustomBaseController->GetPathFollowingComponent()->GetLastMovement().X > 0)
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
			if (possessingCustomBaseController->GetPathFollowingComponent()->GetLastMovement().Y > 0)
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
	possessingCustomBaseController = nullptr;
}

void ACI_BaseAIPawn_CPP::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (NewController->GetClass()->IsChildOf(ACI_BaseAIController_CPP::StaticClass()))
	{
		possessingCustomBaseController = Cast<ACI_BaseAIController_CPP>(NewController);
	}
}

// Called every frame
void ACI_BaseAIPawn_CPP::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (body != nullptr)
		UpdateBody();

}

// Called to bind functionality to input
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
