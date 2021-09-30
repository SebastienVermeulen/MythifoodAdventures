#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent_C.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYTHIFOOD_API UHealthComponent_C : public UActorComponent
{
	GENERATED_BODY()

#pragma region DEFAULT
public:	
	UHealthComponent_C();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:
#pragma endregion

#pragma region UFUNC & UPROP
public:
	UFUNCTION(BlueprintCallable, Category = "Health", meta = (DisplayName = "Damage"))
		bool TakeDamage(int damage);
	UFUNCTION(BlueprintCallable, Category = "Health", meta = (DisplayName = "Revive"))
		void Revive();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health", meta = (DisplayName = "IsDead"))
		bool IsDead();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health", meta = (DisplayName = "GetHealth"))
		int GetHealth();
	UFUNCTION(BlueprintCallable, Category = "Health", meta = (DisplayName = "SetHealth"))
		void SetHealth(int newHealth);
	UFUNCTION(BlueprintCallable, Category = "Health", meta = (DisplayName = "SetMaxHealth"))
		void SetMaxHealth(int newMaxHealth);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health", meta=(DisplayName="health"))
		int m_Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (DisplayName = "health"))
		int m_MaxHealth;

private:
#pragma endregion
};
