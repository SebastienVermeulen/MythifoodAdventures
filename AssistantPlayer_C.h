#pragma once

#include "CoreMinimal.h"
#include "BasePlayer_C.h"
#include "AssistantPlayer_C.generated.h"


UCLASS()
class MYTHIFOOD_API AAssistantPlayer_C : public ABasePlayer_C
{
	GENERATED_BODY()
	
#pragma region DEFAULT
public:
	AAssistantPlayer_C();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void UpdateRotation();
	void UpdateDash(float DeltaTime);

	void SetGrinderGun(class UPrimitiveComponent* pGrinderGun);

protected:
	virtual void BeginPlay() override;

	void Use();

private:
	class UPrimitiveComponent* m_pGrinderGun;
	FRotator m_KeptRotation;

	//Dash
	FTimerHandle m_DashTimerHandle;
	float m_DashChargeCounter;
	float m_DashLength;
	bool m_IsDashCharging;
	bool m_IsDashing;
#pragma endregion

#pragma region UFUNC & UPROP
public:

protected:
	UFUNCTION(BlueprintCallable, Category = "Dash", meta = (DisplayName = "DashCharge"))
	void DashCharge(const FKey key);
	UFUNCTION(BlueprintCallable, Category = "Dash", meta = (DisplayName = "Dash"))
	void Dash(const FKey key);
	UFUNCTION(BlueprintCallable, Category = "Dash", meta = (DisplayName = "EndDash"))
	void EndDash();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collider", meta = (DisplayName = "UseCollider"))
		class UBoxComponent* m_pUseCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash", meta = (DisplayName = "DashStrength"))
		float m_DashStrength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash", meta = (DisplayName = "DashChargeTime"))
		float m_DashChargeTime;

	UFUNCTION(BlueprintCallable, Category = "Items", meta = (DisplayName = "PickUp"))
		void PickUp();

private:

#pragma endregion
};
