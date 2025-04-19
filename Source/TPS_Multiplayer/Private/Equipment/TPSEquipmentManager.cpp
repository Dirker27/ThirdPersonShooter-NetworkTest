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

    GearItems.SetNum(GEAR_SLOT_SIZE);
}

void UTPSEquipmentManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UTPSEquipmentManager, ActiveEquipmentSlot);

    DOREPLIFETIME(UTPSEquipmentManager, PrimaryWeaponInstance);
    DOREPLIFETIME(UTPSEquipmentManager, SecondaryWeaponInstance);
    DOREPLIFETIME(UTPSEquipmentManager, TertiaryWeaponInstance);
    DOREPLIFETIME(UTPSEquipmentManager, LethalEquipmentInstance);
    DOREPLIFETIME(UTPSEquipmentManager, TacticalEquipmentInstance);

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
    for (auto equipmentSlot : PopulatedEquipmentSlots)
    {
        ATPSEquipableItem* item = GetItemFromEquipmentSlot(equipmentSlot);
        item->OwnerAsc = ownerAsc;
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

    for (auto entry : Loadout->EquipmentBySlot)
    {
        if (IsValid(entry.Value)) {
            UE_LOG(LogTemp, Log, TEXT("Instantiating Equipment[%hs]..."), ETPSEquipmentSlotToString(entry.Key));
            InstantiateAndAssignEquipmentItemToSlot(entry.Value, entry.Key);
        }
    }

    for (auto entry : Loadout->GearBySlot)
    {
        if (IsValid(entry.Value)) {
            UE_LOG(LogTemp, Log, TEXT("Instantiating Gear[%hs]..."), ETPSGearSlotToString(entry.Key));
            InstantiateAndAssignGearItemToSlot(entry.Value, entry.Key);
        }
    }
}



void UTPSEquipmentManager::InstantiateAndAssignEquipmentItemToSlot(TSubclassOf<ATPSEquipableItem> templ, const ETPSEquipmentSlot slot)
{
    ATPSEquipableItem* item = GetWorld()->SpawnActor<ATPSEquipableItem>(templ);
    PickUpEquipmentItem(item, slot);
}

void UTPSEquipmentManager::InstantiateAndAssignGearItemToSlot(TSubclassOf<ATPSEquipableItem> templ, const ETPSGearSlot slot)
{
    ATPSGearItem* item = GetWorld()->SpawnActor<ATPSGearItem>(templ);
    PickUpGearItem(item, slot);
}




void UTPSEquipmentManager::HolsterEquipmentItem(const ETPSEquipmentSlot slot)
{
    ATPSEquipableItem* item = GetItemFromEquipmentSlot(slot);
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



void UTPSEquipmentManager::PickUpEquipmentItem(ATPSEquipableItem* equipmentItem, const ETPSEquipmentSlot slot)
{
    if (!IsValid(equipmentItem)) { return; }

    ATPSEquipableItem* existingItem = GetItemFromEquipmentSlot(slot);
    if (IsValid(existingItem))
    {
        DropEquipmentItem(slot);
    }

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
    PopulatedEquipmentSlots.Add(slot);

    equipmentItem->Pickup();
    equipmentItem->OwnerAsc = OwnerAsc;

    if (slot == ActiveEquipmentSlot)
    {
        EquipToPrimaryWeaponHand(equipmentItem);
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
        DropGearItem(slot);
    }

    GearItems[slot] = gearItem;

    gearItem->Pickup();
    gearItem->OwnerAsc = OwnerAsc;

    gearItem->Mount(GetHarnessMountPointForGearSlot(slot));
}



void UTPSEquipmentManager::DropEquipmentItem(const ETPSEquipmentSlot slot)
{
    ATPSEquipableItem* item = GetItemFromEquipmentSlot(slot);

    if (IsValid(item)) {
        PopulatedEquipmentSlots.Remove(slot);
        item->Drop();
    }
}
void UTPSEquipmentManager::DropGearItem(const ETPSGearSlot slot)
{
    ATPSEquipableItem* item = GetGearItem(slot);

    if (IsValid(item)) {
        GearItems[slot] = nullptr;
        item->Drop();
    }
}
void UTPSEquipmentManager::DropAll()
{
    for (ETPSEquipmentSlot slot : PopulatedEquipmentSlots)
    {
        ATPSEquipableItem* item = GetItemFromEquipmentSlot(slot);
        item->Drop();
    }
    PopulatedEquipmentSlots.Empty();

    TArray<TEnumAsByte<ETPSGearSlot>> activeGearSlots;
    for (ATPSGearItem* item : GearItems)
    {
        if (IsValid(item)) {
            item->Drop();
        }
    }
    GearItems.Empty(GEAR_SLOT_SIZE);
}



void UTPSEquipmentManager::DestroyEquipmentItem(const ETPSEquipmentSlot slot)
{
    ATPSEquipableItem* item = GetItemFromEquipmentSlot(slot);

    if (IsValid(item))
    {
        PopulatedEquipmentSlots.Remove(slot);
        item->Destroy();
    }
}
void UTPSEquipmentManager::DestroyGearItem(const ETPSGearSlot slot)
{
    ATPSEquipableItem* item = GetGearItem(slot);

    if (IsValid(item))
    {
        GearItems[slot] = nullptr;
        item->Destroy();
    }
}
void UTPSEquipmentManager::DestroyAll()
{
    for (ETPSEquipmentSlot slot : PopulatedEquipmentSlots)
    {
        ATPSEquipableItem* item = GetItemFromEquipmentSlot(slot);
        item->Destroy();
    }
    PopulatedEquipmentSlots.Empty();

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
    ATPSEquipableItem* activeItem = GetItemFromEquipmentSlot(ActiveEquipmentSlot);

    HolsterEquipmentItem(ActiveEquipmentSlot);
}





void UTPSEquipmentManager::EquipAndArm(ETPSEquipmentSlot equipmentSlot) {
    ATPSEquipableItem* item = GetItemFromEquipmentSlot(equipmentSlot);

    if (item) {
        EquipToPrimaryWeaponHand(item);
        ActiveEquipmentSlot = equipmentSlot;
        item->Equip();
    }
}

void UTPSEquipmentManager::UnEquipActive() {
    if (ActiveEquipmentSlot == ETPSEquipmentSlot::None) { return; }

    ATPSEquipableItem* item = GetItemFromEquipmentSlot(ActiveEquipmentSlot);
    if (IsValid(item)) {
        item->UnEquip();
        HolsterEquipmentItem(ActiveEquipmentSlot);
    }

    ActiveEquipmentSlot = ETPSEquipmentSlot::None;
}




ATPSEquipableItem* UTPSEquipmentManager::GetItemFromEquipmentSlot(const ETPSEquipmentSlot slot)
{
    switch (slot)
    {
    case PrimaryWeapon:
        return PrimaryWeaponInstance;
    case SecondaryWeapon:
        return SecondaryWeaponInstance;
    case TertiaryWeapon:
        return TertiaryWeaponInstance;
    case LethalEquipment:
        return LethalEquipmentInstance;
    case TacticalEquipment:
        return TacticalEquipmentInstance;
    default:
        return nullptr;
    }
}

ATPSEquipableItem* UTPSEquipmentManager::GetGearItem(const ETPSGearSlot slot)
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


UTPSMountPoint* UTPSEquipmentManager::GetHarnessMountPointForEquipmentSlot(const ETPSEquipmentSlot slot)
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
