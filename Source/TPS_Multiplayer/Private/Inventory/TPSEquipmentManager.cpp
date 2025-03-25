// (C) ToasterCat Studios 2025

#include "Inventory/TPSEquipmentManager.h"

#include "ComponentUtils.h"
#include "Behavior/TPSMountPoint.h"
#include "Character/TPSCharacter.h"
#include "Net/UnrealNetwork.h"

UTPSEquipmentManager::UTPSEquipmentManager()
{
    Loadout = CreateDefaultSubobject<UTPSLoadout>(TEXT("DefaultLoadout"));


    ATPSCharacter* owner = Cast<ATPSCharacter>(GetOwner());

    PrimaryWeaponHand = CreateDefaultSubobject<UTPSMountPoint>(TEXT("MP-PrimaryWeaponHand"));
    //PrimaryWeaponHand->SetupAttachment(owner->GetMesh(), PrimaryWeaponHandBone);

    SecondaryWeaponHand = CreateDefaultSubobject<UTPSMountPoint>(TEXT("MP-SecondaryWeaponHand"));
    //SecondaryWeaponHand->SetupAttachment(owner->GetMesh(), SecondaryWeaponHandBone);

    BackHolster = CreateDefaultSubobject<UTPSMountPoint>(TEXT("MP-BackHolster"));
    //BackHolster->SetupAttachment(owner->GetMesh(), BackHolsterBone);

    LeftHipHolster = CreateDefaultSubobject<UTPSMountPoint>(TEXT("MP-LeftHipHolster"));
    //LeftHipHolster->SetupAttachment(owner->GetMesh(), LeftHipHolsterBone);

    LeftLegHolster = CreateDefaultSubobject<UTPSMountPoint>(TEXT("MP-LeftLegHolster"));
    //LeftLegHolster->SetupAttachment(owner->GetMesh(), LeftLegHolsterBone);

    RightHipHolster = CreateDefaultSubobject<UTPSMountPoint>(TEXT("MP-RightHipHolster"));
    //RightHipHolster->SetupAttachment(owner->GetMesh(), RightHipHolsterBone);

    RightLegHolster = CreateDefaultSubobject<UTPSMountPoint>(TEXT("MP-RightLegHolster"));
    //RightLegHolster->SetupAttachment(owner->GetMesh(), RightLegHolsterBone);
}

void UTPSEquipmentManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UTPSEquipmentManager, ActiveEquipmentSlot);

    DOREPLIFETIME(UTPSEquipmentManager, PrimaryWeaponInstance);
    DOREPLIFETIME(UTPSEquipmentManager, SecondaryWeaponInstance);
    DOREPLIFETIME(UTPSEquipmentManager, TertiaryWeaponInstance);
    DOREPLIFETIME(UTPSEquipmentManager, LethalEquipmentInstance);
    DOREPLIFETIME(UTPSEquipmentManager, TacticalEquipmentInstance);
}

void UTPSEquipmentManager::BeginPlay()
{
    Super::BeginPlay();

    SetIsReplicated(true);
}

void UTPSEquipmentManager::BeginDestroy()
{
    Super::BeginDestroy();

    UE_LOG(LogTemp, Log, TEXT("Destroying EquipmentManager sub-component..."));
    DestroyAll();
}


void UTPSEquipmentManager::Initialize()
{
    if (IsValid(Loadout)) {
        InstantiateLoadout();

        EquipToPrimaryHolster(GetItemFromEquipmentSlot(PrimaryWeapon));
        EquipToSecondaryHolster(GetItemFromEquipmentSlot(SecondaryWeapon));
        EquipToBackHolster(GetItemFromEquipmentSlot(TertiaryWeapon));

        EquipToLethalHolster(GetItemFromEquipmentSlot(LethalEquipment));
        EquipToTacticalHolster(GetItemFromEquipmentSlot(TacticalEquipment));
    }


    // For characters who should spawn equipped.
    if (ActiveEquipmentSlot != None)
    {
        EquipAndArm(ActiveEquipmentSlot);
    }
}

