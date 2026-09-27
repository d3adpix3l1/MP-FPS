// Copyright Perfect Pixel Games


#include "Elimination/EliminationComponent.h"


// Sets default values
UEliminationComponent::UEliminationComponent()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = false;
	
}

void UEliminationComponent::OnRoundReported(AActor* Attacker, AActor* Victim, bool bHit, bool bHeadshot, bool bLethal)
{
	
}


