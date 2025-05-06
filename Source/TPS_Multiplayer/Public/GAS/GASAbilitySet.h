// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"

#include "Input/TPSAbilityInput.h"

#include "GASAbilitySet.generated.h"

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
};
