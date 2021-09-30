#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "GrinderGun_C.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYTHIFOOD_API UGrinderGun_C : public USceneComponent
{
	GENERATED_BODY()

#pragma region DEFAULT
public:	
	UGrinderGun_C();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void UpdateAttachedRotation();

	void SetGunMesh(class USkeletalMeshComponent* pGunMesh);

protected:
	virtual void BeginPlay() override;

	void AttachPlayer(ACharacter* pChar);
	void DetachPlayer();

private:
	class AAssistantPlayer_C* m_pAttachedChar;
	class USkeletalMeshComponent* m_pGunMesh;
	FRotator m_DefaultGunRotation;

	FRotator m_AttachedActorRot;
#pragma endregion

#pragma region UFUNC & UPROP
public:
	UFUNCTION(BlueprintCallable, Category = "GunActivation", meta = (DisplayName = "AttachDetachPlayer"))
		void AttachDetachPlayer(ACharacter* pChar);

protected:
	UFUNCTION(BlueprintCallable, Category = "GunActivation", meta = (DisplayName = "ActivateGun"))
		void ActivateGun();
	UFUNCTION(BlueprintCallable, Category = "GunActivation", meta = (DisplayName = "DeactivateGun"))
		void DeactivateGun();

	UFUNCTION(BlueprintCallable, Category = "GunActivation", meta = (DisplayName = "SwapAmmoType"))
		void SwapAmmoType();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gun", meta = (DisplayName = "Ammo"))
		TArray<int> m_Ammo;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gun", meta = (DisplayName = "SelectedAmmo"))
		int m_SelectedAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun", meta = (DisplayName = "AttachPoint"))
		class USceneComponent* m_pAssistantAttachPoint;

private:

#pragma endregion
};
