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

//~ ======================================================================== ~//
//  STATE
//~ ======================================================================== ~//
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EquipmentManager|State", Replicated)
    TEnumAsByte<ETPSEquipmentSlot> ActiveEquipmentSlot;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EquipmentManager|State")
    TObjectPtr<UTPSEquipmentLoadout> Loadout;

//~ ======================================================================== ~//
//  CONFIGURATION
//~ ======================================================================== ~//
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
    TMap<TEnumAsByte<ETPSEquipmentSlot>, TEnumAsByte<ETPSEquipmentHarnessSlot>> EquipmentHarnessMap;

    // EquipmentSlot -> HarnessSlot (when holstered)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EquipmentManager|Configuration")
    TMap<TEnumAsByte<ETPSGearSlot>, TEnumAsByte<ETPSEquipmentHarnessSlot>> GearHarnessMap;

//~ ======================================================================== ~//
//  STORAGE
//~ ======================================================================== ~//
protected:
    // HarnessSlot -> MountPoint
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EquipmentManager|Configuration")
    TMap<TEnumAsByte<ETPSEquipmentHarnessSlot>, UTPSMountPoint*> HarnessMountPointMap;

    //////////////////////////////////////////////////////
    // Equipment Item Instances

    // Instances are stored as individual pointers b/c Map<>s are not
    //   supported for replication.

    // Owned Equipment Items (provide active GAS Effects and Usable actions)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EquipmentManager|State")
    TArray<TEnumAsByte<ETPSEquipmentSlot>> PopulatedEquipmentSlots;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EquipmentManager|State", Replicated)
    TObjectPtr<ATPSEquipableItem> PrimaryWeaponInstance;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EquipmentManager|State", Replicated)
    TObjectPtr<ATPSEquipableItem> SecondaryWeaponInstance;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EquipmentManager|State", Replicated)
    TObjectPtr<ATPSEquipableItem> TertiaryWeaponInstance;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EquipmentManager|State", Replicated)
    TObjectPtr<ATPSEquipableItem> LethalEquipmentInstance;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EquipmentManager|State", Replicated)
    TObjectPtr<ATPSEquipableItem> TacticalEquipmentInstance;

    //////////////////////////////////////////////////////
    // Gear Item Instances

    // TODO: Convert this to use the Array<->Instance grouping (if we need to replicate these instances)

    // Owned Gear Items (provide passive GAS Effects)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EquipmentManager|State")
    TMap<TEnumAsByte<ETPSGearSlot>, ATPSEquipableItem*> GearItems;


//~ ======================================================================== ~//
// PUBLIC OPERATIONS
//~ ======================================================================== ~//
public:

    //////////////////////////////////////////////////////
    // Startup and Binding

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
    void PickUpEquipmentItem(ATPSEquipableItem* equipmentItem, const ETPSEquipmentSlot slot);
    UFUNCTION(BlueprintCallable)
    void PickUpGearItem(ATPSEquipableItem* gearItem, const ETPSGearSlot slot);

    // Drops an equipment item from 
    UFUNCTION(BlueprintCallable)
    void DropEquipmentItem(const ETPSEquipmentSlot slot);
    UFUNCTION(BlueprintCallable)
    void DropGearItem(const ETPSGearSlot slot);
    UFUNCTION(BlueprintCallable)
    void DropAll();


    UFUNCTION(BlueprintCallable)
    void DestroyEquipmentItem(const ETPSEquipmentSlot slot);
    UFUNCTION(BlueprintCallable)
    void DestroyGearItem(const ETPSGearSlot slot);
    UFUNCTION(BlueprintCallable)
    void DestroyAll();


//~ ============================================================= ~//
//  EQUIPMENT OPERATIONS
//~ ============================================================= ~//
public:
    UFUNCTION(BlueprintCallable)
    void UnEquipActive();

    // TODO: Rename 'EquipItem'(?)
    UFUNCTION(BlueprintCallable)
    void EquipAndArm(const ETPSEquipmentSlot equipmentSlot);

    UFUNCTION(BlueprintCallable)
    void HolsterEquipmentItem(const ETPSEquipmentSlot slot);

    // TODO: Rename 'GetItem'(?)
    UFUNCTION(BlueprintCallable, BlueprintPure)
    ATPSEquipableItem* GetItemFromEquipmentSlot(const ETPSEquipmentSlot slot);

    UFUNCTION(BlueprintCallable, BlueprintPure)
    ATPSEquipableItem* GetGearItem(const ETPSGearSlot slot);

private:
    void ConfigureHarnessSlots();

    void InstantiateAndAssignEquipmentItemToSlot(TSubclassOf<ATPSEquipableItem>, const ETPSEquipmentSlot slot);
    void InstantiateAndAssignGearItemToSlot(TSubclassOf<ATPSEquipableItem>, const ETPSGearSlot slot);

    void EquipToPrimaryWeaponHand(ATPSEquipableItem* item);

    UTPSMountPoint* GetHarnessMountPointForEquipmentSlot(const ETPSEquipmentSlot slot);
    UTPSMountPoint* GetHarnessMountPointForGearSlot(const ETPSGearSlot slot);
};
