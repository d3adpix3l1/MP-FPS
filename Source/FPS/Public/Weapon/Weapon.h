// Copyright Perfect Pixel Games

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class FPS_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	AWeapon();
	virtual void OnRep_Instigator() override;
	
	USkeletalMeshComponent* GetMesh1P() const;
	USkeletalMeshComponent* GetMesh3P() const;
	
	void AttachToOwningPawn() const;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = "FPS|WeaponType")
	FGameplayTag WeaponType;

private:
	
	//Weapon Mesh 1P View
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> Mesh1P;
	
	//Weapon Mesh 3P View
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> Mesh3P;
	
	void SetMeshVisibilities(APawn* OwningPawn) const;
};
