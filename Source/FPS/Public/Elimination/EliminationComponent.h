// Copyright Perfect Pixel Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EliminationComponent.generated.h"

enum class ESpecialElimType : uint16;
class AShooterPlayerState;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FPS_API UEliminationComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEliminationComponent();
	
	UFUNCTION()
	void OnRoundReported(AActor* Attacker, AActor* Victim, bool bHit, bool bHeadshot, bool bLethal);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS|Elimination")
	float SequentialElimInterval;
	
private:
	
	float LastElimTime;
	int32 SequentialElims;
	
	AShooterPlayerState* GetPlayerStateFromActor(AActor* Actor); 
	
	void ProcessHitOrMiss(bool bHit, AShooterPlayerState* AttackerPS);
	void ProcessElimination(bool bHeadshot, AShooterPlayerState* AttackerPS, AShooterPlayerState* VictimPS);
	void ProcessHeadshot(bool bHeadshot, ESpecialElimType& OutElimType, AShooterPlayerState* AttackerPS);
	void ProcessSequentialElim(ESpecialElimType OutElimType, AShooterPlayerState* AttackerPS);
};
