// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "Ammunition/TPSAmmunitionType.h"

#include "TPSWeaponConfiguration.generated.h"

UENUM(BlueprintType)
enum ETPSWeaponFireMode : int
{
    SingleShot = 0,
    FullAuto = 1,
    Burst = 2,
    Continuous = 3
};

UENUM(BlueprintType)
enum ETPSWeaponSpreadMode : int
{
    Degrees = 0,
    MOA = 1
};

UENUM(BlueprintType)
enum ETPSProjectileBehavior : int
{
    Slug = 0,
    Spread = 1
};


/**
 * The active configuration for a given Weapon.
 *
 * Unlike "Definition" classes used for Equipment, this is considered MUTABLE
 *   and can change over the course of a Weapon's lifetime.
 *
 *   (change fire modes, magazine types, etc)
 */
UCLASS()
class TPS_MULTIPLAYER_API UTPSWeaponConfiguration : public UDataAsset
{
    GENERATED_BODY()

public:
    // Raw Damage to deal on projectile hit (ignores physics)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
    float Damage = 1;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fire Control")
    TEnumAsByte<ETPSWeaponFireMode> FireMode = SingleShot;

    // How many rounds in a 'Burst'
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fire Control", meta = (EditCondition = "FireMode == ETPSWeaponFireMode::Burst", EditConditionHides))
    int BurstFireCount = 3;

    // Rounds / second
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fire Control")
    float CycleRate = 3.0f;

    // Do we fire slugs or scattered shells?
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration")
    TEnumAsByte<ETPSProjectileBehavior> ProjectileBehavior;

    // Amount of individual projectiles per-shot (ie: Shotgun)
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Configuration", meta = (EditCondition = "ProjectileBehavior == ETPSProjectileBehavior::Spread", EditConditionHides))
    float SpreadCount = 1.0;

    // Delay before a fire actually happens (reaction/trigger-pull timme)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Handling")
    float TriggerDelaySeconds = 0.1;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Handling")
    float ReloadDelaySeconds = 3.0f;

    // 0 - 1 scale of recoil noise
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Handling")
    float RecoilScalar = 0.5; 

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammunition")
    TEnumAsByte<ETPSAmmunitionType> AmmunitionType = FortyFive_ACP;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammunition")
    int AmmunitionCapacity = 10;

    // Weapon can 'plus-1'
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammunition")
    bool HasChamber = true;

    // Should we use Degrees or MOA for Spread calculation
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Accuracy")
    TEnumAsByte<ETPSWeaponSpreadMode> SpreadMode = Degrees;
    // Degrees of shot spread in Degrees (X=Pitch, Y=Yaw)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Accuracy", meta = (EditCondition = "SpreadMode == ETPSWeaponSpreadMode::Degrees", EditConditionHides))
    FVector2D SpreadDegrees = FVector2D(1, 1);
    // Degrees of shot spread in MOA
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Accuracy", meta = (EditCondition = "SpreadMode == ETPSWeaponSpreadMode::MOA", EditConditionHides))
    float SpreadMOA = 1.0;
};
