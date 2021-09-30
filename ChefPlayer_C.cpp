#include "ChefPlayer_C.h"
#include "GrinderGun_C.h"
#include "UsableComponent_C.h"
#include "Components/ChildActorComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "BaseEnemy_C.h"
//Niagara
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

AChefPlayer_C::AChefPlayer_C()
	:m_pGrinderGun{ nullptr }
	, m_pUsableComponent{ nullptr }
	, m_pUseCollider{ nullptr }
	, m_pGunMesh{ nullptr }
{
	PrimaryActorTick.bCanEverTick = true;

	//Gun
	m_pGrinderGun = CreateDefaultSubobject<UGrinderGun_C>("GrinderGun");
	m_pGrinderGun->SetupAttachment(GetMesh(), "gun_jntSocket");

	//Create child use component Use
	m_pUsableComponent = CreateDefaultSubobject<UUsableComponent_C>("ChildActor");
	m_pUsableComponent->SetupAttachment(RootComponent);
	m_pUsableComponent->m_Use.AddDynamic(this, &AChefPlayer_C::GotUsed);

	//Create collider and attach to self
	m_pUseCollider = CreateDefaultSubobject<UBoxComponent>("InteractCollider");
	m_pUseCollider->SetupAttachment(RootComponent);

	m_pLadleMesh = CreateDefaultSubobject<UStaticMeshComponent>("LadleMesh");
	m_pLadleMesh->SetSimulatePhysics(false);
	m_pLadleMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_pLadleMesh->SetupAttachment(GetMesh(), "ladle_jntSocket");

	//Collider that tracks the ladle
	m_pLadleCollider = CreateDefaultSubobject<UBoxComponent>("LadleCollider");
	m_pLadleCollider->SetupAttachment(m_pLadleMesh);
	m_pLadleCollider->SetGenerateOverlapEvents(false);

	m_pLadlePullAttachPoint = CreateDefaultSubobject<USceneComponent>("LadlePullAttachPoint");
	m_pLadlePullAttachPoint->SetupAttachment(m_pLadleMesh);
	m_pLadlePullAttachPoint->SetUsingAbsoluteRotation(true);

	m_pSmashEffectAttachPoint = CreateDefaultSubobject<USceneComponent>("SmashEffectAttachPoint");
	m_pSmashEffectAttachPoint->SetupAttachment(RootComponent);
	m_pSmashEffectAttachPoint->SetUsingAbsoluteRotation(true);

	//The skeletal mesh of the gun
	m_pGunMesh = CreateDefaultSubobject<USkeletalMeshComponent>("CoopGunMesh");
	FSkeletalMeshObject* pMesh = m_pGunMesh->MeshObject;
	m_pGunMesh->SetSimulatePhysics(false);
	m_pGunMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_pGunMesh->SetupAttachment(m_pGrinderGun);
	m_pGrinderGun->SetGunMesh(m_pGunMesh);

	Tags.Add("ChefPlayer");
}
void AChefPlayer_C::BeginPlay()
{
	Super::BeginPlay();

	m_pLadleCollider->OnComponentBeginOverlap.AddDynamic(this, &AChefPlayer_C::OnLaddleOverlapBegin);
}
void AChefPlayer_C::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#pragma region Input
void AChefPlayer_C::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAction("Use", EInputEvent::IE_Pressed, this, &AChefPlayer_C::Use);
}

void AChefPlayer_C::Use()
{
	if (!m_pUseCollider) return;

	TArray<UPrimitiveComponent*> overlappingComponents;
	m_pUseCollider->GetOverlappingComponents(overlappingComponents);

	for (UPrimitiveComponent* pComp : overlappingComponents)
	{
		if (pComp->ComponentHasTag("Use"))
		{
			UUsableComponent_C* pUse = Cast<UUsableComponent_C>(pComp);
			if (pUse)
			{
				pUse->m_Use.Broadcast(this);
			}
		}
	}
}
#pragma endregion

void AChefPlayer_C::OnOverlapBegin(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor == this)
	{
		return;
	}

	GetMesh()->GetAnimInstance()->Montage_Stop(0.1f, m_CurrentMontage);
	ResetCombo();
}

void AChefPlayer_C::OnLaddleOverlapBegin(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this)
	{
		return;
	}

	if (OtherActor->ActorHasTag("BaseEnemy"))
	{
		switch (m_ComboCounter)
		{
			case 1:
				Cast<ABaseEnemy_C>(OtherActor)->ReceiveDamage(1, this);
				break;

			case 2:
				Cast<ABaseEnemy_C>(OtherActor)->ReceiveDamage(1, this);
				Cast<ABaseEnemy_C>(OtherActor)->Pull(true);
				m_pPullEnemies.Add(Cast<ABaseEnemy_C>(OtherActor));
				OtherActor->AttachToComponent(m_pLadlePullAttachPoint, FAttachmentTransformRules{EAttachmentRule::KeepWorld, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, false});
				break;

			case 3:
				Cast<ABaseEnemy_C>(OtherActor)->ReceiveDamage(1, this);
				break;
		}
	}

}

void AChefPlayer_C::SetAttackCollision(bool enabled)
{
	m_pLadleCollider->SetGenerateOverlapEvents(enabled);
	FVector loc{};
	TArray<USceneComponent*> children{};


	switch (m_ComboCounter)
	{
	case 1:
		break;
	case 2:
		if(enabled)
		{
			return;
		}

		children = m_pLadlePullAttachPoint->GetAttachChildren();
		for(USceneComponent* comp : children)
		{
			comp->DetachFromComponent(FDetachmentTransformRules{EDetachmentRule::KeepWorld, EDetachmentRule::KeepRelative, EDetachmentRule::KeepRelative, false});
		}

		for(ABaseEnemy_C* enemy : m_pPullEnemies)
		{
			enemy->Pull(false);
		}

		m_pPullEnemies.Empty();

		break;
	case 3:
		if(m_pComboAttackEffect3)
		{
			loc = FVector{GetActorLocation() + (m_pComboAttackEffect3->GetFixedBounds().GetExtent() * GetActorForwardVector())};
			UNiagaraFunctionLibrary::SpawnSystemAttached(m_pComboAttackEffect3,m_pSmashEffectAttachPoint,"", FVector{FVector::ZeroVector}, FRotator{}, EAttachLocation::SnapToTarget, true);
		}
		break;
	}
}

void AChefPlayer_C::GotUsed(AActor* pCaller)
{
	if (pCaller == this) return;

	m_pGrinderGun->AttachDetachPlayer(Cast<ACharacter>(pCaller));
}