void UTPSEquipmentManager::BindToMesh(USkeletalMeshComponent* mesh)
{
    TargetMesh = mesh;

    PrimaryWeaponHand->TargetParentComponent = TargetMesh;
    SecondaryWeaponHand->TargetParentComponent = TargetMesh;

    BackHolster->TargetParentComponent = TargetMesh;
    LeftHipHolster->TargetParentComponent = TargetMesh;
    LeftLegHolster->TargetParentComponent = TargetMesh;
    RightHipHolster->TargetParentComponent = TargetMesh;
    RightLegHolster->TargetParentComponent = TargetMesh;

    PrimaryWeaponHand->TargetSocketName = FName(TEXT("hand_r"));
    SecondaryWeaponHand->TargetSocketName = FName(TEXT("hand_l"));

    BackHolster->TargetSocketName = FName(TEXT("spine_05"));
    LeftHipHolster->TargetSocketName = FName(TEXT("thigh_l"));
    LeftLegHolster->TargetSocketName = FName(TEXT("calf_l"));
    RightHipHolster->TargetSocketName = FName(TEXT("thigh_r"));
    RightLegHolster->TargetSocketName = FName(TEXT("calf_r"));

    if (IsValid(mesh)) {
        UE_LOG(LogTemp, Log, TEXT("Mounting Holsters..."));

        if (IsValid(PrimaryWeaponHand) && !PrimaryWeaponHandBone.IsNone())
        {
            PrimaryWeaponHand->SetupAttachment(mesh, PrimaryWeaponHandBone);;
        }
        if (IsValid(SecondaryWeaponHand) && !SecondaryWeaponHandBone.IsNone())
        {
            SecondaryWeaponHand->SetupAttachment(mesh, SecondaryWeaponHandBone);
        }

        if (IsValid(BackHolster) && !BackHolsterBone.IsNone())
        {
            BackHolster->SetupAttachment(mesh, BackHolsterBone);
        }
        if (IsValid(LeftHipHolster) && !LeftHipHolsterBone.IsNone())
        {
            LeftHipHolster->SetupAttachment(mesh, LeftHipHolsterBone);
        }
        if (IsValid(LeftLegHolster) && !LeftLegHolsterBone.IsNone())
        {
            LeftLegHolster->SetupAttachment(mesh, LeftLegHolsterBone);
        }
        if (IsValid(RightHipHolster) && !RightHipHolsterBone.IsNone())
        {
            RightHipHolster->SetupAttachment(mesh, RightHipHolsterBone);
        }
        if (IsValid(RightLegHolster) && !RightLegHolsterBone.IsNone())
        {
            RightLegHolster->SetupAttachment(mesh, RightLegHolsterBone);
        }
    }
}

void UTPSEquipmentManager::BindToOwner(UAbilitySystemComponent* ownerAsc)
{
    OwnerAsc = ownerAsc;
}



//~ ============================================================= ~//
//  EQUIPMENT OPERATIONS
//~ ============================================================= ~//

