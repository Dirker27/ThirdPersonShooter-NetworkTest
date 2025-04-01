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
}

void UTPSEquipmentManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UTPSEquipmentManager, ActiveEquipmentSlot);

    DOREPLIFETIME(UTPSEquipmentManager, EquipmentMap);
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

        for (auto entry : EquipmentMap)
        {
            EquipWeaponToHolster(entry.Value, entry.Key);
        }
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


    if (IsValid(mesh)) {
        UE_LOG(LogTemp, Log, TEXT("Mounting Holsters..."));

        for (auto entry : HarnessMountPointMap)
        {
            entry.Value->BindToParentSocket(mesh, *HarnessSocketMap.Find(entry.Key));
        }
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

    for (auto entry : Loadout->EquipmentBySlot)
    {
        if (IsValid(entry.Value)) {
            UE_LOG(LogTemp, Log, TEXT("Instantiating[%hs]..."), ETPSEquipmentSlotToString(entry.Key));
            InstantiateAndAssignEquipmentToSlot(entry.Value, entry.Key);
        }
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

    EquipmentMap.Add(slot, equipmentItem);

    equipmentItem->Pickup();
    equipmentItem->OwnerAsc = OwnerAsc;
    EquipWeaponToHolster(equipmentItem, slot);
}

void UTPSEquipmentManager::DropEquipmentFromSlot(ETPSEquipmentSlot slot)
{
    ATPSEquipableItem* item = *EquipmentMap.Find(slot);
    if (!IsValid(item)) { return; }

    EquipmentMap.Remove(slot);
    item->Drop();
}
void UTPSEquipmentManager::DropAll()
{
    TArray<TEnumAsByte<ETPSEquipmentSlot>> activeSlots;
    EquipmentMap.GetKeys(activeSlots);

    for (ETPSEquipmentSlot slot : activeSlots)
    {
        DropEquipmentFromSlot(slot);
    }
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