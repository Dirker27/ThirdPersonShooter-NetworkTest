#include "GAS/TPSAbilitySystemComponent.h"

#include "GAS/GASAbilitySet.h"

void UTPSAbilitySystemComponent::ReceiveDamage(UTPSAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage)
{
	ReceivedDamage.Broadcast(SourceASC, UnmitigatedDamage, MitigatedDamage);
}

void UTPSAbilitySystemComponent::AbilityLocalInputPressed(int32 InputID)
{
	Super::AbilityLocalInputPressed(InputID);
}


TArray<FGameplayAbilitySpecHandle> UTPSAbilitySystemComponent::GrantAbilitiesFromAbilitySet(UAbilitySet* abilitySet) {
    TArray<FGameplayAbilitySpecHandle> handles;
    if (!IsValid(abilitySet)) { return handles; }

    handles.Reserve(abilitySet->AbilitySetItems.Num());
    for (const auto abilitySetItem : abilitySet->AbilitySetItems)
    {
        handles.AddUnique(GiveAbility(
            FGameplayAbilitySpec(
                abilitySetItem.GameplayAbility,
                9,
                static_cast<uint32>(abilitySetItem.InputKey)
            )
        ));
    }

    return handles;
}

void UTPSAbilitySystemComponent::RevokeAbilitiesFromAbilitySystem(TArray<FGameplayAbilitySpecHandle> abilitySpecHandles)
{
    for (const auto abilityHandle : abilitySpecHandles) {
        ClearAbility(abilityHandle);
    }
}
