// (C) ToasterCat Studios 2025

#include "Equipment/TPSEquipmentManager.h"

#include "Net/UnrealNetwork.h"

#include "Behavior/TPSMountPoint.h"
#include "Character/TPSCharacter.h"

UTPSEquipmentManager::UTPSEquipmentManager(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    //Loadout = CreateDefaultSubobject<UTPSEquipmentLoadout>(TEXT("DefaultLoadout"));

    HarnessSocketMap.Add(PrimaryHand, "hand_r");
    HarnessSocketMap.Add(SecondaryHand, "hand_l");
    HarnessSocketMap.Add(LegHolster_Left, "thigh_l");
    HarnessSocketMap.Add(LegHolster_Right, "thigh_r");
    HarnessSocketMap.Add(ChestHolster_Pistol, "spine_05");
    HarnessSocketMap.Add(ChestHolster_Rifle, "spine_03");
    HarnessSocketMap.Add(BackHolster_Left, "spine_05");
    HarnessSocketMap.Add(BackHolster_Cross, "spine_05");
    HarnessSocketMap.Add(WeaponBelt_Front, "pelvis");
    HarnessSocketMap.Add(WeaponBelt_Rear, "pelvis");
    HarnessSocketMap.Add(WeaponBelt_Left, "pelvis");
    HarnessSocketMap.Add(WeaponBelt_Right, "pelvis");

    EquipmentHolsterMap.Add(PrimaryWeapon, ChestHolster_Rifle);
    EquipmentHolsterMap.Add(SecondaryWeapon, LegHolster_Right);
    EquipmentHolsterMap.Add(TertiaryWeapon, BackHolster_Left);
    EquipmentHolsterMap.Add(LethalEquipment, WeaponBelt_Left);
    EquipmentHolsterMap.Add(TacticalEquipment, WeaponBelt_Rear);

    for (auto harnessSocket : HarnessSocketMap)
    {
        FString prefix = "MP-";
        HarnessMountPointMap.Add(
            harnessSocket.Key,
            CreateDefaultSubobject<UTPSMountPoint>(
                FName(prefix.Append(ETPSEquipmentHarnessSlotToString(harnessSocket.Key))))
        );
    }

    /*PrimaryWeaponHand = CreateDefaultSubobject<UTPSMountPoint>(TEXT("MP-PrimaryWeaponHand"));
    SecondaryWeaponHand = CreateDefaultSubobject<UTPSMountPoint>(TEXT("MP-SecondaryWeaponHand"));

    BackHolster = CreateDefaultSubobject<UTPSMountPoint>(TEXT("MP-BackHolster"));
    LeftHipHolster = CreateDefaultSubobject<UTPSMountPoint>(TEXT("MP-LeftHipHolster"));
    LeftLegHolster = CreateDefaultSubobject<UTPSMountPoint>(TEXT("MP-LeftLegHolster"));
    RightHipHolster = CreateDefaultSubobject<UTPSMountPoint>(TEXT("MP-RightHipHolster"));
    RightLegHolster = CreateDefaultSubobject<UTPSMountPoint>(TEXT("MP-RightLegHolster"));*/

    /*PrimaryWeaponHandBone = FName(TEXT("hand_r"));
    SecondaryWeaponHandBone = FName(TEXT("hand_l"));

    BackHolsterBone = FName(TEXT("spine_05"));
    LeftHipHolsterBone = FName(TEXT("thigh_l"));
    LeftLegHolsterBone = FName(TEXT("calf_l"));
    RightHipHolsterBone = FName(TEXT("thigh_r"));
    // Actually the Front-Chest holster for now
    RightLegHolsterBone = FName(TEXT("spine_04"));*/
}

void UTPSEquipmentManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UTPSEquipmentManager, ActiveEquipmentSlot);

    DOREPLIFETIME(UTPSEquipmentManager, EquipmentMap);

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

    UE_LOG(LogTemp, Log, TEXT("Destroying EquipmentManager sub-component and all managed Equipment items..."));
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

