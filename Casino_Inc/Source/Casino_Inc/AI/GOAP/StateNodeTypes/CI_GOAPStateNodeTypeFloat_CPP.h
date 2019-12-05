#pragma once

#include "CI_GOAPStateNodeTypeBase_CPP.h"
#include "Casino_Inc.h"
#include "CI_GOAPStateNodeTypeFloat_CPP.generated.h"

//TODO:DOCUMENT comment/document this file

UCLASS(EditInlineNew, BlueprintType)
class UGOAPStateNodeType_Float : public UGOAPStateNodeType
{

	GENERATED_BODY()

public:
	virtual bool TestEqual(const UGOAPStateNodeType* other) const override
	{
		if (UGOAPStateNodeType::TestEqual(other))
			return GetValueClass<const UGOAPStateNodeType_Float>(other)->value == value;

		return false;
	}

	virtual bool TestValue(const UGOAPStateNodeType* other) const override
	{
		if (!UGOAPStateNodeType::TestValue(other))
			return false;

		auto otherFloat = GetValueClass<const UGOAPStateNodeType_Float>(other);

		switch (conditionType)
		{
		case EGOAPStateCondition::GSC_Equal:
			return otherFloat->value == value;

		case EGOAPStateCondition::GSC_NotEqual:
			return otherFloat->value != value;

		case EGOAPStateCondition::GSC_More:
			return otherFloat->value > value;

		case EGOAPStateCondition::GSC_Less:
			return otherFloat->value < value;

		case EGOAPStateCondition::GSC_MoreEqual:
			return otherFloat->value >= value;
		case EGOAPStateCondition::GSC_LessEqual:
			return otherFloat->value <= value;

		default:
			UE_LOG(GOAP, Warning, TEXT("Doing wrong test on float state!"));
			return false;

		}
	}


	virtual bool AddValue(const UGOAPStateNodeType* other) override
	{
		UGOAPStateNodeType::AddValue(other);
		value += other->GetValueClass<const UGOAPStateNodeType_Float>(other)->value;

		if (isClamped && value < 0)
			value = 0;

		return true;
	}

	virtual bool RemoveValue(const UGOAPStateNodeType* other) override
	{
		value -= other->GetValueClass<const UGOAPStateNodeType_Float>(other)->value;

		if (isClamped && value < 0)
			value = 0;

		return true;
	}

	virtual FString GetFString() override
	{
		return *FString::Printf(TEXT("float: %f"), value);
	}

	virtual bool TestDefault() override
	{
		if (Super::TestDefault())
			return true;

		switch(conditionType)
		{

		case EGOAPStateCondition::GSC_Equal:
			return 0 == value;

		case EGOAPStateCondition::GSC_NotEqual:
			return 0 != value;

		case EGOAPStateCondition::GSC_More:
			return 0 > value;

		case EGOAPStateCondition::GSC_Less:
			return 0 < value;

		case EGOAPStateCondition::GSC_MoreEqual:
			return 0 >= value;

		case EGOAPStateCondition::GSC_LessEqual:
			return 0 <= value;

		}

		return false;
	}

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
		float value;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
		bool isClamped = false;
};