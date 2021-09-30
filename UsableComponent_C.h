#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "UsableComponent_C.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUse, AActor*, pCaller);

UCLASS()
class MYTHIFOOD_API UUsableComponent_C : public UBoxComponent
{
	GENERATED_BODY()

#pragma region DEFAULT
public:	
	UUsableComponent_C();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:

#pragma endregion

#pragma region UFUNC & UPROP
public:
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Collider", meta = (DisplayName = "UseDelagate"))
		FOnUse m_Use;

protected:

private:

#pragma endregion
};
