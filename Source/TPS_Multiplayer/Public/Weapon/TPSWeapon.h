// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "Equipment/TPSEquipableItem.h"
#include "Reticle/TPSReticle.h"
#include "Weapon/TPSWeaponType.h"
#include "Weapon/TPSWeaponConfiguration.h"
#include "Weapon/TPSWeaponState.h"

#include "TPSWeapon.generated.h"

UDELEGATE(BlueprintAuthorityOnly, NetMulticast, Reliable)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPerformFire);

UDELEGATE(BlueprintAuthorityOnly, NetMulticast, Reliable)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPerformEquip);

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

public:
    //- Broadcast Delegate
    //UPROPERTY(BlueprintAssignable)
    //FPerformFire NotifyPerformFire;

//~ ============================================================= ~//
//  ATTRIBUTES
//~ ============================================================= ~//
public:
    //////////////////////////////////////////////////////
    // Identity

    // Weapon Type
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="TPSWeapon|Identity")
    TEnumAsByte<ETPSWeaponType> Type;

    //////////////////////////////////////////////////////
    // Configuration

    // Weapon Attributes (damage, spread, cycle rate, etc)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TPSWeapon|Configuration")
    TObjectPtr<UTPSWeaponConfiguration> Configuration;

    // UI Reticle
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSWeapon|Configuration")
    TObjectPtr<UTPSWeaponReticle> Reticle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSWeapon|Configuration")
    FTPSMountOffset WeaponHandOffset = FTPSMountOffset();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSWeapon|Configuration")
    FTPSMountOffset WeaponHolsterOffset = FTPSMountOffset();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSWeapon|Configuration")
    TObjectPtr<UTPSMountPoint> SecondaryHandMountLocation;

    //////////////////////////////////////////////////////
    // State

    // Ammo
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSWeapon|State", Replicated);
    int CurrentAmmunitionCount;
    //
    // State
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSWeapon|State", Replicated)
    TEnumAsByte<ETPSWeaponState> CurrentWeaponState;
    //
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TPSWeapon|State", Replicated)
    TEnumAsByte<ETPSWeaponState> PreviousWeaponState;
    //
    // Is Currently Reloading
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSWeapon|State", Replicated)
    bool IsReloading;
    //
    // Is Currently Firing
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSWeapon|State", Replicated)
    bool IsFiring;
    //
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSWeapon|State")
    bool HasEverFired;
    //
    // Is Currently Aiming (Behavior / Aesthetic Modifier)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSWeapon|State", Replicated)
    bool IsAiming;
    //
    // Should highlight / render display frame to Player.
    UFUNCTION(BlueprintCallable)
    bool ShouldRenderUnitFrame() { return IsOwned; }

    //////////////////////////////////////////////////////
    // Fire Control States

    // Targeting
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSWeapon|State");
    FRotator TargetDirection;
    // Accuracy (Degrees of freedom in Pitch/Yaw)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSWeapon|State");
    FVector2D TargetAccuracyTolerance;
    //
    // Meter fire rate
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSWeapon|State")
    float TimeLastFired;
    //
    // Consume the trigger (single/burst mode)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSWeapon|State")
    bool HasTriggerCompleted;
    //
    // For burst-fire count
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSWeapon|State")
    int SuccessiveFireCount;

//~ ============================================================= ~//
//  BEHAVIOR
//~ ============================================================= ~//
public:
    UFUNCTION(BlueprintCallable)
    void Fire();
    UFUNCTION(BlueprintImplementableEvent)
    void OnFire();

    virtual void PerformFire() { UE_LOG(LogTemp, Log, TEXT("Weapon::PerformFire()")); };

    UFUNCTION(BlueprintCallable, BlueprintPure)
    bool CanFire();

    UFUNCTION(BlueprintCallable)
    void StartReload();

    UFUNCTION(BlueprintCallable)
    void CommitReload(int newAmmunitionCount);

private:
    UFUNCTION(BlueprintCallable)
    void ApplyWeaponState(ETPSWeaponState newState);

//~ ============================================================= ~//
//  Equipable Overrides
//~ ============================================================= ~//
public:
    virtual void StartUse() override;
    virtual void StopUse() override;
	virtual void Equip() override;
    virtual void UnEquip() override;

//~ ============================================================= ~//
//  Blueprint Extensions
//~ ============================================================= ~//
public:
    //- Transforms -----------------------------------------=
    //
    //- WeaponType
    UFUNCTION(BlueprintCallable, BlueprintPure)
    static FString WeaponTypeToFString(const ETPSWeaponType t) {
        return FString(ETPSWeaponTypeToString(t));
    };
    //
    //- WeaponState
    UFUNCTION(BlueprintCallable, BlueprintPure)
    static FString WeaponStateToFString(ETPSWeaponState state) {
        return FString(ETPSWeaponStateToString(state));
    };
};

