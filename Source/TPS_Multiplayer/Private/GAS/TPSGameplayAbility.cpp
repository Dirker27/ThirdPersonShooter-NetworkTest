#include "GAS/TPSGameplayAbility.h"

#include "AbilitySystemComponent.h"

UTPSGameplayAbility::UTPSGameplayAbility()
{
	// Default to Instance Per Actor
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	// Default tags that block this ability from activating
	//ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")));
	//ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun")));
}

void UTPSGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if (ActivateAbilityOnGranted)
	{
		ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}
}

void UTPSGameplayAbility::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);

	UE_LOG(LogTemp, Log, TEXT("[%s] InputPressed()"), *GetName());
}

void UTPSGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	/*if (HasAuthority(&ActivationInfo))
	{
		UE_LOG(LogTemp, Log, TEXT("[SERVER] Ability[%s] START."), *GetName());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[CLIENT] Ability[%s] START."), *GetName());
	}*/
}

void UTPSGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	/*if (HasAuthority(&ActivationInfo))
	{
		UE_LOG(LogTemp, Log, TEXT("[SERVER] Ability[%s] END."), *GetName());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[CLIENT] Ability[%s] END."), *GetName());
	}*/
}
