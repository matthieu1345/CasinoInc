// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CI_BuildPreview_CPP.generated.h"

UCLASS()
class CASINO_INC_API ACI_BuildPreview_CPP : public AActor
{
	GENERATED_BODY()
	
	public:	
		// Sets default values for this actor's properties
		ACI_BuildPreview_CPP();
		// Called every frame
		virtual void Tick(float DeltaTime) override;

	protected:
		// Called when the game starts or when spawned
		virtual void BeginPlay() override;

	bool CheckValidPlacement(int newX, int newY);
		void UpdateLocation();
		void UpdatePreview();

		void UpdateSprite();
		void UpdateMap();
		void UpdateRoomMap();

	public:	

	protected:
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class UPaperSpriteComponent* spriteComponent;
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class UPaperTileMapComponent *tilemapComponent;

		class ACI_GameStateBase_CPP* gameState;

	private:

		bool previousSprite = false;
		bool previousMap = false;
		int oldX;
		int oldY;
		FVector homelocation = FVector(0, 0, 0);
};
