// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"

#include "AbilitySystemComponent.h"
#include "Components/BoxComponent.h"

#include "Behavior/TPSMountable.h"

#include "TPSEquipableItem.generated.h"

UDELEGATE(BlueprintAuthorityOnly, NetMulticast, Reliable)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPerformPickup);

UDELEGATE(BlueprintAuthorityOnly, NetMulticast, Reliable)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPerformDrop);

/**
 * The physical Actor class for piece of Equipment.
 *
 * Equipment characteristics are read from the EquipmentDefinition and applied to
 *  this object at instantiation.
 */
UCLASS()
class TPS_MULTIPLAYER_API ATPSEquipableItem : public ATPSMountableActor
{
    GENERATED_BODY()

public:
    ATPSEquipableItem();
    //~ATPSEquipableItem() {};

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
    virtual void BeginPlay() override;

//~ ========================================================================= ~//
//  COMPONENTS
//~ ========================================================================= ~//
protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<USkeletalMeshComponent> Mesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UBoxComponent> CollisionComponent;

#if WITH_EDITORONLY_DATA
    /** Component shown in the editor only to indicate character facing */
    UPROPERTY()
    TObjectPtr<UArrowComponent> ArrowComponent;
#endif

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UAbilitySystemComponent> OwnerAsc;

//~ ========================================================================= ~//
//  ATTRIBUTES
//~ ========================================================================= ~//
public:
    //////////////////////////////////////////////////////////////////
    // Identity

    //- Name
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Identity")
    FString Name;

    /////////////////////////////////////////////////////////////////
    // Schema

        // Effects to be applied to the owning Pawn's AbilitySystem when equipped (active)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipable|Effects")
    TArray<TSubclassOf<UGameplayEffect>> ActiveEffects;

    // Effects to be applied to the owning Pawn's AbilitySystem when in inventory (passive)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipable|Effects")
    TArray<TSubclassOf<UGameplayEffect>> PassiveEffects;

    // Offset to use when item is mounted to a Pawn. (not armed)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mountable|Schema")
    FTPSMountOffset WeaponHolsterOffset = FTPSMountOffset();

    /////////////////////////////////////////////////////////////////
    // State

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipable|Effects")
    TArray<FActiveGameplayEffectHandle> AppliedActiveEffectHandles;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipable|Effects")
    TArray<FActiveGameplayEffectHandle> AppliedPassiveEffectHandles;

    // IsOwned by a Pawn
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipable|TPSGameState", Replicated)
    bool IsOwned;
    
    // IsEquipped by a Pawn
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipable|TPSGameState", Replicated)
    bool IsEquipped;

//~ ========================================================================= ~//
//  BEHAVIOR
//~ ========================================================================= ~//
public:
    /////////////////////////////////////////////////////////////////
    // Pickupable

    // TODO: Move to IPickupable?
    UFUNCTION(BlueprintCallable)
    void Pickup();
    UFUNCTION(BlueprintCallable, BlueprintPure)
    bool CanPickup();
    UFUNCTION(BlueprintImplementableEvent)
    void OnPickup();
    virtual void PerformPickup() { OnPickup(); }
    //
    UFUNCTION(BlueprintCallable)
    void Drop();
    UFUNCTION(BlueprintImplementableEvent)
    void OnDrop();
    virtual void PerformDrop() { OnDrop(); }

    /////////////////////////////////////////////////////////////////
    // Equipable

    // TODO: Move to IEquipable?
    UFUNCTION(BlueprintCallable)
    virtual void Equip();
    UFUNCTION(BlueprintImplementableEvent)
    void OnEquip();
    //
    UFUNCTION(BlueprintCallable)
    virtual void UnEquip();
    UFUNCTION(BlueprintImplementableEvent)
    void OnUnEquip();

    /////////////////////////////////////////////////////////////////
    // Usable

    // TODO: Extract to UsableItem?
    UFUNCTION(BlueprintCallable)
    virtual void StartUse();
    UFUNCTION(BlueprintImplementableEvent)
    void OnStartUse();
    //
    UFUNCTION(BlueprintCallable)
    virtual void StopUse();
    UFUNCTION(BlueprintImplementableEvent)
    void OnStopUse();

//~ ========================================================================= ~//
//  STATE MODIFIERS
//~ ========================================================================= ~//
private:
    UFUNCTION(BlueprintCallable)
    void EnableWorldCollision();

    UFUNCTION(BlueprintCallable)
    void DisableWorldCollision();
};
