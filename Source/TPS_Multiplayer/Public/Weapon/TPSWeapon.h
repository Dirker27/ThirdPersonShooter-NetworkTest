// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "Equipment/TPSEquipableItem.h"
#include "Reticle/TPSReticle.h"
#include "Types/TPSTargetInfo.h"
#include "Weapon/Types/TPSWeaponSlot.h"
#include "Weapon/Types/TPSWeaponType.h"
#include "Weapon/TPSWeaponConfiguration.h"
#include "Weapon/TPSWeaponState.h"

#include "TPSWeapon.generated.h"

/**
 * An Instance of a Weapon in the World that can be fired, dropped, and picked up.
 */
UCLASS()
class TPS_MULTIPLAYER_API ATPSWeapon : public ATPSEquipableItem
{
    GENERATED_BODY()

public:
    ATPSWeapon();
    //~ATPSWeapon() {};

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void BeginDestroy() override;


//~ ======================================================================= ~//
//  COMPONENTS
//~ ======================================================================== ~//
public:
    // UI Reticle
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Schema")
    TObjectPtr<UTPSWeaponReticle> Reticle;

    // Mount offset for WeaponHand
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mountable|Schema")
    FTPSMountOffset PrimaryWeaponHandOffset = FTPSMountOffset();

    // IK Target for SecondaryHand
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Schema")
    TObjectPtr<UTPSMountPoint> SecondaryWeaponHandMountPoint;

//~ ======================================================================= ~//
//  ATTRIBUTES
//~ ======================================================================= ~//
public:
    //////////////////////////////////////////////////////
    // Identity

    // Weapon Type
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Identity")
    TEnumAsByte<ETPSWeaponType> Type;

    //////////////////////////////////////////////////////
    // Schema

    // Effects to be applied to the owning Pawn's AbilitySystem when equipped (active)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipable|Schema")
    TEnumAsByte<ETPSWeaponSlot> WeaponSlot;

    // Weapon Attributes (damage, spread, cycle rate, etc)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Schema")
    TObjectPtr<UTPSWeaponConfiguration> Configuration;

    //////////////////////////////////////////////////////
    // State

    // Ammo
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State", Replicated);
    int CurrentAmmunitionCount;
    //
    // State
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State", Replicated)
    TEnumAsByte<ETPSWeaponState> CurrentWeaponState;
    //
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State", Replicated)
    TEnumAsByte<ETPSWeaponState> PreviousWeaponState;
    //
    // Is Currently Reloading
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State", Replicated)
    bool IsReloading;
    //
    // Is Currently Firing
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State", Replicated)
    bool IsFiring;
    //
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
    bool HasEverFired;
    //
    // Is Currently Aiming (Behavior / Aesthetic Modifier)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State", Replicated)
    bool IsAiming;
    //
    // Should highlight / render display frame to Player.
    UFUNCTION(BlueprintCallable)
    bool ShouldRenderUnitFrame() { return !IsOwned; }



    //////////////////////////////////////////////////////
    // Fire Control / Trigger Behavior


    // Meter fire rate
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State|Fire Control")
    float TimeLastFired;
    //
    // Consume the trigger (single/burst mode)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State|Fire Control")
    bool HasTriggerCompleted;
    //
    // For burst-fire count
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State|Fire Control")
    int SuccessiveFireCount;


    //////////////////////////////////////////////////////
    // Targeting

    // Targeted Location(s) / Actor(s)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State|Targeting")
    FTPSTargetInfo TargetInfo;

    // Where the muzzle is CURRENTLY pointed (lerps to Target)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State|Targeting", Replicated);
    FVector CurrentPointOfAim;
    // Interpolation rate for CurrentLookLocation - gross adjustment (turning around)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration|Targeting")
    float TargetInterpRateFar = 0.5f;
    // Interpolation speed for CurrentLookLocation - fine adjustment (aiming / jitter)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration|Targeting")
    float TargetInterpRateClose = .8f;
    // How close Current->Target can be before using the "close" interpolation.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration|Targeting")
    float TargetingInterpThreshold = 100;


    //////////////////////////////////////////////////////
    // Accuracy

    // Accuracy (Degrees of freedom in Pitch/Yaw)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State|Targeting");
    FVector2D TargetAccuracyTolerance;





//~ ======================================================================== ~//
//  BEHAVIOR
//~ ======================================================================== ~//
public:
    UFUNCTION(BlueprintCallable)
    void Fire();
    virtual void PerformFire() { UE_LOG(LogTemp, Log, TEXT("Weapon::PerformFire()")); };

    UFUNCTION(BlueprintCallable, BlueprintPure)
    bool CanFire();

    UFUNCTION(BlueprintCallable)
    void StartReload();

    UFUNCTION(BlueprintCallable)
    void CommitReload(int newAmmunitionCount);

private:
    UFUNCTION(BlueprintCallable)
    void ApplyWeaponState(const ETPSWeaponState newState);

//~ ======================================================================== ~//
//  Equipable Overrides
//~ ======================================================================== ~//
public:
    virtual void StartUse() override;
    virtual void StopUse() override;
	virtual void Equip() override;
    virtual void UnEquip() override;

//~ ======================================================================== ~//
//  Blueprint Extensions
//~ ======================================================================== ~//
public:
    //////////////////////////////////////////////////////
    // Transforms

    // WeaponType
    UFUNCTION(BlueprintCallable, BlueprintPure)
    static FString WeaponTypeToFString(const ETPSWeaponType t) {
        return FString(ETPSWeaponTypeToString(t));
    };
    
    // WeaponState
    UFUNCTION(BlueprintCallable, BlueprintPure)
    static FString WeaponStateToFString(const ETPSWeaponState state) {
        return FString(ETPSWeaponStateToString(state));
    };
};