void UTPSEquipmentManager::InstantiateLoadout()
{
    if (!IsValid(Loadout)) { return; }

    if (IsValid(Loadout->primaryWeapon)) {
        UE_LOG(LogTemp, Log, TEXT("Instantiating Primary Weapon [%s]..."), *Loadout->primaryWeapon->GetName());
        PrimaryWeaponInstance = GetWorld()->SpawnActor<ATPSEquipableItem>(Loadout->primaryWeapon);
        PrimaryWeaponInstance->OwnerAsc = OwnerAsc;
    }
    if (IsValid(Loadout->secondaryWeapon)) {
        UE_LOG(LogTemp, Log, TEXT("Instantiating Secondary Weapon [%s]..."), *Loadout->secondaryWeapon->GetName());
        SecondaryWeaponInstance = GetWorld()->SpawnActor<ATPSEquipableItem>(Loadout->secondaryWeapon);
        SecondaryWeaponInstance->OwnerAsc = OwnerAsc;
    }
    if (IsValid(Loadout->tertiaryWeapon)) {
        UE_LOG(LogTemp, Log, TEXT("Instantiating Tertiary Weapon [%s]..."), *Loadout->tertiaryWeapon->GetName());
        TertiaryWeaponInstance = GetWorld()->SpawnActor<ATPSEquipableItem>(Loadout->tertiaryWeapon);
        TertiaryWeaponInstance->OwnerAsc = OwnerAsc;
    }
    if (IsValid(Loadout->tacticalEquipment)) {
        UE_LOG(LogTemp, Log, TEXT("Instantiating Tactical Equipment [%s]..."), *Loadout->tacticalEquipment->GetName());
        TacticalEquipmentInstance = GetWorld()->SpawnActor<ATPSEquipableItem>(Loadout->tacticalEquipment);
        TacticalEquipmentInstance->OwnerAsc = OwnerAsc;
    }
    if (IsValid(Loadout->lethalEquipment)) {
        UE_LOG(LogTemp, Log, TEXT("Instantiating Lethal Equipment [%s]..."), *Loadout->lethalEquipment->GetName());
        LethalEquipmentInstance = GetWorld()->SpawnActor<ATPSEquipableItem>(Loadout->lethalEquipment);
        LethalEquipmentInstance->OwnerAsc = OwnerAsc;
    }
}

void UTPSEquipmentManager::EquipWeaponToHolster(ATPSEquipableItem* weapon, ETPSEquipmentSlot slot)
{
    if (!IsValid(weapon)) { return; }

    UTPSMountPoint* mount = GetMountPointForSlot(slot);
    if (!IsValid(mount)) { return; }

    weapon->Mount(mount);
}
void UTPSEquipmentManager::EquipToPrimaryWeaponHand(ATPSEquipableItem* weapon)
{
    if (!IsValid(weapon) || !IsValid(PrimaryWeaponHand)) { return; }

	weapon->Mount(PrimaryWeaponHand);
    UE_LOG(LogTemp, Log, TEXT("Weapon[%s] mounted to Primary Hand."), *weapon->GetName());
}

void UTPSEquipmentManager::EquipToSecondaryWeaponHand(ATPSEquipableItem* weapon)
{
    if (!IsValid(weapon) || !IsValid(SecondaryWeaponHand)) { return; }

    weapon->Mount(SecondaryWeaponHand);
    UE_LOG(LogTemp, Log, TEXT("Weapon[%s] mounted to Secondary Hand."), *weapon->GetName());

}

void UTPSEquipmentManager::EquipToPrimaryHolster(ATPSEquipableItem* weapon)
{
    EquipWeaponToHolster(weapon, PrimaryWeapon);
}
void UTPSEquipmentManager::EquipToSecondaryHolster(ATPSEquipableItem* weapon)
{
    EquipWeaponToHolster(weapon, SecondaryWeapon);
}
void UTPSEquipmentManager::EquipToTacticalHolster(ATPSEquipableItem* weapon)
{
    EquipWeaponToHolster(weapon, TacticalEquipment);
}
void UTPSEquipmentManager::EquipToLethalHolster(ATPSEquipableItem* weapon)
{
    EquipWeaponToHolster(weapon, LethalEquipment);
}
void UTPSEquipmentManager::EquipToBackHolster(ATPSEquipableItem* weapon)
{
    EquipWeaponToHolster(weapon, TertiaryWeapon);
}


//~ ============================================================= ~//
//  PUBLIC OPERATIONS
//~ ============================================================= ~//

void UTPSEquipmentManager::EquipItemToSlot(ATPSEquipableItem* equipmentItem, ETPSEquipmentSlot slot)
{
    switch (slot)
    {
    case PrimaryWeapon:
        PrimaryWeaponInstance = equipmentItem;
        break;
    case SecondaryWeapon:
        SecondaryWeaponInstance = equipmentItem;
        break;
    case TertiaryWeapon:
        TertiaryWeaponInstance = equipmentItem;
        break;
    case LethalEquipment:
        LethalEquipmentInstance = equipmentItem;
        break;
    case TacticalEquipment:
        TacticalEquipmentInstance = equipmentItem;
        break;
    }

    equipmentItem->OwnerAsc = OwnerAsc;
    EquipWeaponToHolster(equipmentItem, slot);
}

