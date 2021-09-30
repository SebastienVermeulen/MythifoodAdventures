#include "BasePlayer_C.h"
#include "GameFramework/CharacterMovementComponent.h"
//Niagara
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
//Gameplay
#include "HealthComponent_C.h"
#include "Engine/EngineTypes.h"
#include "Animation/AnimMontage.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "BaseEnemy_C.h"
#include "FoodDataTable_C.h"

ABasePlayer_C::ABasePlayer_C()
	:m_pHealthComponent{ nullptr }
	, m_LastMov{ 0.f, 0.f }
	, m_IsAttached{ false }
	, m_Attacking{false}
	, m_ChainAttack{ false }
	, m_ComboLocked{ false }
	, m_DebugMode{false}
	, m_CurrentMontage{}
	, m_StunTimerHandle{ FTimerHandle() }
	, m_MaxMovementSpeed{ 0.f }
	, m_ComboCounter{ 0 }
	, m_AttackSlow{ 0.7f }
	, m_ComboMontages{}
	, m_pWalkingEffect{ nullptr }
	, m_pWalkingEffectComp{ nullptr }
	, m_pStunEffect{ nullptr }
	, m_pStunEffectComp{ nullptr }
	, m_pSlowEffect{ nullptr }
	, m_pSlowEffectComp{ nullptr }
	, m_pChargeAttackChargeEffect{ nullptr }
	, m_pChargeAttackChargeEffectComp{ nullptr }
	, m_pChargeAttackEffect{ nullptr }
	, m_pComboAttackEffect1{ nullptr }
	, m_pComboAttackEffect2{ nullptr }
	, m_pComboAttackEffect3{ nullptr }
	, m_pSpecialAttackEffect{ nullptr }
	, m_ItemHoldCapacity{3.0f}
	, m_DebugSpeedMultiplier{30}
{
	PrimaryActorTick.bCanEverTick = true;

	//Making the health component and adding to hiarchy
	m_pHealthComponent = CreateDefaultSubobject<UHealthComponent_C>("HealthComponent");
	this->AddOwnedComponent(m_pHealthComponent);
	m_pHealthComponent->SetHealth(1);
	m_pHealthComponent->SetMaxHealth(1);

	Tags.Add("BasePlayer");
}
void ABasePlayer_C::BeginPlay()
{
	Super::BeginPlay();

	//Movement
	UCharacterMovementComponent* pMovComp = GetCharacterMovement();
	if (!pMovComp) return;

	m_MaxMovementSpeed = pMovComp->MaxWalkSpeed;
}
void ABasePlayer_C::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#pragma region Input
void ABasePlayer_C::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("Forward", this, &ABasePlayer_C::MoveForward);
	InputComponent->BindAxis("Right", this, &ABasePlayer_C::MoveRight);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ABasePlayer_C::Attack);
	PlayerInputComponent->BindAction("DebugToggleMode", IE_Pressed, this, &ABasePlayer_C::ToggleDebugMode);
}

void ABasePlayer_C::MoveForward(float axis)
{
	if (!m_IsAttached && (Controller != NULL) && (axis != 0.0f))
	{
		const FRotator rotation = FRotator{ 0.f, UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraRotation().Yaw, 0.f };

		AddMovementInput(UKismetMathLibrary::GetForwardVector(rotation), axis);
	}
	m_LastMov.Y = axis;
}
void ABasePlayer_C::MoveRight(float axis)
{
	if (!m_IsAttached && (Controller != NULL) && (axis != 0.0f))
	{
		const FRotator rotation = FRotator{ 0.f, UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraRotation().Yaw, 0.f };

		AddMovementInput(UKismetMathLibrary::GetRightVector(rotation), axis);
	}
	m_LastMov.X = axis;
}

#pragma endregion

