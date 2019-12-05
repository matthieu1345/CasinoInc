#pragma once

#include "StateNodeTypes/CI_GOAPStateNodeTypeAll_CPP.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CI_GOAPStateContainer_CPP.generated.h"

//TODO:DOCUMENT comment/document this file

USTRUCT(BlueprintType)
struct FGOAPStateList
{
	GENERATED_BODY()

	FGOAPStateList()
	{
		id = FGuid::NewGuid();
	}

public:

	bool operator==(const FGOAPStateList other) const { return id == other.id; }
	bool operator!=(const FGOAPStateList other) const { return id != other.id; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, instanced)
	TMap<FString, UGOAPStateNodeType*> stateNodes;

	void Copy(FGOAPStateList other)
	{
		stateNodes.Empty();
		for (auto state : other.stateNodes)
		{
			stateNodes.Add(state.Key) = DuplicateObject(state.Value, state.Value->GetOuter());
		}
	}

private:
	FGuid id;
};

UCLASS()
class CASINO_INC_API UGOAPStateUtil : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintPure, Category = "GOAP|State")
	static bool TestEqual(FGOAPStateList const &first,FGOAPStateList const &second, int &difference)
	{
		difference = 0;

		for (const TPair<FString, UGOAPStateNodeType*> stateNode : first.stateNodes)
		{
			const UGOAPStateNodeType* secondValue = *second.stateNodes.Find(stateNode.Key);

			if (secondValue == nullptr)
			{
				difference++;
				continue;
			}

			const UGOAPStateNodeType* firstValue = stateNode.Value;

			if (!firstValue->TestEqual(secondValue))
			{
				difference++;
			}
		}

		for (const TPair<FString, UGOAPStateNodeType*> stateNode : second.stateNodes)
		{
			const UGOAPStateNodeType* secondValue = *first.stateNodes.Find(stateNode.Key);

			if (secondValue == nullptr)
			{
				difference++;
			}
		}

		

		return difference == 0;
	}

	UFUNCTION(BlueprintPure, Category = "GOAP|Test")
	static bool TestConditions(FGOAPStateList const &conditions, FGOAPStateList const &inState, int &difference)
	{
		difference = 0;
		for (const TPair<FString, UGOAPStateNodeType*> conditionNode : conditions.stateNodes)
		{
			if (!inState.stateNodes.Contains(conditionNode.Key) && !conditionNode.Value->TestDefault())
			{
				//the state does not have a node for the needed condition
				UE_LOG(GOAP, Verbose, TEXT("Condition check failed because %s did not exist!"), *conditionNode.Key);
				difference++;
				continue;
			}
			else if (!inState.stateNodes.Contains(conditionNode.Key) && conditionNode.Value->TestDefault())
			{
				continue;
			}

			const UGOAPStateNodeType* stateValue = *inState.stateNodes.Find(conditionNode.Key);
			const UGOAPStateNodeType* conditionValue = conditionNode.Value;

			if (!conditionValue->TestValue(stateValue))
			{
				// the value in the state does not compare correctly to the condition needed!
				UE_LOG(GOAP, Verbose, TEXT("Condition check failed because %s had the wrong value!"), *conditionNode.Key);
				difference++;
			}
		}

		return difference == 0;
	}

	static void Print(FGOAPStateList const &state)
	{
		int counter = 0;
		for (auto node : state.stateNodes)
		{
			UE_LOG(GOAP, Log, TEXT("- %d: %s : %s"), counter, *node.Key, *node.Value->GetFString())
				counter++;
		}
	}

};