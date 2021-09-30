#pragma once

#include "CoreMinimal.h"
#include "BasePlayer_C.h"
#include "ChefPlayer_C.generated.h"

/**
 * 
 */
UCLASS()
class MYTHIFOOD_API AChefPlayer_C : public ABasePlayer_C
{
	GENERATED_BODY()

#pragma region DEFAULT
public:
	AChefPlayer_C();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	void Use();

private:

	TArray<class ABaseEnemy_C*> m_pPullEnemies;
#pragma endregion

#pragma region UFUNC & UPROP
public:
	//UFunctions
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnLaddleOverlapBegin(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = "Combo", meta = (DisplayName = "SetAttackCollision"))
		void SetAttackCollision(bool enabled);

protected:
	UFUNCTION()
		void GotUsed(AActor* pCaller);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChildActor", meta = (DisplayName = "UsableComponent"))
		class UUsableComponent_C* m_pUsableComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collider", meta = (DisplayName = "UseCollider"))
		class UBoxComponent* m_pUseCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collider", meta = (DisplayName = "LadleCollider"))
		class UBoxComponent* m_pLadleCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo", meta = (DisplayName = "LadlePullAttachPoint"))
		class USceneComponent* m_pLadlePullAttachPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo", meta = (DisplayName = "SmashEffectAttachPoint"))
		class USceneComponent* m_pSmashEffectAttachPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun", meta = (DisplayName = "GrinderGun"))
		class UGrinderGun_C* m_pGrinderGun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ladle", meta = (DisplayName = "LadleMesh"))
		class UStaticMeshComponent* m_pLadleMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun", meta = (DisplayName = "CoopGunMesh"))
		class USkeletalMeshComponent* m_pGunMesh;

private:

#pragma endregion
};
