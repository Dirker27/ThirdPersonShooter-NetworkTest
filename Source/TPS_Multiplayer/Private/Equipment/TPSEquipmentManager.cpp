// (C) ToasterCat Studios 2025

#include "Equipment/TPSEquipmentManager.h"

#include "Net/UnrealNetwork.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffect.h"

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
        EquipWeapon(ActiveEquipmentSlot);
    }
}

// Should be called in consumer's constructor.
void UTPSEquipmentManager::BindToMesh(USkeletalMeshComponent* mesh)
{
    TargetMesh = mesh;

    if (IsValid(mesh)) {
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
    ApplyPassiveItemEffectsToOwner(weapon);

    if (slot == ActiveEquipmentSlot)
    {
        EquipWeapon(ActiveEquipmentSlot);
    }
    else
    {
        HolsterWeapon(slot);
    }
}

void UTPSEquipmentManager::PickUpGearItem(ATPSGearItem* gearItem, const ETPSGearSlot slot)
{
    if (!IsValid(gearItem)) { return; }

    ATPSEquipableItem* existingItem = GetGearItem(slot);
    if (IsValid(existingItem))
    {
        DropGearItemFromSlot(slot);
    }

    GearItems[slot] = gearItem;

    gearItem->Pickup();
    gearItem->OwnerAsc = OwnerAsc;
    ApplyPassiveItemEffectsToOwner(gearItem);

    gearItem->Mount(GetHarnessMountPointForGearSlot(slot));
}





void UTPSEquipmentManager::DropWeaponFromSlot(const ETPSWeaponSlot slot)
{
    ATPSEquipableItem* weapon = GetWeapon(slot);

    if (IsValid(weapon)) {
        weapon->Drop();
        RemoveActiveItemEffectsFromOwner(weapon);
        RemovePassiveItemEffectsFromOwner(weapon);
    }
    WeaponItems[slot] = nullptr;
}
void UTPSEquipmentManager::DropGearItemFromSlot(const ETPSGearSlot slot)
{
    ATPSEquipableItem* item = GetGearItem(slot);

    if (IsValid(item)) {
        item->Drop();
        RemoveActiveItemEffectsFromOwner(item);
        RemovePassiveItemEffectsFromOwner(item);
    }
    GearItems[slot] = nullptr;

    if (ActiveEquipmentSlot == slot)
    {
        ActiveEquipmentSlot = None;
    }
}
void UTPSEquipmentManager::DropAll()
{
    for (int slotIndex = 0; slotIndex < WEAPON_SLOT_SIZE; slotIndex++)
    {
        ATPSWeapon* weapon = WeaponItems[slotIndex];
        if (IsValid(weapon))
        {
            weapon->Drop();
            RemoveActiveItemEffectsFromOwner(weapon);
            RemovePassiveItemEffectsFromOwner(weapon);
        }
        WeaponItems[slotIndex] = nullptr;
    }

    for (int slotIndex = 0; slotIndex < GEAR_SLOT_SIZE; slotIndex++)
    {
        ATPSGearItem* item = GearItems[slotIndex];
        if (IsValid(item)) {
            item->Drop();
            RemoveActiveItemEffectsFromOwner(item);
            RemovePassiveItemEffectsFromOwner(item);
        }
        GearItems[slotIndex] = nullptr;
    }
}





void UTPSEquipmentManager::DestroyWeaponAtSlot(const ETPSWeaponSlot slot)
{
    ATPSEquipableItem* item = GetWeapon(slot);

    if (IsValid(item))
    {
        item->Destroy();
        RemoveActiveItemEffectsFromOwner(item);
        RemovePassiveItemEffectsFromOwner(item);
    }
    WeaponItems[slot] = nullptr;

    if (ActiveEquipmentSlot == slot)
    {
        ActiveEquipmentSlot = None;
    }
}
void UTPSEquipmentManager::DestroyGearItemAtSlot(const ETPSGearSlot slot)
{
    ATPSEquipableItem* item = GetGearItem(slot);

    if (IsValid(item))
    {
        item->Destroy();
        RemoveActiveItemEffectsFromOwner(item);
        RemovePassiveItemEffectsFromOwner(item);
    }
    GearItems[slot] = nullptr;
}
void UTPSEquipmentManager::DestroyAll()
{
    for (int slotIndex = 0; slotIndex < WEAPON_SLOT_SIZE; slotIndex++)
    {
        ATPSWeapon* weapon = WeaponItems[slotIndex];
        if (IsValid(weapon))
        {
            weapon->Destroy();
            RemoveActiveItemEffectsFromOwner(weapon);
            RemovePassiveItemEffectsFromOwner(weapon);
        }
        WeaponItems[slotIndex] = nullptr;
    }

    for (int slotIndex = 0; slotIndex < GEAR_SLOT_SIZE; slotIndex++)
    {
        ATPSGearItem* item = GearItems[slotIndex];
        if (IsValid(item)) {
            item->Destroy();
            RemoveActiveItemEffectsFromOwner(item);
            RemovePassiveItemEffectsFromOwner(item);
        }
        GearItems[slotIndex] = nullptr;
    }
}





void UTPSEquipmentManager::Ready()
{
    if (ActiveEquipmentSlot == None)
    {
        ActiveEquipmentSlot = PrimaryWeapon;
    }

    EquipWeapon(ActiveEquipmentSlot);
    IsReadied = true;
}

void UTPSEquipmentManager::UnReady()
{
    ATPSEquipableItem* activeItem = GetWeapon(ActiveEquipmentSlot);

    HolsterWeapon(ActiveEquipmentSlot);
    IsReadied = false;
}





void UTPSEquipmentManager::EquipWeapon(ETPSWeaponSlot equipmentSlot) {
    ATPSEquipableItem* item = GetWeapon(equipmentSlot);
    if (!IsValid(item)) { return; }

    if (equipmentSlot != ActiveEquipmentSlot)
    {
        HolsterWeapon(ActiveEquipmentSlot);
    }
    ActiveEquipmentSlot = equipmentSlot;

    ApplyActiveItemEffectsToOwner(item);

    EquipToPrimaryWeaponHand(item);
    item->Equip();
}

void UTPSEquipmentManager::HolsterWeapon(const ETPSWeaponSlot slot)
{
    ATPSEquipableItem* item = GetWeapon(slot);
    UTPSMountPoint* mount = GetHarnessMountPointForEquipmentSlot(slot);
    if (!IsValid(mount) || !IsValid(item)) { return; }

    RemoveActiveItemEffectsFromOwner(item);

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
}




void UTPSEquipmentManager::ApplyActiveItemEffectsToOwner(ATPSEquipableItem* item)
{
    for (auto effect : item->ActiveEffects)
    {
        FActiveGameplayEffectHandle handle = OwnerAsc->ApplyGameplayEffectToSelf(
            effect->GetDefaultObject<UGameplayEffect>(),
            1,
            OwnerAsc->MakeEffectContext());

        item->AppliedActiveEffectHandles.Add(handle);
    }
}
void UTPSEquipmentManager::ApplyPassiveItemEffectsToOwner(ATPSEquipableItem* item)
{
    for (auto effect : item->PassiveEffects)
    {
        FActiveGameplayEffectHandle handle = OwnerAsc->ApplyGameplayEffectToSelf(
            effect->GetDefaultObject<UGameplayEffect>(),
            1,
            OwnerAsc->MakeEffectContext());

        item->AppliedPassiveEffectHandles.Add(handle);
    }
}
void UTPSEquipmentManager::RemoveActiveItemEffectsFromOwner(ATPSEquipableItem* item)
{
    for (auto effectHandle : item->AppliedActiveEffectHandles)
    {
        OwnerAsc->RemoveActiveGameplayEffect(effectHandle);
    }
    item->AppliedActiveEffectHandles.Empty();
}
void UTPSEquipmentManager::RemovePassiveItemEffectsFromOwner(ATPSEquipableItem* item)
{
    for (auto effectHandle : item->AppliedPassiveEffectHandles)
    {
        OwnerAsc->RemoveActiveGameplayEffect(effectHandle);
    }
    item->AppliedPassiveEffectHandles.Empty();
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




ATPSWeapon* UTPSEquipmentManager::GetWeapon(const ETPSWeaponSlot slot)
{
    return WeaponItems[slot];
}

ATPSGearItem* UTPSEquipmentManager::GetGearItem(const ETPSGearSlot slot)
{
    return GearItems[slot];
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