void UTPSEquipmentManager::DropEquipmentFromSlot(ETPSEquipmentSlot slot)
{
    ATPSEquipableItem* item = nullptr;
    switch (slot)
    {
    case PrimaryWeapon:
        item = PrimaryWeaponInstance;
        PrimaryWeaponInstance = nullptr;
        break;
    case SecondaryWeapon:
        item = SecondaryWeaponInstance;
        SecondaryWeaponInstance = nullptr;
        break;
    case TertiaryWeapon:
        item = TertiaryWeaponInstance;
        TertiaryWeaponInstance = nullptr;
        break;
    case LethalEquipment:
        item = LethalEquipmentInstance;
        LethalEquipmentInstance = nullptr;
        break;
    case TacticalEquipment:
        item = TacticalEquipmentInstance;
        TacticalEquipmentInstance = nullptr;
        break;
    }

    if (IsValid(item))
    {
        item->Drop();
    }
}
void UTPSEquipmentManager::DropAll()
{
    DropEquipmentFromSlot(PrimaryWeapon);
    DropEquipmentFromSlot(SecondaryWeapon);
    DropEquipmentFromSlot(TertiaryWeapon);
    DropEquipmentFromSlot(LethalEquipment);
    DropEquipmentFromSlot(TacticalEquipment);
}



void UTPSEquipmentManager::DestroyItemAtSlot(ETPSEquipmentSlot slot)
{
    ATPSEquipableItem* item = nullptr;
    switch (slot)
    {
    case PrimaryWeapon:
        item = PrimaryWeaponInstance;
        PrimaryWeaponInstance = nullptr;
        break;
    case SecondaryWeapon:
        item = SecondaryWeaponInstance;
        SecondaryWeaponInstance = nullptr;
        break;
    case TertiaryWeapon:
        item = TertiaryWeaponInstance;
        TertiaryWeaponInstance = nullptr;
        break;
    case LethalEquipment:
        item = LethalEquipmentInstance;
        LethalEquipmentInstance = nullptr;
        break;
    case TacticalEquipment:
        item = TacticalEquipmentInstance;
        TacticalEquipmentInstance = nullptr;
        break;
    }

    if (IsValid(item))
    {
        item->Destroy();
    }
}
void UTPSEquipmentManager::DestroyAll()
{
    DestroyItemAtSlot(PrimaryWeapon);
    DestroyItemAtSlot(SecondaryWeapon);
    DestroyItemAtSlot(TertiaryWeapon);
    DestroyItemAtSlot(LethalEquipment);
    DestroyItemAtSlot(TacticalEquipment);
}





void UTPSEquipmentManager::Ready()
{
    if (!IsValid(Loadout)) { return; }

    if (ActiveEquipmentSlot == None)
    {
        EquipAndArm(PrimaryWeapon);
    }

    /*weaponController.activeWeapons.Add(Loadout->primaryWeapon);
    weaponController.Ready();*/
}

void UTPSEquipmentManager::UnReady()
{
    if (!IsValid(Loadout)) { return; }
    /*weaponController.CancelReady();
    weaponController.activeWeapons.Clear();*/

    EquipToPrimaryHolster(GetItemFromEquipmentSlot(PrimaryWeapon));
    EquipToSecondaryHolster(GetItemFromEquipmentSlot(SecondaryWeapon));
    EquipToBackHolster(GetItemFromEquipmentSlot(TertiaryWeapon));

    EquipToLethalHolster(GetItemFromEquipmentSlot(LethalEquipment));
    EquipToTacticalHolster(GetItemFromEquipmentSlot(TacticalEquipment));
}

