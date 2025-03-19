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
	None,
	Interact,

	// Locomotion
	Mantle,
	Crouch,
	Boost,
	UnBoost,

	// Primary Actions
	PrimaryAttack,
	SecondaryAttack,
	Aim,

	// Weapon Handling
	Reload,

	// Inventory
	EquipPrimary,
	EquipSecondary,
	EquipTertiary,
	EquipLethalEquipment,
	EquipTacticalEquipment,
	UnEquip,

	PauseMenu,
	InGameMenu,

	// Debug Abilities
	StopTime,
	SlowTime,
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
