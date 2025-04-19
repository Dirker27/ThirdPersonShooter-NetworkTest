// (C) ToasterCat Studios 2025

#include "Equipment/TPSEquipmentManager.h"

#include "Net/UnrealNetwork.h"

#include "Behavior/TPSMountPoint.h"
#include "Character/TPSCharacter.h"
#include "Equipment/TPSGearItem.h"

UTPSEquipmentManager::UTPSEquipmentManager(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    //Loadout = CreateDefaultSubobject<UTPSEquipmentLoadout>(TEXT("DefaultLoadout"));

    ConfigureHarnessSlots();

    for (auto harnessSocket : HarnessSocketMap)
    {
        FString prefix = "MP-";
        HarnessMountPointMap.Add(
            harnessSocket.Key,
            CreateDefaultSubobject<UTPSMountPoint>(
                FName(prefix.Append(ETPSEquipmentHarnessSlotToString(harnessSocket.Key))))
        );
    }

    WeaponItems.SetNum(WEAPON_SLOT_SIZE);
    GearItems.SetNum(GEAR_SLOT_SIZE);
}

void UTPSEquipmentManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UTPSEquipmentManager, ActiveEquipmentSlot);

    DOREPLIFETIME(UTPSEquipmentManager, WeaponItems);
    DOREPLIFETIME(UTPSEquipmentManager, GearItems);
}

void UTPSEquipmentManager::BeginPlay()
{
    Super::BeginPlay();

    SetIsReplicated(true);

    // Re-bind with BP overrides.
    //BindToMesh(TargetMesh.Get());
}


void UTPSEquipmentManager::Initialize()
{
    if (IsValid(Loadout)) {
        InstantiateLoadout();
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
            FName* socket = HarnessSocketMap.Find(entry.Key);
            if (socket != nullptr) {
                entry.Value->BindToParentSocket(mesh, *HarnessSocketMap.Find(entry.Key));
            } else
            {
                UE_LOG(LogTemp, Log, TEXT("FAILED to bind socket for slot[%hs]..."),
                    ETPSEquipmentHarnessSlotToString(entry.Key));
            }
        }
    }
}

void UTPSEquipmentManager::BindToOwnerAbilitySystem(UAbilitySystemComponent* ownerAsc)
{
    OwnerAsc = ownerAsc;

    for (auto weapon : WeaponItems)
    {
        if (weapon != nullptr)
        {
            weapon->OwnerAsc = ownerAsc;
        }
    }

    for (auto gear : GearItems)
    {
        if (gear != nullptr)
        {
            gear->OwnerAsc = ownerAsc;
        }
    }
}



//~ ======================================================================== ~//
//  EQUIPMENT OPERATIONS
//~ ======================================================================== ~//

void UTPSEquipmentManager::InstantiateLoadout()
{
    if (!IsValid(Loadout)) { return; }

    for (auto entry : Loadout->WeaponsBySlot)
    {
        if (IsValid(entry.Value)) {
            UE_LOG(LogTemp, Log, TEXT("Instantiating Weapon[%hs]..."), ETPSWeaponSlotToString(entry.Key));
            InstantiateWeaponAndAssignToSlot(entry.Value, entry.Key);
        }
    }

    for (auto entry : Loadout->GearBySlot)
    {
        if (IsValid(entry.Value)) {
            UE_LOG(LogTemp, Log, TEXT("Instantiating Gear[%hs]..."), ETPSGearSlotToString(entry.Key));
            InstantiateGearItemAndAssignToSlot(entry.Value, entry.Key);
        }
    }
}



void UTPSEquipmentManager::InstantiateWeaponAndAssignToSlot(TSubclassOf<ATPSWeapon> templ, const ETPSWeaponSlot slot)
{
    ATPSWeapon* weapon = GetWorld()->SpawnActor<ATPSWeapon>(templ);
    PickUpWeapon(weapon, slot);
}

void UTPSEquipmentManager::InstantiateGearItemAndAssignToSlot(TSubclassOf<ATPSGearItem> templ, const ETPSGearSlot slot)
{
    ATPSGearItem* item = GetWorld()->SpawnActor<ATPSGearItem>(templ);
    PickUpGearItem(item, slot);
}




void UTPSEquipmentManager::HolsterEquipmentItem(const ETPSWeaponSlot slot)
{
    ATPSEquipableItem* item = GetWeapon(slot);
    UTPSMountPoint* mount = GetHarnessMountPointForEquipmentSlot(slot);
    if (!IsValid(mount) || !IsValid(item)) { return; }

    item->MountWithOffset(mount, item->WeaponHolsterOffset);
    item->UnEquip();
}

