#pragma once

#include "Engine/DataAsset.h"
#include "CI_GOAPStateNodeTypeBase_CPP.generated.h"

//TODO:DOCUMENT comment/document this file

UENUM(BlueprintType)
enum class EGOAPStateCondition : uint8
{
	GSC_Equal		UMETA(DisplayName = "=="),
	GSC_NotEqual	UMETA(DisplayName = "!="),
	GSC_More		UMETA(DisplayName = ">"),
	GSC_MoreEqual	UMETA(DisplayName = ">="),
	GSC_Less		UMETA(DisplayName = "<"),
	GSC_LessEqual	UMETA(DisplayName = "<="),
	GSC_Nullptr		UMETA(DisplayName = "Nullptr"),
	GSC_NotNullptr	UMETA(DisplayName = "Not Nullptr")
};

UCLASS(EditInlineNew, DefaultToInstanced, Abstract, BlueprintType)
class CASINO_INC_API UGOAPStateNodeType : public UObject
{
public:
	GENERATED_BODY()



	UFUNCTION(BlueprintCallable)
	virtual bool TestEqual(const UGOAPStateNodeType* other ) const
	{
		return other->GetClass() == this->GetClass();
	}

	UFUNCTION(BlueprintCallable)
	virtual bool TestValue(const UGOAPStateNodeType* other) const
	{
		return other->GetClass() == this->GetClass();
	}

	UFUNCTION(BlueprintCallable)
	virtual bool AddValue(const UGOAPStateNodeType* other)
	{
		conditionType = other->conditionType;
		isCondition = other->isCondition;
		return true;
	}

	UFUNCTION(BlueprintCallable)
	virtual bool RemoveValue(const UGOAPStateNodeType* other)
	{
		return true;
	}

	template<class t>
	t* GetValueClass(const UGOAPStateNodeType* other) const
	{
		return Cast<t>(other);
	}

	virtual FString GetFString()
	{
		return FString("invalid");
	}

	virtual bool TestDefault()
	{
		return conditionType == EGOAPStateCondition::GSC_Nullptr;
	}

	virtual bool IsPostLoadThreadSafe() const override { return true; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, AdvancedDisplay, category = "condition")
		bool isCondition = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, AdvancedDisplay, category = "condition", meta = (EditCondition = "isCondition"))
		EGOAPStateCondition conditionType;
};