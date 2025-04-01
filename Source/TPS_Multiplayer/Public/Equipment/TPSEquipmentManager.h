// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "Equipment/TPSEquipmentSlot.h"
#include "Equipment/TPSEquipmentLoadout.h"

#include "TPSEquipmentManager.generated.h"

/*
 * Manages a Pawn's Equipment Items
 *  - Mounts visible Actors to Mesh (EquipmentHarness)
 *  - Applies Equipment GAS Abilities/Effects to owning Pawn's ASC
 *  - Attaches owner's ASC to Equipment/Weapons to extend abilities
 */
UCLASS()
class TPS_MULTIPLAYER_API UTPSEquipmentManager : public UActorComponent
{
    GENERATED_BODY()

public:
    UTPSEquipmentManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
    //~UTPSEquipmentManager();

protected:
    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual void BeginDestroy() override;

//~ ============================================================= ~//
//  STATE
//~ ============================================================= ~//
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EquipmentManager|State", Replicated)
    TEnumAsByte<ETPSEquipmentSlot> ActiveEquipmentSlot;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EquipmentManager|State")
    TObjectPtr<UTPSEquipmentLoadout> Loadout;

//~ ============================================================= ~//
//  CONFIGURATION
//~ ============================================================= ~//
protected:
    //////////////////////////////////////////////////////
    // Target Mesh Configuration

    // Parent Mesh - Bound on Startup
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EquipmentManager|Configuration")
    TWeakObjectPtr<USkeletalMeshComponent> TargetMesh;

    // Owner's ASC (integrates to equipment w/ owner GAS) - Bound on Startup
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EquipmentManager|Configuration")
    TWeakObjectPtr<UAbilitySystemComponent> OwnerAsc;

    // HarnessSlot -> Mesh SocketName (MountPoint setup only)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EquipmentManager|Configuration")
    TMap<TEnumAsByte<ETPSEquipmentHarnessSlot>, FName> HarnessSocketMap;

    // EquipmentSlot -> HarnessSlot (when holstered)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EquipmentManager|Configuration")
    TMap<TEnumAsByte<ETPSEquipmentSlot>, TEnumAsByte<ETPSEquipmentHarnessSlot>> EquipmentHolsterMap;


//~ ============================================================= ~//
//  COMPONENTS
//~ ============================================================= ~//
public:
    // HarnessSlot -> MountPoint
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EquipmentManager|Configuration")
    TMap<TEnumAsByte<ETPSEquipmentHarnessSlot>, UTPSMountPoint*> HarnessMountPointMap;

    // Equipment Instances
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EquipmentManager|State")
    TMap<TEnumAsByte<ETPSEquipmentSlot>, ATPSEquipableItem*> EquipmentMap;


//~ ============================================================= ~//
//  PUBLIC OPERATIONS
//~ ============================================================= ~//
public:

    //////////////////////////////////////////////////////
    // Startup and Binding

    //UFUNCTION(BlueprintCallable)
    void BindToMesh(USkeletalMeshComponent* mesh);

    void BindToOwnerAbilitySystem(UAbilitySystemComponent* ownerAsc);

    UFUNCTION(BlueprintCallable)
    void Initialize();

protected:
    UFUNCTION(BlueprintCallable)
    void InstantiateLoadout();

public:
    //////////////////////////////////////////////////////
    // Target Mesh Configuration

    // Equip the current ActiveWeaponSlot to the armed hand.
    //   If no equipment is currently active, pick the first available slot.
    UFUNCTION(BlueprintCallable)
    void Ready();

    // Stow the currently Equipped Weapon, but don't change the active slot.
    UFUNCTION(BlueprintCallable)
    void UnReady();

    //////////////////////////////////////////////////////
    // Drop / Pickup Items

    // Pick up a new item and bind it to the designated equipment slot.
    //   - Bound GAS abilities/attributes will be applied to owner's ASC.
    //   - If slot is occupied, that equipment item will be dropped.
    UFUNCTION(BlueprintCallable)
    void PickupAndAssignEquipmentToSlot(ATPSEquipableItem* equipmentItem, ETPSEquipmentSlot slot);
    // Drops an equipment item from 
    UFUNCTION(BlueprintCallable)
    void DropEquipmentFromSlot(ETPSEquipmentSlot slot);
    UFUNCTION(BlueprintCallable)
    void DropAll();


    UFUNCTION(BlueprintCallable)
    void DestroyItemAtSlot(ETPSEquipmentSlot slot);
    UFUNCTION(BlueprintCallable)
    void DestroyAll();


    UFUNCTION(BlueprintCallable)
    void EquipPrimary();
    UFUNCTION(BlueprintCallable)
    void EquipSecondary();
    UFUNCTION(BlueprintCallable)
    void EquipTertiary();
    UFUNCTION(BlueprintCallable)
    void EquipLethalThrowable();
    UFUNCTION(BlueprintCallable)
    void EquipTacticalThrowable();


    UFUNCTION(BlueprintCallable)
    void WeaponSwap();
    UFUNCTION(BlueprintCallable)
    void EquipmentSwap();

//~ ============================================================= ~//
//  EQUIPMENT OPERATIONS
//~ ============================================================= ~//
protected:
    UFUNCTION(BlueprintCallable)
    void UnEquipActive();
    UFUNCTION(BlueprintCallable)
    void EquipAndArm(ETPSEquipmentSlot equipmentSlot);

    UFUNCTION(BlueprintCallable)
    void EquipWeaponToHolster(ATPSEquipableItem* weapon, ETPSEquipmentSlot slot);
    UFUNCTION(BlueprintCallable)
    void EquipToPrimaryWeaponHand(ATPSEquipableItem* weapon);
    UFUNCTION(BlueprintCallable)
    void EquipToSecondaryWeaponHand(ATPSEquipableItem* weapon);
    UFUNCTION(BlueprintCallable)
    void EquipToPrimaryHolster(ATPSEquipableItem* weapon);
    UFUNCTION(BlueprintCallable)
	void EquipToSecondaryHolster(ATPSEquipableItem* weapon);
    UFUNCTION(BlueprintCallable)
    void EquipToTacticalHolster(ATPSEquipableItem* weapon);
    UFUNCTION(BlueprintCallable)
	void EquipToLethalHolster(ATPSEquipableItem* weapon);
    UFUNCTION(BlueprintCallable)
    void EquipToBackHolster(ATPSEquipableItem* weapon);


protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category="EquipmentManager|State")
    TObjectPtr<ATPSEquipableItem> PrimaryWeaponInstance;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "EquipmentManager|State")
    TObjectPtr<ATPSEquipableItem> SecondaryWeaponInstance;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "EquipmentManager|State")
    TObjectPtr<ATPSEquipableItem> TertiaryWeaponInstance;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "EquipmentManager|State")
    TObjectPtr<ATPSEquipableItem> LethalEquipmentInstance;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "EquipmentManager|State")
    TObjectPtr<ATPSEquipableItem> TacticalEquipmentInstance;

public:
    UFUNCTION(BlueprintCallable)
    ATPSEquipableItem* GetItemFromEquipmentSlot(ETPSEquipmentSlot slot);

private:
    void InstantiateAndAssignEquipmentToSlot(TSubclassOf<ATPSEquipableItem>, ETPSEquipmentSlot slot);

    UTPSMountPoint* GetHolsterMountPointForEquipmentSlot(ETPSEquipmentSlot slot);
};