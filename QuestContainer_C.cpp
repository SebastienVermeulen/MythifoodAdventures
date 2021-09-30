#include "QuestContainer_C.h"

#pragma region Quest
AQuest_C::AQuest_C()
	:m_Ingredients(TArray<FRequirement>())
{
	PrimaryActorTick.bCanEverTick = false;
}
void AQuest_C::BeginPlay()
{
	Super::BeginPlay();
}
bool AQuest_C::TryAddCollected(FDataTableRowHandle newCollected)
{
	//Go through all the ingredients
	for (FRequirement requirement : m_Ingredients)
	{
		//Is the ingredient already finished => Skip
		//Check if the collected one is of the same type
		if (!requirement.m_Finished && requirement.m_FoodType == newCollected)
		{
			//Add to collected and check if finished
			requirement.m_Collected++;
			if (requirement.m_Collected >= requirement.m_Amount)
				requirement.m_Finished = true; //Set to finished
			return true; //Return that one was added
		}
	}
	return false; //Return that none were added
}
bool AQuest_C::IsRequirementFilled()
{
	//Go through all the ingredients
	for (FRequirement requirement : m_Ingredients)
	{
		//Is the ingredient already finished
		if (!requirement.m_Finished) return false;
	}
	return true;
}
#pragma endregion

#pragma region Container
AQuestContainer_C::AQuestContainer_C()
	:m_ActiveQuests{ TArray<AQuest_C*>() }
	, m_Assets{ TArray<TSubclassOf<AQuest_C>>() }
{
	PrimaryActorTick.bCanEverTick = false;
}
void AQuestContainer_C::BeginPlay()
{
	Super::BeginPlay();
}
bool AQuestContainer_C::TryAddCollected(FDataTableRowHandle newCollected)
{
	for (AQuest_C* quest : m_ActiveQuests)
	{
		bool check = quest->TryAddCollected(newCollected);
		if (check) return true;
	}
	return false;
}
bool AQuestContainer_C::AreRequirementsFilled()
{
	for (AQuest_C* quest : m_ActiveQuests)
	{
		bool check = quest->IsRequirementFilled();
		if (check) return true;
	}
	return false;
}
#pragma endregion
