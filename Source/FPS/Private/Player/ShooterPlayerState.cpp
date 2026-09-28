// Copyright Perfect Pixel Games


#include "Player/ShooterPlayerState.h"

AShooterPlayerState::AShooterPlayerState()
{
	SetNetUpdateFrequency(100.f);
	
	ScoredElims = 0.f;
	Defeats = 0.f;
	Hits = 0.f;
	Misses = 0.f;
	bOnStreak = false;
	HeadShotElims = 0.f;
	HighestStreak = 0.f;
	RevengeElims = 0.f;
	DethroneElims = 0.f;
	ShowStopperElims = 0.f;
	bFirstBlood = false;
	bWinner = false;
	
}

void AShooterPlayerState::AddScoredElim()
{
	++ScoredElims;
}

void AShooterPlayerState::AddDefeat()
{
	++Defeats;
}

void AShooterPlayerState::AddHit()
{
	++Hits;
}

void AShooterPlayerState::AddMiss()
{
	++Misses;
}

void AShooterPlayerState::AddHeadShotElim()
{
	++HeadShotElims;
}

void AShooterPlayerState::AddSequentialElim(int32 SequenceCount)
{
	if (SequentialElims.Contains(SequenceCount))
	{
		SequentialElims[SequenceCount]++;
	}
	else
	{
		SequentialElims.Add(SequenceCount, 1);
	}
	/* Reduce the count for all lower sequence counts
	 * this is because a triple elim means a double was scored first.
	 * but we want tho count this as just a triple
	 * i.e. elim 1, elim 2, elim 3, is a triple elim, not a double and a triple elim
	 */
	for (auto& Elem : SequentialElims)
	{
		if (Elem.Key < SequenceCount && Elem.Value > 0)
		{
			++Elem.Value;
		}
	}
}

void AShooterPlayerState::UpdateHighestStreak(int32 StreakCount)
{
	if (StreakCount > HighestStreak)
	{
		HighestStreak = StreakCount;
	}
}

void AShooterPlayerState::AddRevengeElim()
{
	++RevengeElims;
}

void AShooterPlayerState::AddDethroneElim()
{
	++DethroneElims;
}

void AShooterPlayerState::AddShowStopperElim()
{
	++ShowStopperElims;
}

void AShooterPlayerState::GotFirstBlood()
{
	bFirstBlood = true;
}

void AShooterPlayerState::IsNowWinner()
{
	bWinner = true;
}

void AShooterPlayerState::SetOnStreak(bool bIsOnStreak)
{
	bOnStreak = bIsOnStreak;
}

void AShooterPlayerState::SetLastAttacker(APlayerState* Attacker)
{
	LastAttacker = Attacker;
}

bool AShooterPlayerState::IsOnStreak() const
{
	return bOnStreak;
}

APlayerState* AShooterPlayerState::GetLastAttacker() const
{
	return LastAttacker.IsValid() ? LastAttacker.Get() : nullptr;
}

int32 AShooterPlayerState::GetScoredElims() const
{
	return ScoredElims;
}

void AShooterPlayerState::Client_ScoredElim_Implementation(int32 ElimScore)
{
	//
}

void AShooterPlayerState::Client_SpecialElim_Implementation(const ESpecialElimType& SpecialElim, int32 SequentialElimCount, int32 StreakCount, int32 ElimScore)
{
	//
}

void AShooterPlayerState::Client_LostTheLead_Implementation()
{
	//TODO: show to client with widgets in the hud
}
