#include "HealthComponent_C.h"

UHealthComponent_C::UHealthComponent_C()
	:m_Health{0}
	, m_MaxHealth{0}
{
	PrimaryComponentTick.bCanEverTick = true;
}
void UHealthComponent_C::BeginPlay()
{
	Super::BeginPlay();
}
void UHealthComponent_C::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UHealthComponent_C::TakeDamage(int damage)
{
	m_Health -= damage;
	m_Health = std::max(m_Health, 0);
	return IsDead();
}
void UHealthComponent_C::Revive()
{
	m_Health = m_MaxHealth;
}
bool UHealthComponent_C::IsDead()
{
	return m_Health == 0;
}

int UHealthComponent_C::GetHealth()
{
	return m_Health;
}
void UHealthComponent_C::SetHealth(int newHealth)
{
	m_Health = newHealth;
}
void UHealthComponent_C::SetMaxHealth(int newMaxHealth)
{
	m_MaxHealth = newMaxHealth;
}
