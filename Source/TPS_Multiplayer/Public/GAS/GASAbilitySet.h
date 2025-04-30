// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"

#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpecHandle.h"

#include "GASAbilitySet.generated.h"


UENUM(BlueprintType)
enum class EAbilityInput : uint8
{
	None     = 0 UMETA(DisplayName = "NONE"),
	Interact = 1,

	// Locomotion
	Mantle  = 10,
	Boost   = 12 UMETA(DisplayName = "Boost"),
	UnBoost = 13 UMETA(DisplayName = "UnBoost"),
	Crouch  = 14 UMETA(DisplayName = "Crouch"),

	// Primary Actions
	PrimaryAttack,
	SecondaryAttack,
	Aim,

	// Weapon Handling
	UnEquip        = 30 UMETA(DisplayName = "EquipSlot0"),
	EquipPrimary   = 31 UMETA(DisplayName = "EquipSlot1"),
	EquipSecondary = 32 UMETA(DisplayName = "EquipSlot2"),
	EquipTertiary  = 33 UMETA(DisplayName = "EquipSlot3"),
	EquipLethalEquipment = 34 UMETA(DisplayName = "EquipSlot4"),
	EquipTacticalEquipment = 35 UMETA(DisplayName = "EquipSlot5"),
	Reload         = 36 UMETA(DisplayName = "Reload"),

	// Possession
	PossessTarget = 50 UMETA(DisplayName = "Possess"),
	PossessMenu   = 51 UMETA(DisplayName = "PossessMenu"),

	// Inventory Management
	PickupItem,
	DropItem,

	// System
	PauseMenu,
	InGameMenu,

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


USTRUCT()
struct FAbilityInputBindings
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TArray<FAbilityInputToInputActionBinding> Bindings;
};

USTRUCT()
struct FAbilitySystem
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="Ability")
	TSubclassOf<UGameplayAbility> GameplayAbility;

	UPROPERTY(EditDefaultsOnly, Category="Ability")
	EAbilityInput InputKey;
};


UCLASS()
class TPS_MULTIPLAYER_API UAbilitySet : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Ability")
	TArray<FAbilitySystem> AbilitySetItems;

	TArray<FGameplayAbilitySpecHandle> GrantAbilitiesToAbilitySystem(UAbilitySystemComponent* AbilitySystemComponent) const;
};
