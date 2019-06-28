#pragma once

#include "CI_GOAPStateNodeTypeBase_CPP.h"
#include "Casino_Inc.h"
#include "CI_GOAPStateNodeTypeObject_CPP.generated.h"

UENUM(BlueprintType)
enum class EGOAPTestObjectCondition : uint8
{
	GTC_Class		UMETA(DisplayName = "class"),
	GTC_Object		UMETA(DisplayName = "Object")
};

UCLASS(EditInlineNew, BlueprintType)
class UGOAPStateNodeType_Object : public UGOAPStateNodeType
{

	GENERATED_BODY()

public:
	virtual bool TestEqual(const UGOAPStateNodeType* other) const override
	{
		if (UGOAPStateNodeType::TestEqual(other))
		{
			if (valueClass == GetValueClass<const UGOAPStateNodeType_Object>(other)->valueClass)
				return value == GetValueClass<const UGOAPStateNodeType_Object>(other)->value;
		}
		return false;
	}


	virtual bool TestValue(const UGOAPStateNodeType* other) const override
	{
		if (UGOAPStateNodeType::TestValue(other))
		{
			TSubclassOf<UObject> otherValueClass = GetValueClass<const UGOAPStateNodeType_Object>(other)->valueClass;

			switch (testOn)
			{
			case EGOAPTestObjectCondition::GTC_Class:

				switch (conditionType)
				{
				case EGOAPStateCondition::GSC_Equal:
					return valueClass->GetFName() == GetValueClass<const UGOAPStateNodeType_Object>(other)->valueClass->GetFName();

				case EGOAPStateCondition::GSC_NotEqual:
					return valueClass->GetFName() != GetValueClass<const UGOAPStateNodeType_Object>(other)->valueClass->GetFName();

				case EGOAPStateCondition::GSC_Nullptr: 
					return GetValueClass<const UGOAPStateNodeType_Object>(other)->valueClass == nullptr;

				case EGOAPStateCondition::GSC_NotNullptr: 
					return GetValueClass<const UGOAPStateNodeType_Object>(other)->valueClass != nullptr;

				default:
					return false;
				}

			case EGOAPTestObjectCondition::GTC_Object: 

				switch (conditionType)
				{
				case EGOAPStateCondition::GSC_Equal:
					return value == GetValueClass<const UGOAPStateNodeType_Object>(other)->value;

				case EGOAPStateCondition::GSC_NotEqual:
					return value != GetValueClass<const UGOAPStateNodeType_Object>(other)->value;

				case EGOAPStateCondition::GSC_Nullptr:
					return GetValueClass<const UGOAPStateNodeType_Object>(other)->value == nullptr;

				case EGOAPStateCondition::GSC_NotNullptr:
					return GetValueClass<const UGOAPStateNodeType_Object>(other)->value != nullptr;

				default:
					return false;
				}


			default:
				return false;
			}
		}

		return false;
	}

	virtual bool TestDefault() override
	{
		switch (testOn)
		{
		case EGOAPTestObjectCondition::GTC_Class:

			switch (conditionType)
			{
			case EGOAPStateCondition::GSC_Equal:
				return valueClass->GetFName() == FName();

			case EGOAPStateCondition::GSC_NotEqual:
				return valueClass->GetFName() != FName();

			case EGOAPStateCondition::GSC_Nullptr:
				return true;

			case EGOAPStateCondition::GSC_NotNullptr:
				return false;

			default:
				return false;
			}

		case EGOAPTestObjectCondition::GTC_Object:

			switch (conditionType)
			{
			case EGOAPStateCondition::GSC_Equal:
				return value == nullptr;

			case EGOAPStateCondition::GSC_NotEqual:
				return value != nullptr;

			case EGOAPStateCondition::GSC_Nullptr:
				return true;

			case EGOAPStateCondition::GSC_NotNullptr:
				return false;

			default:
				return false;
			}


		default:
			return false;
		}
	}

	virtual bool AddValue(const UGOAPStateNodeType* other) override
	{
		UGOAPStateNodeType::AddValue(other);
		SetValue(other->GetValueClass<const UGOAPStateNodeType_Object>(other)->value.Get());
		return true;
	}

	virtual bool RemoveValue(const UGOAPStateNodeType* other) override
	{
		value = nullptr;
		return true;
	}

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override
	{
		FName propertyName = PropertyChangedEvent.GetPropertyName();

		if (testOn != EGOAPTestObjectCondition::GTC_Class)
		{
			if (propertyName == GET_MEMBER_NAME_CHECKED(UGOAPStateNodeType_Object, value))
			{
				if (value != nullptr)
					valueClass = value->GetClass();
				else
					valueClass = nullptr;
			}
		}

		Super::PostEditChangeProperty(PropertyChangedEvent);
	}
#endif

	UFUNCTION(BlueprintCallable)
	void SetValue(UObject* newValue)
	{
		value = newValue;
		if (testOn != EGOAPTestObjectCondition::GTC_Class)
			valueClass = value->GetClass();
	}

	UFUNCTION(BlueprintCallable)
	void SetValueClass(TSubclassOf<UObject> newValueClass)
	{
		valueClass = newValueClass;
	}


	virtual FString GetFString() override
	{
		if (isCondition && testOn == EGOAPTestObjectCondition::GTC_Class)
		{
			return *FString::Printf(TEXT("Class: %s"), *valueClass->GetName());
		}

		return *FString::Printf(TEXT("Object: %s"), value.IsValid() ? *value->GetName() : TEXT("nullptr"));
	}
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	TWeakObjectPtr<UObject> value = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State", meta = (EditCondition = "isCondition"))
	TSubclassOf<UObject> valueClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, AdvancedDisplay, category = "condition", meta= (DisplayName="Test On:"))
		EGOAPTestObjectCondition testOn = EGOAPTestObjectCondition::GTC_Class;


};