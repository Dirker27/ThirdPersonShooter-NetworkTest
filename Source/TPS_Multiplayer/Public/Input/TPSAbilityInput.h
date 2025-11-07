// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"

#include "TPSAbilityInput.generated.h"

UENUM(BlueprintType)
enum class EAbilityInput : uint8
{
	None = 100,

	// Primary Actions
	Interact = 00 UMETA(DisplayName = "Interact"),
	PrimaryAttack = 01 UMETA(DisplayName = "PrimaryAttack"),
	SecondaryAttack = 02 UMETA(DisplayName = "SecondaryAttack"),
	Aim = 03 UMETA(DisplayName = "Aim / Zoom"),
	Reload = 04 UMETA(DisplayName = "Reload"),


	// Locomotion
	Mantle = 10,
	Boost = 12 UMETA(DisplayName = "Boost"),
	UnBoost = 13 UMETA(DisplayName = "UnBoost"),
	Crouch = 14 UMETA(DisplayName = "Crouch"),
	SwitchStance = 19 UMETA(DisplayName = "Switch Stance"),

	// Equipment / Inventory Management
	UnEquip = 30 UMETA(DisplayName = "Equip Slot 0"),
	EquipPrimary = 31 UMETA(DisplayName = "Equip Slot 1"),
	EquipSecondary = 32 UMETA(DisplayName = "Equip Slot 2"),
	EquipTertiary = 33 UMETA(DisplayName = "Equip Slot 3"),
	EquipLethalEquipment = 34 UMETA(DisplayName = "Equip Slot 4"),
	EquipTacticalEquipment = 35 UMETA(DisplayName = "Equip Slot 5"),
	PickupItem = 38 UMETA(DisplayName = "Pick Up Item"),
	DropItem = 39 UMETA(DisplayName = "Drop Item"),

	// Possession
	PossessTarget = 50 UMETA(DisplayName = "Possess"),
	PossessMenu = 51 UMETA(DisplayName = "PossessMenu"),

	// Commands
	PingLocation = 60 UMETA(DisplayName = "CMD Ping Location"),
	MoveToLocation = 60 UMETA(DisplayName = "CMD Move to Location"),
	AttackTarget = 60 UMETA(DisplayName = "CMD Attack Target"),


	// Menus and UI
	UI_Confirm = 90 UMETA(DispalyName = "UI Select / Confirm"),
	UI_Cancel = 91 UMETA(DisplayName = "UI Back / Cancel"),
	UI_Up = 92 UMETA(DisplayName = "UI Selection Up"),
	UI_Down = 93 UMETA(DisplayName = "UI Selection Down"),
	UI_Left = 94 UMETA(DisplayName = "UI Selection Left"),
	UI_Right = 95 UMETA(DisplayName = "UI Selection Right"),

	UI_CharacterMenu = 98,
	UI_SystemMenu = 99,

	// Debug Abilities
	StopTime,
	SlowTime  UMETA(DisplayName = "SlowTime"),
	Respawn
};


USTRUCT()
struct FAbilityInputToInputActionBinding
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	UInputAction* InputAction;

	UPROPERTY(EditDefaultsOnly)
	EAbilityInput AbilityInput;
};


UCLASS()
class UAbilityInputBindings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TArray<FAbilityInputToInputActionBinding> Bindings;
};