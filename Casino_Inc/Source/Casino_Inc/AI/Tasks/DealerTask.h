#pragma once

#include "CoreMinimal.h"
#include "AI/Tasks/CI_BaseTask_CPP.h"
#include "DealerTask.generated.h"

UCLASS()
class CASINO_INC_API UDealerTask : public UCI_BaseTask_CPP
{
	GENERATED_BODY()

	UDealerTask()
	{
		allowedAITypes.Add(ECharacterType::CT_Dealer);
		type = ETaskType::TT_DealerTask;
	}

public:
	virtual void FinishTask_Implementation(bool success, int x, int y, ACI_GameStateBase_CPP* gamestate) override;
	virtual bool CheckValidLocation_Implementation(int _x, int _y, ACI_GameStateBase_CPP* _gamestate) override;
};