#include "AssistantPlayer_C.h"
#include "Components/BoxComponent.h"
#include "UsableComponent_C.h"
#include "GrinderGun_C.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
//#include "NavigationSystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerInput.h"

AAssistantPlayer_C::AAssistantPlayer_C()
	:m_pGrinderGun{nullptr}
	, m_KeptRotation{}
	, m_DashTimerHandle{}
	, m_DashChargeCounter{0.0f}
	, m_DashLength{0.3f}
	, m_IsDashing{false}
	, m_pUseCollider{nullptr}
	, m_DashStrength{50.0f}
	, m_DashChargeTime{2.0f}
{
	PrimaryActorTick.bCanEverTick = true;

	//Create collider and attach to self
	m_pUseCollider = CreateDefaultSubobject<UBoxComponent>("InteractCollider");
	m_pUseCollider->SetupAttachment(RootComponent);

	//Create 3 locations for the items to be attached to
	USceneComponent* AttachParent;
	AttachParent = CreateDefaultSubobject<USceneComponent>("AttachParent");
	AttachParent->SetupAttachment(RootComponent);
	//Reserve items
	m_ItemHoldLocations.Reserve((int)m_ItemHoldCapacity);
	USceneComponent* temp;
	FIngredientAttach pickUp;
	//Add ItemHoldCapacity amount of items
	for (int i = 0; i < (int)m_ItemHoldCapacity; i++)
	{
		FName name = FName("AttachLocation" + FString::FromInt(i));
		temp = CreateDefaultSubobject<USceneComponent>(name);
		temp->SetupAttachment(AttachParent);
		pickUp.m_pAttached = temp;
		m_ItemHoldLocations.Add(pickUp);
	}

	Tags.Add("AssistantPlayer");
}
void AAssistantPlayer_C::BeginPlay()
{
	Super::BeginPlay();
}
void AAssistantPlayer_C::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateRotation();
	if(m_IsDashCharging)
		UpdateDash(DeltaTime);
}

void AAssistantPlayer_C::UpdateRotation()
{
	//If the other player isn't attached return
	if (m_IsAttached == false) return;

	if (abs(m_LastMov.X) < 0.3f && abs(m_LastMov.Y) < 0.3f)
	{
		m_pGrinderGun->SetWorldRotation(m_KeptRotation);
		return;
	}

	float angle{ UKismetMathLibrary::Atan2(-m_LastMov.Y, m_LastMov.X) + UKismetMathLibrary::GetPI() / 2.0f };

	//Use the given movement to create a rotation
	FQuat worldRot = FQuat::MakeFromEuler(FVector(90, 0, 180 * angle / PI + 90));
	//Set the rotation to this for the gun
	m_pGrinderGun->SetWorldRotation(worldRot);

	m_KeptRotation = m_pGrinderGun->GetComponentRotation();
}
void AAssistantPlayer_C::UpdateDash(float DeltaTime)
{
	m_DashChargeCounter += DeltaTime;
}

void AAssistantPlayer_C::SetGrinderGun(class UPrimitiveComponent* pGrinderGun)
{
	m_pGrinderGun = pGrinderGun;
	m_KeptRotation = m_pGrinderGun->GetComponentRotation();
}

#pragma region Input
void AAssistantPlayer_C::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAction("Use", EInputEvent::IE_Pressed, this, &AAssistantPlayer_C::Use);
	InputComponent->BindAction("SpecialAttack", EInputEvent::IE_Pressed, this, &AAssistantPlayer_C::DashCharge);
	InputComponent->BindAction("SpecialAttack", EInputEvent::IE_Released, this, &AAssistantPlayer_C::Dash);
	InputComponent->BindAction("PickUp", EInputEvent::IE_Released, this, &AAssistantPlayer_C::PickUp);
}

void AAssistantPlayer_C::Use()
{
	if (!m_pUseCollider) return;

	TArray<UPrimitiveComponent*> overlappingComponents;
	m_pUseCollider->GetOverlappingComponents(overlappingComponents);

	for (UPrimitiveComponent* pComp : overlappingComponents)
	{
		if (!pComp->ComponentHasTag("Use"))
			continue;

		UUsableComponent_C* pUse = Cast<UUsableComponent_C>(pComp);
		if (pUse)
		{
			pUse->m_Use.Broadcast(this);
		}
	}
}

void AAssistantPlayer_C::DashCharge(const FKey key)
{
	m_IsDashCharging = true;
}
void AAssistantPlayer_C::Dash(const FKey key)
{
	//Charge logic
	m_IsDashCharging = false;
	if (m_DashChargeCounter < m_DashChargeTime)
	{
		m_DashChargeCounter = 0.0f;
		return;
	}
	m_DashChargeCounter = 0.0f;

	//Prevent double dash
	if (m_IsDashing)
		return;

	//Launch character
	const FVector forward = GetActorForwardVector();
	LaunchCharacter(forward * m_DashStrength, true, false);
	m_IsDashing = true;
	//Set timer to end the dash
	GetWorld()->GetTimerManager().SetTimer(m_DashTimerHandle, this, &AAssistantPlayer_C::EndDash, m_DashLength);
}
void AAssistantPlayer_C::EndDash() 
{
	m_DashTimerHandle.Invalidate();
	GetCharacterMovement()->StopMovementImmediately();
	m_IsDashing = false;
}

void AAssistantPlayer_C::PickUp()
{
	if (!m_pUseCollider) return;

	TArray<UPrimitiveComponent*> overlappingComponents;
	m_pUseCollider->GetOverlappingComponents(overlappingComponents);

	for (UPrimitiveComponent* pComp : overlappingComponents)
	{
		if (!pComp->ComponentHasTag("PickUp"))
			continue;

		UUsableComponent_C* pUse = Cast<UUsableComponent_C>(pComp);
		if (pUse)
		{
			pUse->m_Use.Broadcast(this);
		}
	}
}
#pragma endregion
