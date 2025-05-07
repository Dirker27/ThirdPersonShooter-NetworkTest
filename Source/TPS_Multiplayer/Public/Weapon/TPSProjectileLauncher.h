// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "Projectile/TPSProjectile.h"
#include "UObject/ObjectMacros.h"
#include "Weapon/TPSWeapon.h"

#include "TPSProjectileLauncher.generated.h"

USTRUCT(BlueprintType)
struct FProjectileLaunchInfo
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TArray<FVector> ProjectilePaths;
};



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

    // Projectile Definition
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
    TSubclassOf<ATPSProjectile> ProjectileTemplate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
    bool ShowDebugTrace = false;


//~ ============================================================= ~//
//  BEHAVIOR
//~ ============================================================= ~//
public:
    UFUNCTION(BlueprintImplementableEvent)
    void OnFire(const FProjectileLaunchInfo& launchInfo);


    UFUNCTION(BlueprintCallable)
    ATPSProjectile* LaunchProjectile(FRotator target);
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