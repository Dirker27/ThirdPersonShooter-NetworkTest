// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "Equipment/TPSEquipmentSlot.h"
#include "Equipment/TPSEquipmentLoadout.h"

#include "TPSEquipmentManager.generated.h"

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

    //WeaponController weaponController;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EquipmentManager|State")
    TObjectPtr<UTPSEquipmentLoadout> Loadout;

//~ ============================================================= ~//
//  CONFIGURATION
//~ ============================================================= ~//
public:
    //////////////////////////////////////////////////////
    // Target Mesh Configuration

    // Parent Mesh
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EquipmentManager|Configuration")
    TWeakObjectPtr<USkeletalMeshComponent> TargetMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EquipmentManager|Configuration")
    TMap<TEnumAsByte<ETPSEquipmentHarnessSlot>, FName> HarnessSocketMap;

    //
    // Weapon Hands
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EquipmentManager|Configuration")
    FName PrimaryWeaponHandBone;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EquipmentManager|Configuration")
    FName SecondaryWeaponHandBone;
    //
    // Hip Holsters
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EquipmentManager|Configuration")
    FName LeftHipHolsterBone;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EquipmentManager|Configuration")
    FName RightHipHolsterBone;
    //
    // Leg Holsters
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EquipmentManager|Configuration")
    FName LeftLegHolsterBone;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EquipmentManager|Configuration")
    FName RightLegHolsterBone;
    //
    // Back Holsters
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EquipmentManager|Configuration")
    FName BackHolsterBone;

//~ ============================================================= ~//
//  COMPONENTS
//~ ============================================================= ~//
protected:
    //////////////////////////////////////////////////////
    // Mount Targets
    
    // Weapon Hands
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EquipmentManager|Configuration")
    TObjectPtr<UTPSMountPoint> PrimaryWeaponHand;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EquipmentManager|Configuration")
    TObjectPtr<UTPSMountPoint> SecondaryWeaponHand;
    //
    // Hip Holsters
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EquipmentManager|Configuration")
    TObjectPtr<UTPSMountPoint> LeftHipHolster;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EquipmentManager|Configuration")
    TObjectPtr<UTPSMountPoint> RightHipHolster;
    //
    // Leg Holsters
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EquipmentManager|Configuration")
    TObjectPtr<UTPSMountPoint> LeftLegHolster;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EquipmentManager|Configuration")
    TObjectPtr<UTPSMountPoint> RightLegHolster;
    //
    // Back Holsters
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EquipmentManager|Configuration")
    TObjectPtr<UTPSMountPoint> BackHolster;


public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EquipmentManager|Configuration")
    TWeakObjectPtr<UAbilitySystemComponent> OwnerAsc;


//~ ============================================================= ~//
//  PUBLIC OPERATIONS
//~ ============================================================= ~//
public:
    //UFUNCTION(BlueprintCallable)
    void BindToMesh(USkeletalMeshComponent* mesh);

    void BindToOwner(UAbilitySystemComponent* ownerAsc);

    UFUNCTION(BlueprintCallable)
    void Initialize();
    UFUNCTION(BlueprintCallable)
    void InstantiateLoadout();

    UFUNCTION(BlueprintCallable)
    void Ready();
    UFUNCTION(BlueprintCallable)
    void UnReady();

    UFUNCTION(BlueprintCallable)
    void PickupAndAssignEquipmentItemToSlot(ATPSEquipableItem* equipmentItem, ETPSEquipmentSlot slot);
    UFUNCTION(BlueprintCallable)
    void DropEquipmentItemFromSlot(ETPSEquipmentSlot slot);
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
    ATPSEquipableItem* GetItemFromEquipmentSlot(ETPSEquipmentSlot slot) const;

private:
    void InstantiateAndAssignItemToSlot(TSubclassOf<ATPSEquipableItem>, ETPSEquipmentSlot slot);

    UTPSMountPoint* GetMountPointForSlot(ETPSEquipmentSlot slot) const;
};