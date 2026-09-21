// Copyright Perfect Pixel Games

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "ShooterTypes/ShooterTypes.h"
#include "Weapon.generated.h"

class UMaterialInstanceDynamic;
enum EPhysicalSurface : int;

UENUM(BlueprintType)
enum class EFireType : uint8
{
	Auto UMETA(DisplayName = "Automatic"),
	SemiAuto UMETA(DisplayName = "Semi-Automatic")
};

UENUM(BlueprintType)
enum class EWeaponStatus : uint8
{
	Idle, // Weapon doing nothing, can fire/reload/cycle
	Firing, // Weapon is firing, can't reload/cycle
	Reloading, // Currently reloading, can't fire or cycle
	Cycling, // Currently cycling, can't fire or reload
	Unequipped // Weapon is on our person but can't fire or reload
};

UCLASS()
class FPS_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	AWeapon();
	
	USkeletalMeshComponent* GetMesh1P() const;
	USkeletalMeshComponent* GetMesh3P() const;
	UMaterialInstanceDynamic* GetReticleDynamicMaterialInstance();
	UMaterialInstanceDynamic* GetAmmoCounterDynamicMaterialInstance();
	
	void AttachToOwningPawn(APawn* Pawn) const;
	void WeaponTrace(FHitResult& OutHit, float TraceLength);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FPS|WeaponType")
	FGameplayTag WeaponType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS|Aiming")
	float AimFieldOfView;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS|Aiming")
	float TraceRadius;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FPS|FireType")
	EFireType FireType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FPS|FireType")
	float FireTime;
	
	UPROPERTY(EditDefaultsOnly, Category = "FPS|Reticle")
	FReticleParams ReticleParams;
	
	UPROPERTY(EditDefaultsOnly, Category = "FPS|Icon")
	TObjectPtr<UMaterialInterface> WeaponIcon;
	
	void Local_Fire(const FVector& ImpactPoint, const FVector& ImpactNormal, TEnumAsByte<EPhysicalSurface> ImpactSurfaceType, bool bIsFirstPerson);
	void Auth_Fire();
	void Rep_Fire(int32 AuthAmmo);
	
	UPROPERTY(EditAnywhere, Category = "FPS|Ammo")
	int32 MagCapacity;
	
	UPROPERTY(EditAnywhere, Category = "FPS|Ammo")
	int32 Ammo;
	
	UPROPERTY(EditAnywhere, Category = "FPS|Ammo")
	int32 StartingCarriedAmmo;
	
	EWeaponStatus WeaponStatus;

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void FireEffects(const FVector& ImpactPoint, const FVector& ImpactNormal, EPhysicalSurface ImpactSurfaceType, bool bIsFirstPerson);

	//Weapon Mesh 1P View
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FPS|Weapon")
	TObjectPtr<USkeletalMeshComponent> Mesh1P;
	
	//Weapon Mesh 3P View
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FPS|Weapon")
	TObjectPtr<USkeletalMeshComponent> Mesh3P;
	
private:
	
	void SetMeshVisibilities(const APawn* OwningPawn) const;
	
	int32 Sequence;
	
	UPROPERTY(EditDefaultsOnly, Category = "FPS|Weapon")
	TObjectPtr<UMaterialInterface> ReticleMaterial;
	
	UPROPERTY(EditDefaultsOnly, Category = "FPS|Weapon")
	TObjectPtr<UMaterialInterface> AmmoCounterMaterial;
	
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynMatInst_Reticle;
	
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynMatInst_AmmoCounter;
};
