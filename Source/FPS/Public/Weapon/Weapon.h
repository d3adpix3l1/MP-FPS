// Copyright Perfect Pixel Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class FPS_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	AWeapon();
	
	USkeletalMeshComponent* GetMesh1P() const;
	USkeletalMeshComponent* GetMesh3P() const;

protected:
	virtual void BeginPlay() override;

private:
	
	//Weapon Mesh 1P View
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> Mesh1P;
	
	//Weapon Mesh 3P View
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> Mesh3P;
};
