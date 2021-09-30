#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/SkeletalMesh.h"
#include "Animation/AnimBlueprint.h"
#include "FoodDataTable_C.generated.h"

UENUM(BlueprintType)
enum class eFoodType : uint8
{
    spice = 0   UMETA(DisplayName = "Spice"),
    snack = 1   UMETA(DisplayName = "Snack"),
    stuffing = 2   UMETA(DisplayName = "Stuffing"),
    gourmet = 3   UMETA(DisplayName = "Gourmet")
};

UENUM(BlueprintType)
enum class eAmmoType : uint8
{
    none = 0   UMETA(DisplayName = "None"),
    machinegun = 1   UMETA(DisplayName = "Machinegun"),
    bouncebomb = 2   UMETA(DisplayName = "BounceBomb"),
    freezemortar = 3   UMETA(DisplayName = "FreezeMortar")
};

USTRUCT(BlueprintType)
struct FFoodData : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (DisplayName = "FoodType"))
        eFoodType m_FoodType = eFoodType::spice;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (DisplayName = "AmmoType"))
        eAmmoType m_AmmoType = eAmmoType::none;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (DisplayName = "Mesh"))
        USkeletalMesh* m_pMesh = nullptr;
     UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (DisplayName = "AnimBP"))
        UClass* m_pAnimBlueprintClass = nullptr;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (DisplayName = "Points"))
        int m_Points = 1;
};

USTRUCT(BlueprintType)
struct FIngredientAttach
{
    GENERATED_USTRUCT_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (DisplayName = "Attached"))
        class USceneComponent* m_pAttached;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (DisplayName = "Enemy"))
        class ABaseEnemy_C* m_pEnemy;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (DisplayName = "Data"))
        FFoodData m_Data;
};
