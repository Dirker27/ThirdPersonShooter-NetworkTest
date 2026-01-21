#include "GAS/TPSAbilitySystemComponent.h"

#include "EnhancedInputComponent.h"

#include "GAS/GASAbilitySet.h"



void UTPSAbilitySystemComponent::InitializeBaseAbilitiesAndEffects()
{
    GrantBaseAbilities(BaseAbilitySet);

    // Initialize attributes with primer GameplayEffects
    for (auto gameplayEffect : InitialGameplayEffects)
    {
        ApplyGameplayEffectToSelf(
            gameplayEffect->GetDefaultObject<UGameplayEffect>(),
            1,
            MakeEffectContext());
    }
}








//~ ======================================================================== ~//
//  GAS Documentation suggestion (tranek)
//~ ======================================================================== ~//



void UTPSAbilitySystemComponent::ReceiveDamage(UTPSAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage)
{
	ReceivedDamage.Broadcast(SourceASC, UnmitigatedDamage, MitigatedDamage);
}

void UTPSAbilitySystemComponent::AbilityLocalInputPressed(int32 InputID)
{
	Super::AbilityLocalInputPressed(InputID);
}



//~ ======================================================================== ~//
//  ABILITIES CRUD
//~ ======================================================================== ~//

static void _log_ability_list(TArray<FGameplayAbilitySpec> abilityList)
{
    for (auto ability : abilityList)
    {
        UE_LOG(LogTemp, Log, TEXT("|--- [%s]::[%i]"), *ability.Ability->GetName(), ability.InputID);
    }
}


void UTPSAbilitySystemComponent::GrantBaseAbilities(UAbilitySet* abilitySet)
{
    BaseAbilitySpecHandles.Append(GrantAbilitiesFromAbilitySet(abilitySet));
}

void UTPSAbilitySystemComponent::GrantPlayerBasedAbilities(UAbilitySet* abilitySet)
{
    if (!PlayerBasedAbilitySpecHandles.IsEmpty())
    {
        RevokePlayerBasedAbilities();
    }

    PlayerBasedAbilitySpecHandles.Append(GrantAbilitiesFromAbilitySet(abilitySet));
    //_log_ability_list(GetActivatableAbilities());
}

void UTPSAbilitySystemComponent::RevokePlayerBasedAbilities()
{
    RevokeAbilities(PlayerBasedAbilitySpecHandles);

    //_log_ability_list(GetActivatableAbilities());
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

void UTPSAbilitySystemComponent::RevokeAbilities(TArray<FGameplayAbilitySpecHandle> abilitySpecHandles)
{
    for (const auto abilityHandle : abilitySpecHandles)
    {
        ClearAbility(abilityHandle);
    }
}




//~ ======================================================================== ~//
//  INPUT WIRING
//~ ======================================================================== ~//





void UTPSAbilitySystemComponent::BindToInputComponent(UInputComponent* inputComponent)
{
    UEnhancedInputComponent* enhacedInput = Cast<UEnhancedInputComponent>(inputComponent);
    if (!IsValid(enhacedInput) || !IsValid(InputBindings)) { return; }

    UE_LOG(LogTemp, Log, TEXT("Binding ASC to Owner's InputComponent..."));
    for (const FAbilityInputToInputActionBinding& binding : InputBindings->Bindings)
    {
        enhacedInput->BindAction(binding.InputAction, ETriggerEvent::Started, this, &ThisClass::AbilityInputBindingPressedHandler, binding.AbilityInput);
        enhacedInput->BindAction(binding.InputAction, ETriggerEvent::Completed, this, &ThisClass::AbilityInputBindingReleasedHandler, binding.AbilityInput);
    }
    UE_LOG(LogTemp, Log, TEXT("ASC Input Bound."));
}

void UTPSAbilitySystemComponent::ReleaseAbilityBindingsFromInputComponent(UEnhancedInputComponent* inputComponent)
{
    // TODO: Might not be necessary? Does InputComponent deconstruct on UnPossess?
}


// EnhancedInput -> GAS plumbing
void UTPSAbilitySystemComponent::AbilityInputBindingPressedHandler(EAbilityInput abilityInput) {
	UE_LOG(LogTemp, Verbose, TEXT("ASC[%s]::OnInputPressed[%i]"), *GetOwnerActor()->GetName(), abilityInput);
	AbilityLocalInputPressed(static_cast<uint32>(abilityInput));
}
void UTPSAbilitySystemComponent::AbilityInputBindingReleasedHandler(EAbilityInput abilityInput) {
	UE_LOG(LogTemp, Verbose, TEXT("ASC[%s]::OnInputReleased[%i]"), *GetOwnerActor()->GetName(), abilityInput);
	AbilityLocalInputReleased(static_cast<uint32>(abilityInput));
}
