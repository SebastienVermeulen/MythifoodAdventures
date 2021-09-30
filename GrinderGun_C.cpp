#include "GrinderGun_C.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AssistantPlayer_C.h"
#include "FoodDataTable_C.h"
#include "ChefPlayer_C.h"

UGrinderGun_C::UGrinderGun_C()
	:m_Ammo{ TArray<int>() }
	, m_SelectedAmmo{ 0 }
	, m_pGunMesh{ nullptr }
	, m_pAssistantAttachPoint{ nullptr }
	, m_pAttachedChar{ nullptr }
	, m_DefaultGunRotation{}
	, m_AttachedActorRot{}
{
	PrimaryComponentTick.bCanEverTick = true;

	m_pAssistantAttachPoint = CreateDefaultSubobject<USceneComponent>("AssistantAttachPoint");
	m_pAssistantAttachPoint->SetupAttachment(this);

	//Set size to the max ammo size (0 does not count so this is perfect size)
	m_Ammo.SetNum((uint32)eAmmoType::freezemortar);
}
void UGrinderGun_C::BeginPlay()
{
	Super::BeginPlay();

	m_DefaultGunRotation = m_pGunMesh->GetRelativeRotation();
}
void UGrinderGun_C::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateAttachedRotation();
}

void UGrinderGun_C::UpdateAttachedRotation() 
{
	if (!m_pAttachedChar)
		return;
	
	m_pAttachedChar->SetActorRelativeRotation(FRotator{180,180,-90});
}

void UGrinderGun_C::SetGunMesh(class USkeletalMeshComponent* pGunMesh)
{
	m_pGunMesh = pGunMesh;
}

void UGrinderGun_C::ActivateGun()
{

}
void UGrinderGun_C::DeactivateGun()
{
	m_pGunMesh->SetRelativeRotation(m_DefaultGunRotation);
}

void UGrinderGun_C::AttachDetachPlayer(ACharacter* pChar)
{
	//If player is attached detach
	if (m_pAttachedChar)
	{
		DetachPlayer();
		DeactivateGun();
	}
	else 
	{
		AttachPlayer(pChar);
		ActivateGun();

		m_pAttachedChar->SetGrinderGun(m_pGunMesh);
	}
}
void UGrinderGun_C::AttachPlayer(ACharacter* pChar)
{
	m_pAttachedChar = Cast<AAssistantPlayer_C>(pChar);

	//Set the character as attached
	m_pAttachedChar->ToggleAttached();

	m_AttachedActorRot = m_pAttachedChar->GetActorRotation();

	//Attach to point
	FAttachmentTransformRules rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false);
	m_pAttachedChar->AttachToComponent(m_pAssistantAttachPoint, rules);

	//Disable movement
	UActorComponent* comp = m_pAttachedChar->GetComponentByClass(UMovementComponent::StaticClass());
	UMovementComponent* movComp = Cast<UMovementComponent>(comp);
	movComp->Deactivate();
}
void UGrinderGun_C::DetachPlayer()
{
	//Set the character as attached
	m_pAttachedChar->ToggleAttached();

	//Set the character as detached
	FDetachmentTransformRules rules = FDetachmentTransformRules(EDetachmentRule::KeepRelative, EDetachmentRule::KeepRelative, EDetachmentRule::KeepRelative, false);
	m_pAttachedChar->DetachFromActor(rules);
	m_pAttachedChar->SetActorLocation(GetAttachmentRootActor()->GetActorLocation());

	//Enable movement
	UActorComponent* comp = m_pAttachedChar->GetComponentByClass(UMovementComponent::StaticClass());
	UMovementComponent* movComp = Cast<UMovementComponent>(comp);
	movComp->Activate();

	m_pAttachedChar->SetActorRotation(m_AttachedActorRot);

	m_pAttachedChar = nullptr;
}

void UGrinderGun_C::SwapAmmoType()
{
	m_SelectedAmmo++;

	if (m_Ammo.Num() == m_SelectedAmmo)
		m_SelectedAmmo = 0;
}
