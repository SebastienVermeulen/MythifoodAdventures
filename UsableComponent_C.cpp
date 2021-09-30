#include "UsableComponent_C.h"

UUsableComponent_C::UUsableComponent_C()
{	
	PrimaryComponentTick.bCanEverTick = true;

	ComponentTags.Add("Use");
}
void UUsableComponent_C::BeginPlay()
{
	Super::BeginPlay();
}
void UUsableComponent_C::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
