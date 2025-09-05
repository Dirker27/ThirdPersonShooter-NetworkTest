// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "TPSGearItem.h"

#include "Equipment/TPSEquipmentSlot.h"
#include "Equipment/TPSEquipmentLoadout.h"
#include "Weapon/Types/TPSWeaponSlot.h"

#include "TPSEquipmentManager.generated.h"

/*
 * Manages a Pawn's Equipment Items
 *  - Mounts visible Actors to Mesh (EquipmentHarness)
 *  - Applies Equipment GAS Abilities/Effects to owning Pawn's AbilitySystem
 *  - Attaches owner's AbilitySystem to Equipment/Weapons to extend abilities
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
    TEnumAsByte<ETPSWeaponSlot> ActiveEquipmentSlot;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EquipmentManager|State")
    TObjectPtr<UTPSEquipmentLoadout> Loadout;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EquipmentManager|State")
    bool IsReadied;

//~ ======================================================================== ~//
//  CONFIGURATION
//~ ======================================================================== ~//
protected:
    //////////////////////////////////////////////////////
    // Target Mesh Configuration

    // Parent Mesh - Bound on Startup
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EquipmentManager|Configuration")
    TObjectPtr<USkeletalMeshComponent> TargetMesh;

    // Owner's AbilitySystem (integrates to equipment w/ owner GAS) - Bound on Startup
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EquipmentManager|Configuration")
    TObjectPtr<UAbilitySystemComponent> OwnerAsc;

    // HarnessSlot -> Mesh SocketName (MountPoint setup only)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EquipmentManager|Configuration")
    TMap<TEnumAsByte<ETPSEquipmentHarnessSlot>, FName> HarnessSocketMap;

    // EquipmentSlot -> HarnessSlot (when holstered)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EquipmentManager|Configuration")
    TMap<TEnumAsByte<ETPSWeaponSlot>, TEnumAsByte<ETPSEquipmentHarnessSlot>> EquipmentHarnessMap;

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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EquipmentManager|State", Replicated)
    TArray<ATPSWeapon*> WeaponItems;

    //////////////////////////////////////////////////////
    // Gear Item Instances

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EquipmentManager|State", Replicated)
    TArray<ATPSGearItem*> GearItems;


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
    //   - Bound GAS abilities/attributes will be applied to owner's AbilitySystem.
    //   - If slot is occupied, that equipment item will be dropped.
    UFUNCTION(BlueprintCallable)
    void PickUpWeapon(ATPSWeapon* weapon, const ETPSWeaponSlot slot);

    UFUNCTION(BlueprintCallable)
    void PickUpGearItem(ATPSGearItem* gearItem, const ETPSGearSlot slot);

    // Drops an equipment item from 
    UFUNCTION(BlueprintCallable)
    void DropWeaponFromSlot(const ETPSWeaponSlot slot);
    UFUNCTION(BlueprintCallable)
    void DropGearItemFromSlot(const ETPSGearSlot slot);
    UFUNCTION(BlueprintCallable)
    void DropAll();


    UFUNCTION(BlueprintCallable)
    void DestroyWeaponAtSlot(const ETPSWeaponSlot slot);
    UFUNCTION(BlueprintCallable)
    void DestroyGearItemAtSlot(const ETPSGearSlot slot);
    UFUNCTION(BlueprintCallable)
    void DestroyAll();


//~ ============================================================= ~//
//  EQUIPMENT OPERATIONS
//~ ============================================================= ~//
public:

    // Equip a Weapon assigned to given slot.
    //  - Weapon must already be picked up and registered in inventory.
    //  - Applies Weapon's active effects to owner's AbilitySystem.
    UFUNCTION(BlueprintCallable)
    void EquipWeapon(const ETPSWeaponSlot equipmentSlot);

    // Un-Equips a Weapon at the registered slot. to the mapped EquipmentHarness slot.
    UFUNCTION(BlueprintCallable)
    void HolsterWeapon(const ETPSWeaponSlot slot);

    UFUNCTION(BlueprintCallable, BlueprintPure)
    ATPSWeapon* GetWeapon(const ETPSWeaponSlot slot);

    UFUNCTION(BlueprintCallable, BlueprintPure)
    ATPSGearItem* GetGearItem(const ETPSGearSlot slot);

private:
    void ConfigureHarnessSlots();

    void InstantiateWeaponAndAssignToSlot(TSubclassOf<ATPSWeapon>, const ETPSWeaponSlot slot);
    void InstantiateGearItemAndAssignToSlot(TSubclassOf<ATPSGearItem>, const ETPSGearSlot slot);

    void EquipToPrimaryWeaponHand(ATPSEquipableItem* item);

    void ApplyActiveItemEffectsToOwner(ATPSEquipableItem* item);
    void RemoveActiveItemEffectsFromOwner(ATPSEquipableItem* item);
    void ApplyPassiveItemEffectsToOwner(ATPSEquipableItem* item);
    void RemovePassiveItemEffectsFromOwner(ATPSEquipableItem* item);

    UTPSMountPoint* GetHarnessMountPointForEquipmentSlot(const ETPSWeaponSlot slot);
    UTPSMountPoint* GetHarnessMountPointForGearSlot(const ETPSGearSlot slot);
};
