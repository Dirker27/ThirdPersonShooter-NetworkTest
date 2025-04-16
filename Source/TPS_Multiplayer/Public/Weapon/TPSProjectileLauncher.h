// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "Projectile/TPSProjectile.h"
#include "UObject/ObjectMacros.h"
#include "Weapon/TPSWeapon.h"

#include "TPSProjectileLauncher.generated.h"

UCLASS()
class TPS_MULTIPLAYER_API ATPSProjectileLauncher : public ATPSWeapon
{
    GENERATED_BODY()

public:
    ATPSProjectileLauncher();

public:
    ////////////////////////////////////////////////////////
    // Configuration

    // Muzzle / Projectile Launch Point
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
    TObjectPtr<UTPSMountPoint> Muzzle;
    
    // Shell Ejection Location / Direction
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
    TObjectPtr<UTPSMountPoint> ShellEjectPort;

    // Fired Projectile
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
    TSubclassOf<ATPSProjectile> ProjectileTemplate;

//~ ============================================================= ~//
//  BEHAVIOR
//~ ============================================================= ~//
public:
    UFUNCTION(BlueprintCallable)
    void LaunchProjectile(FRotator target);
    virtual void PerformFire() override; // Wired to ^

    //- Usable ------------------------------------------=
    //
    virtual void StartUse() override;
    //
    virtual void StopUse() override;

private:
    FVector2D CalculateAccuracyNoise() const;
    FVector2D CalculateSpreadNoise() const;
};