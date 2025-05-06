#include "GAS/GASAbilitySet.h"

/*
TArray<FGameplayAbilitySpecHandle> UAbilitySet::GrantAbilitiesToAbilitySystem(UAbilitySystemComponent* asc) const {
    check(asc);

    TArray<FGameplayAbilitySpecHandle> handles;
    handles.Reserve(AbilitySetItems.Num());

    for (const auto abilitySetItem : AbilitySetItems)
    {
        handles.AddUnique(asc->GiveAbility(
                FGameplayAbilitySpec(
                    abilitySetItem.GameplayAbility,
                    9,
                    static_cast<uint32>(abilitySetItem.InputKey)
                )
            ));
    }
    
    return handles;
}

void UAbilitySet::RevokeAbilitiesFromAbilitySystem(UAbilitySystemComponent* asc, TArray<FGameplayAbilitySpecHandle> abilitySpecHandles) const
{
    for (const auto abilityHandle : abilitySpecHandles) {
        asc->ClearAbility(abilityHandle);
    }
}
*/