void UTPSEquipmentManager::EquipToPrimaryWeaponHand(ATPSEquipableItem* item)
{
    if (!IsValid(item)) { return; }

    UTPSMountPoint* mp = *HarnessMountPointMap.Find(PrimaryHand);
    if (!IsValid(mp)) { return; }

    ATPSWeapon* weapon = Cast<ATPSWeapon>(item);
    if (IsValid(weapon))
    {
        weapon->MountWithOffset(mp, weapon->PrimaryWeaponHandOffset);
    }
    else
    {
        item->Mount(mp);
    }
	
    item->Equip();
    UE_LOG(LogTemp, Log, TEXT("Weapon[%s] mounted to Primary Hand."), *item->GetName());
}



void UTPSEquipmentManager::PickUpWeapon(ATPSWeapon* weapon, const ETPSWeaponSlot slot)
{
    if (!IsValid(weapon)) { return; }

    ATPSEquipableItem* existingItem = GetWeapon(slot);
    if (IsValid(existingItem))
    {
        DropWeaponFromSlot(slot);
    }

    WeaponItems[slot] = weapon;

    weapon->Pickup();
    weapon->OwnerAsc = OwnerAsc;

    if (slot == ActiveEquipmentSlot)
    {
        EquipToPrimaryWeaponHand(weapon);
    }
    else
    {
        HolsterEquipmentItem(slot);
    }
}

void UTPSEquipmentManager::PickUpGearItem(ATPSGearItem* gearItem, const ETPSGearSlot slot)
{
    if (!IsValid(gearItem)) { return; }

    ATPSEquipableItem* existingItem = GetGearItem(slot);
    if (IsValid(existingItem))
    {
        DropGearFromSlot(slot);
    }

    GearItems[slot] = gearItem;

    gearItem->Pickup();
    gearItem->OwnerAsc = OwnerAsc;

    gearItem->Mount(GetHarnessMountPointForGearSlot(slot));
}



void UTPSEquipmentManager::DropWeaponFromSlot(const ETPSWeaponSlot slot)
{
    ATPSEquipableItem* item = GetWeapon(slot);

    if (IsValid(item)) {
        item->Drop();
    }
    WeaponItems[slot] = nullptr;
}
void UTPSEquipmentManager::DropGearFromSlot(const ETPSGearSlot slot)
{
    ATPSEquipableItem* item = GetGearItem(slot);

    if (IsValid(item)) {
        item->Drop();
    }
    GearItems[slot] = nullptr;
}
void UTPSEquipmentManager::DropAll()
{
    for (ATPSWeapon* weapon : WeaponItems)
    {
        if (IsValid(weapon))
        {
            weapon->Drop();
        }
    }
    WeaponItems.Empty(WEAPON_SLOT_SIZE);

    for (ATPSGearItem* item : GearItems)
    {
        if (IsValid(item)) {
            item->Drop();
        }
    }
    GearItems.Empty(GEAR_SLOT_SIZE);
}



void UTPSEquipmentManager::DestroyWeapon(const ETPSWeaponSlot slot)
{
    ATPSEquipableItem* item = GetWeapon(slot);

    if (IsValid(item))
    {
        item->Destroy();
    }
    WeaponItems[slot] = nullptr;
}
void UTPSEquipmentManager::DestroyGearItem(const ETPSGearSlot slot)
{
    ATPSEquipableItem* item = GetGearItem(slot);

    if (IsValid(item))
    {
        item->Destroy();
    }
    GearItems[slot] = nullptr;
}
void UTPSEquipmentManager::DestroyAll()
{
    for (ATPSWeapon* weapon : WeaponItems)
    {
        if (IsValid(weapon))
        {
            weapon->Destroy();
        }
    }
    WeaponItems.Empty(WEAPON_SLOT_SIZE);

    TArray<TEnumAsByte<ETPSGearSlot>> activeGearSlots;
    for (ATPSGearItem* item : GearItems)
    {
        if (IsValid(item)) {
            item->Destroy();
        }
    }
    GearItems.Empty(GEAR_SLOT_SIZE);
}





void UTPSEquipmentManager::Ready()
{
    if (ActiveEquipmentSlot == None)
    {
        ActiveEquipmentSlot = PrimaryWeapon;
    }

    EquipAndArm(ActiveEquipmentSlot);
}