void UTPSEquipmentManager::EquipPrimary()
{
    UnEquipActive();
    EquipAndArm(ETPSEquipmentSlot::PrimaryWeapon);
}
void UTPSEquipmentManager::EquipSecondary()
{
    UnEquipActive();
    EquipAndArm(ETPSEquipmentSlot::SecondaryWeapon);
}
void UTPSEquipmentManager::EquipTertiary() {
    UnEquipActive();
    EquipAndArm(ETPSEquipmentSlot::TertiaryWeapon);
}
void UTPSEquipmentManager::EquipLethalThrowable() {
    UnEquipActive();
    EquipAndArm(ETPSEquipmentSlot::LethalEquipment);
}
void UTPSEquipmentManager::EquipTacticalThrowable() {
    UnEquipActive();
    EquipAndArm(ETPSEquipmentSlot::TacticalEquipment);
}

void UTPSEquipmentManager::UnEquipActive() {
    if (ActiveEquipmentSlot == ETPSEquipmentSlot::None) { return; }

    ATPSEquipableItem* item = GetItemFromEquipmentSlot(ActiveEquipmentSlot);
    if (IsValid(item)) {
        item->UnEquip();
        EquipWeaponToHolster(item, ActiveEquipmentSlot);
    }

    //weaponController.activeWeapons.Clear();
    ActiveEquipmentSlot = ETPSEquipmentSlot::None;
}

void UTPSEquipmentManager::EquipAndArm(ETPSEquipmentSlot equipmentSlot) {
    ATPSEquipableItem* item = GetItemFromEquipmentSlot(equipmentSlot);

    if (item) {
        EquipToPrimaryWeaponHand(item);
        //weaponController.activeWeapons.Add(weapon);
        ActiveEquipmentSlot = equipmentSlot;
        item->Equip();

        /*UAbilitySystemComponent* asc = OwningCharacter->GetAbilitySystemComponent();
        for (TSubclassOf<UGameplayEffect*> effect : item->AppliedEffects)
        {
            asc->ApplyGameplayEffectToSelf(effect.Get(), 1, FGameplayEffectContextHandle::Get(),
            FPredictionKey::Base);
        }*/
    }
}


void UTPSEquipmentManager::WeaponSwap() {
    UE_LOG(LogTemp, Log, TEXT("Swapping Weapons..."));

    // Needs to acct for non-populated slots.

    /*int nextSlot = ((int)ActiveEquipmentSlot) + 1 % 5;

    UnEquipActive();
    EquipAndArm((ETPSEquipmentSlot) nextSlot);*/
}

void UTPSEquipmentManager::EquipmentSwap() {
    UE_LOG(LogTemp, Log, TEXT("Swapping Equipment..."));
}

ATPSEquipableItem* UTPSEquipmentManager::GetItemFromEquipmentSlot(ETPSEquipmentSlot slot) const
{
    switch (slot)
    {
    case ETPSEquipmentSlot::PrimaryWeapon:
        return PrimaryWeaponInstance;
    case ETPSEquipmentSlot::SecondaryWeapon:
        return SecondaryWeaponInstance;
    case ETPSEquipmentSlot::TertiaryWeapon:
        return TertiaryWeaponInstance;
    case ETPSEquipmentSlot::LethalEquipment:
        return LethalEquipmentInstance;
    case ETPSEquipmentSlot::TacticalEquipment:
        return TacticalEquipmentInstance;
    }
    return nullptr;
}

UTPSMountPoint* UTPSEquipmentManager::GetMountPointForSlot(ETPSEquipmentSlot slot) const
{
    UTPSMountPoint* mount = nullptr;
    switch (slot)
    {
    case PrimaryWeapon:
        mount = RightLegHolster;
        break;
    case SecondaryWeapon:
        mount = RightHipHolster;
        break;
    case TertiaryWeapon:
        mount = BackHolster;
        break;
    case LethalEquipment:
        mount = LeftLegHolster;
        break;
    case TacticalEquipment:
        mount = LeftHipHolster;
        break;
    }
    return mount;
}