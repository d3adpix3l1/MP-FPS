// Copyright Perfect Pixel Games


#include "Elimination/EliminationComponent.h"
#include "GameFramework/Pawn.h"
#include "Player/ShooterPlayerState.h"
#include "ShooterTypes/ShooterTypes.h"
#include "Engine/World.h"
#include "Game/ShooterGameStateBase.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
UEliminationComponent::UEliminationComponent()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = false;
	SequentialElimInterval = 2.f;
	LastElimTime = 0.f;
	SequentialElims = 0;
	Streak = 0;
	ElimsNeededForStreak = 5;
}

void UEliminationComponent::OnRoundReported(AActor* Attacker, AActor* Victim, bool bHit, bool bHeadshot, bool bLethal)
{
	AShooterPlayerState* AttackerPS = GetPlayerStateFromActor(Attacker);
	if (!IsValid(AttackerPS)) return;
	
	ProcessHitOrMiss(bHit, AttackerPS);
	
	if (!bHit) return; // Early return if we missed
	
	AShooterPlayerState* VictimPS = GetPlayerStateFromActor(Victim);
	if (!IsValid(VictimPS)) return;
	
	if (bLethal)
	{
		ProcessElimination(bHeadshot, AttackerPS, VictimPS);
	}
}

void UEliminationComponent::ProcessElimination(bool bHeadShot, AShooterPlayerState* AttackerPS, AShooterPlayerState* VictimPS)
{
	AttackerPS->AddScoredElim();
	VictimPS->AddDefeat();
	
	ESpecialElimType SpecialElimType{};
	
	ProcessHeadshot(bHeadShot, SpecialElimType, AttackerPS);
	ProcessSequentialElim(SpecialElimType, AttackerPS);
	ProcessStreaks(SpecialElimType, AttackerPS, VictimPS);
	
	AShooterGameStateBase* GameState = Cast<AShooterGameStateBase>(UGameplayStatics::GetGameState(AttackerPS));
	if (IsValid(GameState))
	{
		HandleFirstBlood(GameState, SpecialElimType, AttackerPS);
		UpdateLeaderStatus(GameState, SpecialElimType, AttackerPS, VictimPS);
	}
	
	if (HasSpecialElimTypes(SpecialElimType))
	{
		AttackerPS->Client_SpecialElim(SpecialElimType, SequentialElims, Streak, AttackerPS->GetScoredElims());
	}
	else
	{
		AttackerPS->Client_ScoredElim(AttackerPS->GetScoredElims());
	}
}

void UEliminationComponent::ProcessHeadshot(bool bHeadshot, ESpecialElimType& OutElimType, AShooterPlayerState* AttackerPS)
{
	if (bHeadshot)
	{
		OutElimType |= ESpecialElimType::Headshot;
		AttackerPS->AddHeadShotElim();
	}
}

void UEliminationComponent::ProcessSequentialElim(ESpecialElimType OutElimType, AShooterPlayerState* AttackerPS)
{
	const float CurrentTime = GetWorld()->GetTimeSeconds();
	
	if (CurrentTime - LastElimTime <= SequentialElimInterval)
	{
		++SequentialElims;
	}
	else
	{
		SequentialElims = 1;
	}
	
	LastElimTime = CurrentTime;
	
	if (SequentialElims > 1)
	{
		OutElimType |= ESpecialElimType::Sequential;
		AttackerPS->AddSequentialElim(SequentialElims);
	}
}

void UEliminationComponent::ProcessStreaks(ESpecialElimType OutElimType, AShooterPlayerState* AttackerPS, AShooterPlayerState* VictimPS)
{
	++Streak;
	if (Streak == ElimsNeededForStreak)
	{
		OutElimType |= ESpecialElimType::Streak;
		AttackerPS->SetOnStreak(true);
		AttackerPS->UpdateHighestStreak(Streak);
	}
	
	if (VictimPS->IsOnStreak())
	{
		OutElimType |= ESpecialElimType::ShowStopper;
		AttackerPS->AddShowStopperElim();
		VictimPS->SetOnStreak(false);
	}
	
	if (AttackerPS->GetLastAttacker() == VictimPS)
	{
		OutElimType |= ESpecialElimType::Revenge;
		AttackerPS->AddRevengeElim();
		AttackerPS->SetLastAttacker(nullptr);
	}
	VictimPS->SetLastAttacker(AttackerPS);
}

void UEliminationComponent::HandleFirstBlood(AShooterGameStateBase* GameState, ESpecialElimType& OutElimType,
	AShooterPlayerState* AttackerPS)
{
	if (!GameState->HasFirstBloodBeenHad())
	{
		OutElimType |= ESpecialElimType::FirstBlood;
		AttackerPS->GotFirstBlood();
	}
}

void UEliminationComponent::UpdateLeaderStatus(AShooterGameStateBase* GameState, ESpecialElimType& OutElimType, AShooterPlayerState* AttackerPS,
	AShooterPlayerState* VictimPS)
{
	AShooterPlayerState* LastLeader = GameState->GetSoleLeader();
	const bool bAttackerWasTiedForTheLead = GameState->IsTiedForTheLead(AttackerPS);
	GameState->UpdateLeader();
	if (!bAttackerWasTiedForTheLead && GameState->IsTiedForTheLead(AttackerPS))
	{
		// Attacker was not tied for the lead before and is now tied for the lead
		OutElimType |= ESpecialElimType::TiedTheLeader;
	}
	if (IsValid(LastLeader) && LastLeader != GameState->GetSoleLeader())
	{
		// Last Leader has lost the lead
		LastLeader->Client_LostTheLead();
		
		if (VictimPS == LastLeader)
		{
			OutElimType |= ESpecialElimType::Dethrone;
			AttackerPS->AddDethroneElim();
		}
	}
	
	if (AttackerPS!= LastLeader && AttackerPS == GameState->GetSoleLeader())
	{
		OutElimType |= ESpecialElimType::GainedTheLead;
	}
}

bool UEliminationComponent::HasSpecialElimTypes(const ESpecialElimType& SpecialElimType) const
{
	return static_cast<uint16>(SpecialElimType) != 0;
}

void UEliminationComponent::ProcessHitOrMiss(bool bHit, AShooterPlayerState* AttackerPS)
{
	if (bHit)
	{
		AttackerPS->AddHit();
	} else
	{
		AttackerPS->AddMiss();
	}
}

AShooterPlayerState* UEliminationComponent::GetPlayerStateFromActor(AActor* Actor)
{
	APawn* Pawn = Cast<APawn>(Actor);
	if (IsValid(Pawn))
	{
		return Pawn->GetPlayerState<AShooterPlayerState>();
	}
	
	return nullptr;
}


