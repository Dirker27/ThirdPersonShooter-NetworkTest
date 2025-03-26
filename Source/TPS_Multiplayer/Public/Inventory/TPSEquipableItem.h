// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"

#include "AbilitySystemComponent.h"
#include "Behavior/TPSMountable.h"
#include "Components/BoxComponent.h"

#include "TPSEquipableItem.generated.h"

UDELEGATE(BlueprintAuthorityOnly, NetMulticast, Reliable)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPerformPickup);

UDELEGATE(BlueprintAuthorityOnly, NetMulticast, Reliable)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPerformDrop);

UCLASS()
class TPS_MULTIPLAYER_API ATPSEquipableItem : public ATPSMountableActor
{
    GENERATED_BODY()

public:
    ATPSEquipableItem();
    //~ATPSEquipableItem();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
    virtual void BeginPlay() override;
    virtual void BeginDestroy() override;

//~ ============================================================= ~//
//  COMPONENTS
//~ ============================================================= ~//
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
    TWeakObjectPtr<UAbilitySystemComponent> OwnerAsc;

//~ ============================================================= ~//
//  ATTRIBUTES
//~ ============================================================= ~//
public:
    //- Identity ------------------------------------------=
    //
    //- Name
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPS|Identity")
    FString Name;

    //- Configuration ------------------------------------=
    //
    //- Name
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TPS|Configuration")
    TArray<TSubclassOf<UGameplayEffect>> AppliedEffects;

    //- State --------------------------------------------=
    //
    //- IsOwned
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TPS|Equippable|State", Replicated)
    bool IsOwned;
    //
    //- IsEquipped
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TPS|Equippable|State", Replicated)
    bool IsEquipped;

//~ ============================================================= ~//
//  BEHAVIOR
//~ ============================================================= ~//
public:
    //- Pickupable -----------------------------------------=
    //
    // TODO: Move to IPickupable?
    UFUNCTION(BlueprintCallable)
    void Pickup();
    UFUNCTION(BlueprintImplementableEvent)
    void OnPickup();
    virtual void PerformPickup() { OnPickup(); }
    //
    UFUNCTION(BlueprintCallable)
    void Drop();
    UFUNCTION(BlueprintImplementableEvent)
    void OnDrop();
    virtual void PerformDrop() { OnDrop(); }

    //- Equipable -----------------------------------------=
    //
    UFUNCTION(BlueprintCallable)
    virtual void Equip();
    UFUNCTION(BlueprintImplementableEvent)
    void OnEquip();
    //
    UFUNCTION(BlueprintCallable)
    virtual void UnEquip();
    UFUNCTION(BlueprintImplementableEvent)
    void OnUnEquip();

    //- Usable ------------------------------------------=
    //  (TODO: Extract to UsableItem?)
    UFUNCTION(BlueprintCallable)
    virtual void StartUse();
    UFUNCTION(BlueprintImplementableEvent)
    void OnStartUse();
    //
    UFUNCTION(BlueprintCallable)
    virtual void StopUse();
    UFUNCTION(BlueprintImplementableEvent)
    void OnStopUse();

//~ ============================================================= ~//
//  STATE MODIFIERS
//~ ============================================================= ~//
private:
    UFUNCTION(BlueprintCallable)
    void EnableWorldCollision();

    UFUNCTION(BlueprintCallable)
    void DisableWorldCollision();
};