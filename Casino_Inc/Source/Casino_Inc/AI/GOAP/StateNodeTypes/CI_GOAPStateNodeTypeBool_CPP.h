#pragma once

#include "CI_GOAPStateNodeTypeBase_CPP.h"
#include "Casino_Inc.h"
#include "CI_GOAPStateNodeTypeBool_CPP.generated.h"

UCLASS(EditInlineNew, BlueprintType)
class UGOAPStateNodeType_Bool : public UGOAPStateNodeType
{

	GENERATED_BODY()
public:

	virtual bool TestEqual(const UGOAPStateNodeType* other) const override
	{
		if (UGOAPStateNodeType::TestEqual(other))
			return value == GetValueClass<const UGOAPStateNodeType_Bool>(other)->value;

		return false;
	}

	virtual bool TestValue(const UGOAPStateNodeType* other) const override
	{
		if (UGOAPStateNodeType::TestEqual(other))
		{
			switch (conditionType)
			{
			case EGOAPStateCondition::GSC_Equal: 
				return TestEqual(other);
			case EGOAPStateCondition::GSC_NotEqual:
				return !TestEqual(other);
			default: 
				
				UE_LOG(GOAP, Warning, TEXT("Doing wrong test on bool state!"));
			}
		}

		return false;
	}

	virtual bool AddValue(const UGOAPStateNodeType* other) override
	{
		UGOAPStateNodeType::AddValue(other);
		value = other->GetValueClass<const UGOAPStateNodeType_Bool>(other)->value;
		return true;
	}

	virtual bool RemoveValue(const UGOAPStateNodeType* other) override
	{
		value = !other->GetValueClass<const UGOAPStateNodeType_Bool>(other)->value;
		return true;
	}


	virtual FString GetFString() override
	{
		return *FString::Printf(TEXT("bool: %s"), value ? TEXT("True") : TEXT("False"));
	}

	virtual bool TestDefault() override
	{
		if (Super::TestDefault())
			return true;

		if (conditionType == EGOAPStateCondition::GSC_NotEqual && value ||
			conditionType == EGOAPStateCondition::GSC_Equal && !value)
			return true;

		return false;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
		bool value;

};