void ABasePlayer_C::ToggleAttached()
{
	m_IsAttached = !m_IsAttached;
}
void ABasePlayer_C::PickUpIngredient(AActor* pActor)
{
	//Check if it's an enemy
	ABaseEnemy_C* pEnemy = Cast<ABaseEnemy_C>(pActor);
	if (!pEnemy)
		return;

	//Get the data
	FDataTableRowHandle handle = pEnemy->GetDataEntry();
	FString error;
	FFoodData* pData = handle.GetRow<FFoodData>(error);

	//Check if there is capacity
	for (int idx = 0; idx < (int)m_ItemHoldCapacity; idx++)
	{
		//Check if spot is filled
		if (m_ItemHoldLocations[idx].m_pEnemy != nullptr)
			continue;

		//Return if the weight would be too big
		switch (pData->m_FoodType) 
		{
		case eFoodType::spice:
			//Do nothing
			//Spices are not held in an inventory of their own
			return;
		case eFoodType::snack:
			//Weight of snack is 1.0f
			if (m_ItemHoldWeight + 1.0f > m_ItemHoldCapacity + FLT_EPSILON)
			{
				return;
			}
			//Passed check, add weight
			m_ItemHoldWeight += 1.0f;
			break;
		case eFoodType::stuffing:
			//Weight of stuffing is 1.5f
			if (m_ItemHoldWeight + 1.5f > m_ItemHoldCapacity + FLT_EPSILON)
			{
				return;
			}
			//Passed check, add weight
			m_ItemHoldWeight += 1.5f;
			break;
		case eFoodType::gourmet:
			//Weight of gourmet is ItemHoldCapacity
			if (m_ItemHoldWeight + m_ItemHoldCapacity > m_ItemHoldCapacity + FLT_EPSILON)
			{
				return;
			}
			//Passed check, add weight
			m_ItemHoldWeight += m_ItemHoldCapacity;
			break;
		}

		//Attach
		pEnemy->AttachToComponent(m_ItemHoldLocations[idx].m_pAttached, FAttachmentTransformRules(
			EAttachmentRule::KeepWorld,
			EAttachmentRule::KeepWorld,
			EAttachmentRule::KeepWorld, false));

		//Add data
		m_ItemHoldLocations[idx].m_pEnemy = pEnemy;
		m_ItemHoldLocations[idx].m_Data = *pData;
	}
}
void ABasePlayer_C::DropAllIngredients()
{
	//Go over all spots
	for (int idx = 0; idx < (int)m_ItemHoldCapacity; idx++)
	{
		ABaseEnemy_C* pEnemy = Cast<ABaseEnemy_C>(m_ItemHoldLocations[idx].m_pEnemy);
		//Check if spot is filled
		if (pEnemy != nullptr)
			return;;

		//Revive
		TSubclassOf<UHealthComponent_C> pHealthComp;
		pEnemy->GetComponentByClass(pHealthComp);
		Cast<UHealthComponent_C>(pHealthComp)->Revive();

		//Detach
		pEnemy->DetachFromActor(FDetachmentTransformRules(
			EDetachmentRule::KeepWorld, 
			EDetachmentRule::KeepWorld, 
			EDetachmentRule::KeepWorld, false));

		//Clear
		m_ItemHoldLocations[idx].m_pEnemy = nullptr;
	}

	//Set weight to 0
	m_ItemHoldWeight = 0.0f;
}

void ABasePlayer_C::RecieveDamage(const int damage)
{
	m_pHealthComponent->TakeDamage(damage);
}

void ABasePlayer_C::Stun(float stunDuration)
{
	UCharacterMovementComponent* pMovComp = GetCharacterMovement();
	if (!pMovComp) return;

	FMovementProperties movementProps;
	movementProps.bCanWalk = false;
	pMovComp->MovementState = movementProps;
	//Trigger endstun after "stunDuration" seconds
	GetWorld()->GetTimerManager().SetTimer(m_StunTimerHandle, this, &ABasePlayer_C::EndStun, stunDuration, false);
}
void ABasePlayer_C::EndStun() 
{
	UCharacterMovementComponent* pMovComp = GetCharacterMovement();
	if (!pMovComp) return;

	FMovementProperties movementProps;
	movementProps.bCanWalk = true;
	pMovComp->MovementState = movementProps;
}

void ABasePlayer_C::Slow(float slowPercent, bool reset)
{
	UCharacterMovementComponent* pMovComp = GetCharacterMovement();
	if (!pMovComp) return;

	if (reset)
	{
		pMovComp->MaxWalkSpeed = m_MaxMovementSpeed;
		return;
	}

	pMovComp->MaxWalkSpeed = m_MaxMovementSpeed * slowPercent;
}

FVector2D ABasePlayer_C::GetLastMov() const
{
	return m_LastMov;
}

void ABasePlayer_C::ResetCombo()
{
	m_ChainAttack = false;
	m_Attacking = false;
	m_ComboLocked = false;
	m_ComboCounter = 0;

	FString text = FString::FromInt(m_ComboCounter);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, text);
}
void ABasePlayer_C::Attack()
{
	if (GetCharacterMovement()->IsFalling())
		return;

	if (m_ComboLocked)
	{
		return;
	}

	if (m_Attacking)
	{
		m_ChainAttack = true;
		return;
	}

	m_Attacking = true;

	DoAttack();
}
void ABasePlayer_C::AttackSlow(float slowPercent, bool reset)
{
	UCharacterMovementComponent* pMovComp = GetCharacterMovement();
	if (!pMovComp) return;

	if (reset)
	{
		pMovComp->MaxWalkSpeed = m_MaxMovementSpeed;
		return;
	}

	pMovComp->MaxWalkSpeed = m_MaxMovementSpeed * slowPercent;
}
void ABasePlayer_C::ComboLock(bool locked)
{
	m_ComboLocked = locked;
}
void ABasePlayer_C::ToggleDebugMode()
{
	m_DebugMode = !m_DebugMode;

	UCharacterMovementComponent* pMovComp = GetCharacterMovement();
	if (!pMovComp) return;

	if(m_DebugMode)
	{
		pMovComp->MaxWalkSpeed = m_MaxMovementSpeed * m_DebugSpeedMultiplier;
	}
	else
	{
		pMovComp->MaxWalkSpeed = m_MaxMovementSpeed;
	}
}
void ABasePlayer_C::ChainAttack()
{
	if (GetCharacterMovement()->IsFalling())
		return;

	if (!m_ChainAttack)
	{
		return;
	}

	m_ChainAttack = false;

	DoAttack();
}
void ABasePlayer_C::DoAttack()
{
	if(!m_ComboMontages.Num() || !m_ComboMontages[m_ComboCounter])
	{
		return;
		Reset();
	}

	m_CurrentMontage = m_ComboMontages[m_ComboCounter];
	GetMesh()->GetAnimInstance()->Montage_Play(m_ComboMontages[m_ComboCounter]);
	m_ComboCounter++;
}
