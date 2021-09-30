#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "QuestContainer_C.generated.h"

UENUM()
enum class eRating : uint8
{
	Easy UMETA(DisplayName = "Easy"),
	Medium UMETA(DisplayName = "Medium"),
	Hard UMETA(DisplayName = "Hard")
};

#pragma region Requirement
USTRUCT(BlueprintType)
struct FRequirement
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Requirement", meta = (DisplayName = "Type"))
		FDataTableRowHandle m_FoodType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Requirement", meta = (DisplayName = "Amount"))
		int m_Amount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Requirement", meta = (DisplayName = "Collecteds"))
		int m_Collected;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Requirement", meta = (DisplayName = "Finished"))
		bool m_Finished = false;
};
#pragma endregion

#pragma region Quest
UCLASS(BlueprintType, ClassGroup = (Custom))
class MYTHIFOOD_API AQuest_C : public AActor
{
	GENERATED_BODY()

public:
	AQuest_C();

	UFUNCTION(BlueprintCallable)
		bool TryAddCollected(FDataTableRowHandle newCollected);
	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool IsRequirementFilled();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest", meta = (DisplayName = "Ingredients"))
		TArray<FRequirement> m_Ingredients;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quests", meta = (DisplayName = "Rating"))
		eRating m_Rating;
};
#pragma endregion

#pragma region Container
UCLASS(BlueprintType, ClassGroup = (Custom))
class MYTHIFOOD_API AQuestContainer_C : public AActor
{
	GENERATED_BODY()
	
public:	
	AQuestContainer_C();

	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool TryAddCollected(FDataTableRowHandle newCollected);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool AreRequirementsFilled();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quests", meta = (DisplayName = "Quests"))
		TArray<AQuest_C*> m_ActiveQuests;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest", meta = (DisplayName = "Quest Assets"))
		TArray<TSubclassOf<AQuest_C>> m_Assets;
};
#pragma endregion