void UTPSEquipmentManager::UnReady()
{
    ATPSEquipableItem* activeItem = GetWeapon(ActiveEquipmentSlot);

    HolsterEquipmentItem(ActiveEquipmentSlot);
}





void UTPSEquipmentManager::EquipAndArm(ETPSWeaponSlot equipmentSlot) {
    ATPSEquipableItem* item = GetWeapon(equipmentSlot);

    if (item) {
        EquipToPrimaryWeaponHand(item);
        ActiveEquipmentSlot = equipmentSlot;
        item->Equip();
    }
}

void UTPSEquipmentManager::UnEquipActive() {
    if (ActiveEquipmentSlot == ETPSWeaponSlot::None) { return; }

    ATPSEquipableItem* item = GetWeapon(ActiveEquipmentSlot);
    if (IsValid(item)) {
        item->UnEquip();
        HolsterEquipmentItem(ActiveEquipmentSlot);
    }

    ActiveEquipmentSlot = ETPSWeaponSlot::None;
}




ATPSWeapon* UTPSEquipmentManager::GetWeapon(const ETPSWeaponSlot slot)
{
    return WeaponItems[slot];
}

ATPSGearItem* UTPSEquipmentManager::GetGearItem(const ETPSGearSlot slot)
{
    return GearItems[slot];
}




void UTPSEquipmentManager::ConfigureHarnessSlots()
{
    HarnessSocketMap.Add(PrimaryHand, "hand_r");
    HarnessSocketMap.Add(SecondaryHand, "hand_l");

    HarnessSocketMap.Add(LegHolster_Left, "thigh_l");
    HarnessSocketMap.Add(LegHolster_Right, "thigh_r");
    HarnessSocketMap.Add(ChestHolster_Pistol, "spine_05");
    HarnessSocketMap.Add(ChestHolster_Rifle, "spine_03");
    HarnessSocketMap.Add(BackHolster_Left, "spine_05");
    HarnessSocketMap.Add(BackHolster_Right, "spine_05");
    HarnessSocketMap.Add(BackHolster_Cross, "spine_05");
    HarnessSocketMap.Add(WeaponBelt_Front, "pelvis");
    HarnessSocketMap.Add(WeaponBelt_Rear, "pelvis");
    HarnessSocketMap.Add(WeaponBelt_Left, "pelvis");
    HarnessSocketMap.Add(WeaponBelt_Right, "pelvis");

    HarnessSocketMap.Add(Headgear_Cap, "head");
    HarnessSocketMap.Add(Headgear_Eyes, "head");
    HarnessSocketMap.Add(Backpack, "spine_05");
    HarnessSocketMap.Add(Chestpiece, "spine_05");
    HarnessSocketMap.Add(Belt, "pelvis");

    EquipmentHarnessMap.Add(PrimaryWeapon, ChestHolster_Rifle);
    EquipmentHarnessMap.Add(SecondaryWeapon, WeaponBelt_Rear);
    EquipmentHarnessMap.Add(TertiaryWeapon, BackHolster_Right);
    EquipmentHarnessMap.Add(LethalEquipment, WeaponBelt_Left);
    EquipmentHarnessMap.Add(TacticalEquipment, WeaponBelt_Right);

    GearHarnessMap.Add(Headgear, Headgear_Cap);
    GearHarnessMap.Add(Eyewear, Headgear_Eyes);
    GearHarnessMap.Add(Vest, Chestpiece);
    GearHarnessMap.Add(Pack, Backpack);
}


UTPSMountPoint* UTPSEquipmentManager::GetHarnessMountPointForEquipmentSlot(const ETPSWeaponSlot slot)
{
    TEnumAsByte<ETPSEquipmentHarnessSlot>* s = EquipmentHarnessMap.Find(slot);
    if (s == nullptr) { return nullptr; }

    UTPSMountPoint** mp = HarnessMountPointMap.Find(*s);

    if (mp != nullptr)
    {
        return *mp;
    }
    return nullptr;
}

UTPSMountPoint* UTPSEquipmentManager::GetHarnessMountPointForGearSlot(const ETPSGearSlot slot)
{
    TEnumAsByte<ETPSEquipmentHarnessSlot>* s = GearHarnessMap.Find(slot);
    if (s == nullptr) { return nullptr; }

    UTPSMountPoint** mp = HarnessMountPointMap.Find(*s);

    if (mp != nullptr)
    {
        return *mp;
    }
    return nullptr;
}
