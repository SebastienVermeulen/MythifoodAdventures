#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BasePlayer_C.generated.h"

UCLASS()
class MYTHIFOOD_API ABasePlayer_C : public ACharacter
{
	GENERATED_BODY()

#pragma region DEFAULT
public:
	ABasePlayer_C();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	void MoveForward(float axis);
	void MoveRight(float axis);

	void DoAttack();

	class UHealthComponent_C* m_pHealthComponent;

	FVector2D m_LastMov;
	bool m_IsAttached;

	//Attacking Vars
	bool m_Attacking;
	bool m_ChainAttack;
	bool m_ComboLocked;

	//DEBUG
	bool m_DebugMode;

	class UAnimMontage* m_CurrentMontage;
private:
	struct FTimerHandle m_StunTimerHandle;
	float m_MaxMovementSpeed;
	float m_ItemHoldWeight;

#pragma endregion

#pragma region UFUNC & UPROP
public:
	UFUNCTION(BlueprintCallable, Category = "Attachement", meta = (DisplayName = "ToggleAttached"))
		void ToggleAttached();
	UFUNCTION(BlueprintCallable, Category = "Items", meta = (DisplayName = "PickUpIngredient"))
		void PickUpIngredient(AActor* pActor);
	UFUNCTION(BlueprintCallable, Category = "Items", meta = (DisplayName = "DropAllIngredients"))
		void DropAllIngredients();

	UFUNCTION(BlueprintCallable, Category = "Buff/Debuff", meta = (DisplayName = "Stun"))
		void RecieveDamage(const int damage);

	UFUNCTION(BlueprintCallable, Category = "Buff/Debuff", meta = (DisplayName = "Stun"))
		void Stun(const float stunDuration = 2.f);
	UFUNCTION(BlueprintCallable, Category = "Buff/Debuff", meta = (DisplayName = "EndStun"))
		void EndStun();

	UFUNCTION(BlueprintCallable, Category = "Buff/Debuf", meta = (DisplayName = "Slow"))
		void Slow(float slowPercent = 0.5f, bool reset = false);

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Buff/Debuf", meta = (DisplayName = "Slow"))
		FVector2D GetLastMov() const;

	UFUNCTION(BlueprintCallable, Category = "Combo", meta = (DisplayName = "ChainAttack"))
		void ChainAttack();
	UFUNCTION(BlueprintCallable, Category = "Combo", meta = (DisplayName = "ResetCombo"))
		void ResetCombo();
	UFUNCTION(BlueprintCallable, Category = "Combo", meta = (DisplayName = "Attack"))
		void Attack();
	UFUNCTION(BlueprintCallable, Category = "Combo", meta = (DisplayName = "AttackSlow"))
		void AttackSlow(float slowPercent = 0.5f, bool reset = false);
	UFUNCTION(BlueprintCallable, Category = "Combo", meta = (DisplayName = "ComboLock"))
		void ComboLock(bool locked);

	UFUNCTION(BlueprintCallable, Category = "DEBUG", meta = (DisplayName = "DEBUGMODETOGGLE"))
		void ToggleDebugMode();

protected:
#pragma region SFX
#pragma endregion

#pragma region Combo
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo", Meta = (DisplayName = "ComboCounter"))
		int m_ComboCounter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo", Meta = (DisplayName = "AttackSlow"))
		float m_AttackSlow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations", Meta = (DisplayName = "ComboMontages"))
		TArray<class UAnimMontage*> m_ComboMontages;


#pragma endregion

#pragma region RFX
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RFX", meta = (DisplayName = "Walking"))
	class UNiagaraSystem* m_pWalkingEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RFX", meta = (DisplayName = "WalkingComp"))
	class UNiagaraComponent* m_pWalkingEffectComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RFX", meta = (DisplayName = "Stun"))
	class UNiagaraSystem* m_pStunEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RFX", meta = (DisplayName = "StunComp"))
	class UNiagaraComponent* m_pStunEffectComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RFX", meta = (DisplayName = "Slow"))
	class UNiagaraSystem* m_pSlowEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RFX", meta = (DisplayName = "SlowComp"))
	class UNiagaraComponent* m_pSlowEffectComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RFX", meta = (DisplayName = "ChargeAttackCharge"))
	class UNiagaraSystem* m_pChargeAttackChargeEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RFX", meta = (DisplayName = "ChargeAttackChargeComp"))
	class UNiagaraComponent* m_pChargeAttackChargeEffectComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RFX", meta = (DisplayName = "ChargeAttack"))
	class UNiagaraSystem* m_pChargeAttackEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RFX", meta = (DisplayName = "Combo1"))
	class UNiagaraSystem* m_pComboAttackEffect1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RFX", meta = (DisplayName = "Combo2"))
	class UNiagaraSystem* m_pComboAttackEffect2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RFX", meta = (DisplayName = "Combo3"))
	class UNiagaraSystem* m_pComboAttackEffect3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RFX", meta = (DisplayName = "SpecialAttack"))
	class UNiagaraSystem* m_pSpecialAttackEffect;
#pragma endregion

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items", Meta = (DisplayName = "HoldingLocations"))
		TArray<struct FIngredientAttach> m_ItemHoldLocations;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items", Meta = (DisplayName = "ItemHoldCapacity"))
		float m_ItemHoldCapacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DEBUG", Meta = (DisplayName = "DebugSpeedMultiplier"))
		int m_DebugSpeedMultiplier;

private:

#pragma endregion
};