// Should be called in consumer's constructor.
void UTPSEquipmentManager::BindToMesh(USkeletalMeshComponent* mesh)
{
    TargetMesh = mesh;

    /*PrimaryWeaponHand->Target.ParentComponent = TargetMesh;
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
    RightLegHolster->TargetSocketName = FName(TEXT("calf_r"));*/

    if (IsValid(mesh)) {
        UE_LOG(LogTemp, Log, TEXT("Mounting Holsters..."));

        for (auto entry : HarnessMountPointMap)
        {
            entry.Value->Attach(mesh, *HarnessSocketMap.Find(entry.Key));
        }

        // ---

        /*
        if (IsValid(PrimaryWeaponHand) && !PrimaryWeaponHandBone.IsNone())
        {
            PrimaryWeaponHand->Attach(mesh, PrimaryWeaponHandBone);
        }
        if (IsValid(SecondaryWeaponHand) && !SecondaryWeaponHandBone.IsNone())
        {
            SecondaryWeaponHand->Attach(mesh, SecondaryWeaponHandBone);
        }

        if (IsValid(BackHolster) && !BackHolsterBone.IsNone())
        {
            BackHolster->Attach(mesh, BackHolsterBone);
        }
        if (IsValid(LeftHipHolster) && !LeftHipHolsterBone.IsNone())
        {
            LeftHipHolster->Attach(mesh, LeftHipHolsterBone);
        }
        if (IsValid(LeftLegHolster) && !LeftLegHolsterBone.IsNone())
        {
            LeftLegHolster->Attach(mesh, LeftLegHolsterBone);
        }
        if (IsValid(RightHipHolster) && !RightHipHolsterBone.IsNone())
        {
            RightHipHolster->Attach(mesh, RightHipHolsterBone);
        }
        if (IsValid(RightLegHolster) && !RightLegHolsterBone.IsNone())
        {
            RightLegHolster->Attach(mesh, RightLegHolsterBone);
        }
        */
    }
}

void UTPSEquipmentManager::BindToOwnerAbilitySystem(UAbilitySystemComponent* ownerAsc)
{
    OwnerAsc = ownerAsc;
    for (auto equipment : EquipmentMap)
    {
        equipment.Value->OwnerAsc = ownerAsc;
    }
}



//~ ============================================================= ~//
//  EQUIPMENT OPERATIONS
//~ ============================================================= ~//

void UTPSEquipmentManager::InstantiateLoadout()
{
    if (!IsValid(Loadout)) { return; }

    if (IsValid(Loadout->PrimaryWeapon)) {
        UE_LOG(LogTemp, Log, TEXT("Instantiating Primary Weapon [%s]..."), *Loadout->PrimaryWeapon->GetName());
        InstantiateAndAssignEquipmentToSlot(Loadout->PrimaryWeapon, PrimaryWeapon);
    }
    if (IsValid(Loadout->SecondaryWeapon)) {
        UE_LOG(LogTemp, Log, TEXT("Instantiating Secondary Weapon [%s]..."), *Loadout->SecondaryWeapon->GetName());
        InstantiateAndAssignEquipmentToSlot(Loadout->SecondaryWeapon, SecondaryWeapon);
    }
    if (IsValid(Loadout->TertiaryWeapon)) {
        UE_LOG(LogTemp, Log, TEXT("Instantiating Tertiary Weapon [%s]..."), *Loadout->TertiaryWeapon->GetName());
        InstantiateAndAssignEquipmentToSlot(Loadout->TertiaryWeapon, TertiaryWeapon);
    }
    if (IsValid(Loadout->TacticalEquipment)) {
        UE_LOG(LogTemp, Log, TEXT("Instantiating Tactical Equipment [%s]..."), *Loadout->TacticalEquipment->GetName());
        InstantiateAndAssignEquipmentToSlot(Loadout->TacticalEquipment, TacticalEquipment);
    }
    if (IsValid(Loadout->LethalEquipment)) {
        UE_LOG(LogTemp, Log, TEXT("Instantiating Lethal Equipment [%s]..."), *Loadout->LethalEquipment->GetName());
        InstantiateAndAssignEquipmentToSlot(Loadout->LethalEquipment, LethalEquipment);
    }
}

