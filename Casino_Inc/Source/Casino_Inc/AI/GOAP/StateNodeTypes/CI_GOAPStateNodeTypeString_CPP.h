#pragma once

#include "CI_GOAPStateNodeTypeBase_CPP.h"
#include "Casino_Inc.h"
#include "CI_GOAPStateNodeTypeString_CPP.generated.h"

UCLASS(EditInlineNew, BlueprintType)
class UGOAPStateNodeType_String : public UGOAPStateNodeType
{

	GENERATED_BODY()


public:
	virtual bool TestEqual(const UGOAPStateNodeType* other) const override
	{
		if (!UGOAPStateNodeType::TestEqual(other))
			return false;

		return value == other->GetValueClass<const UGOAPStateNodeType_String>(other)->value;
	}

	virtual bool TestValue(const UGOAPStateNodeType* other) const override
	{
		if (!UGOAPStateNodeType::TestValue(other))
			return false;


		FString otherValue = other->GetValueClass<const UGOAPStateNodeType_String>(other)->value;

		switch (conditionType)
		{
		case EGOAPStateCondition::GSC_Equal: 
			return otherValue == value;

		case EGOAPStateCondition::GSC_NotEqual: 
			return otherValue != value;

		case EGOAPStateCondition::GSC_Nullptr: 
			return otherValue.IsEmpty();

		case EGOAPStateCondition::GSC_NotNullptr: 
			return !otherValue.IsEmpty();

		default: 
			UE_LOG(GOAP, Warning, TEXT("Doing wrong test on string state!"));
			return false;
		}
	}


	virtual bool TestDefault() override
	{
		if (Super::TestDefault())
			return true;

		switch (conditionType)
		{
		case EGOAPStateCondition::GSC_Equal:
			return FString() == value;

		case EGOAPStateCondition::GSC_NotEqual:
			return FString() != value;

		case EGOAPStateCondition::GSC_Nullptr:
			return true;

		case EGOAPStateCondition::GSC_NotNullptr:
			return false;

		default:
			UE_LOG(GOAP, Warning, TEXT("Doing wrong test on string state!"));
			return false;
		}
	}

	virtual bool AddValue(const UGOAPStateNodeType* other) override
	{
		UGOAPStateNodeType::AddValue(other);
		value = other->GetValueClass<const UGOAPStateNodeType_String>(other)->value;
		return true;
	}

	virtual bool RemoveValue(const UGOAPStateNodeType* other) override
	{
		if (other->TestEqual(other))
		{
			value = FString();
			conditionType = other->conditionType;
			isCondition = other->isCondition;
			return true;
		}
		else
		{
			value = other->GetValueClass<const UGOAPStateNodeType_String>(other)->value;
			conditionType = other->conditionType;
			isCondition = other->isCondition;
			return true;
		}

		return false;
	}

	virtual FString GetFString() override
	{
		return *FString::Printf(TEXT("String: %s"), *value);
	}

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
		FString value;
};