void UTPSEquipmentManager::InstantiateAndAssignEquipmentToSlot(TSubclassOf<ATPSEquipableItem> templ, ETPSEquipmentSlot slot)
{
    ATPSEquipableItem* item = GetWorld()->SpawnActor<ATPSEquipableItem>(templ);
    PickupAndAssignEquipmentToSlot(item, slot);
}


void UTPSEquipmentManager::EquipWeaponToHolster(ATPSEquipableItem* weapon, ETPSEquipmentSlot slot)
{
    if (!IsValid(weapon)) { return; }

    UTPSMountPoint* mount = GetHolsterMountPointForEquipmentSlot(slot);
    if (!IsValid(mount)) { return; }

    weapon->Mount(mount);
    weapon->UnEquip();
}
void UTPSEquipmentManager::EquipToPrimaryWeaponHand(ATPSEquipableItem* weapon)
{
    if (!IsValid(weapon)) { return; }

    UTPSMountPoint* mp = *HarnessMountPointMap.Find(PrimaryHand);
    if (!IsValid(mp)) { return; }

	weapon->Mount(mp);
    weapon->Equip();
    UE_LOG(LogTemp, Log, TEXT("Weapon[%s] mounted to Primary Hand."), *weapon->GetName());
}

void UTPSEquipmentManager::EquipToSecondaryWeaponHand(ATPSEquipableItem* weapon)
{
    if (!IsValid(weapon)) { return; }

    UTPSMountPoint* mp = *HarnessMountPointMap.Find(SecondaryHand);
    if (!IsValid(mp)) { return; }

    weapon->Mount(mp);
    weapon->Equip();
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

void UTPSEquipmentManager::PickupAndAssignEquipmentToSlot(ATPSEquipableItem* equipmentItem, ETPSEquipmentSlot slot)
{
    ATPSEquipableItem* existingItem = GetItemFromEquipmentSlot(slot);
    if (IsValid(existingItem))
    {
        DropEquipmentFromSlot(slot);
    }

    /*
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
    }*/
    EquipmentMap.Add(slot, equipmentItem);

    equipmentItem->Pickup();
    equipmentItem->OwnerAsc = OwnerAsc;
    EquipWeaponToHolster(equipmentItem, slot);
}

void UTPSEquipmentManager::DropEquipmentFromSlot(ETPSEquipmentSlot slot)
{
    ATPSEquipableItem* item = *EquipmentMap.Find(slot);
    if (item == nullptr) { return; }

    item->Drop();
    EquipmentMap.Remove(slot);

    /*
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
    }*/
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

    /*
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
    */

    if (IsValid(item))
    {
        item->Destroy();
        EquipmentMap.Remove(slot);
    }
}
void UTPSEquipmentManager::DestroyAll()
{
    DestroyItemAtSlot(PrimaryWeapon);
    DestroyItemAtSlot(SecondaryWeapon);
    DestroyItemAtSlot(TertiaryWeapon);
    DestroyItemAtSlot(LethalEquipment);
    DestroyItemAtSlot(TacticalEquipment);

    for (auto EquipmentSlot : EquipmentMap)
    {
        EquipmentSlot.Value->Destroy();
    }
    EquipmentMap.Empty();
}





void UTPSEquipmentManager::Ready()
{
    //if (!IsValid(Loadout)) { return; }

    if (ActiveEquipmentSlot == None)
    {
        EquipAndArm(PrimaryWeapon);
    }
}

void UTPSEquipmentManager::UnReady()
{
    //if (!IsValid(Loadout)) { return; }
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

ATPSEquipableItem* UTPSEquipmentManager::GetItemFromEquipmentSlot(ETPSEquipmentSlot slot)
{
    ATPSEquipableItem** item = EquipmentMap.Find(slot);

    if (item != nullptr)
    {
        return *item;
    }
    return nullptr;
    /*
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
    }*/
    //return nullptr;
}

UTPSMountPoint* UTPSEquipmentManager::GetHolsterMountPointForEquipmentSlot(ETPSEquipmentSlot slot)
{
    ETPSEquipmentHarnessSlot s = *EquipmentHolsterMap.Find(slot);
    UTPSMountPoint** mp = HarnessMountPointMap.Find(s);

    if (mp != nullptr)
    {
        return *mp;
    }
    return nullptr;

    /*
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
    return mount;*/
}