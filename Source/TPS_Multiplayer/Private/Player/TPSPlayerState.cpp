// (C) ToasterCat Studios 2025

#include "Player/TPSPlayerState.h"

#include "EnhancedInputComponent.h"
#include "GAS/TPSAbilitySystemComponent.h"

ATPSPlayerState::ATPSPlayerState()
{
	SetReplicates(true);

	AbilitySystemComponent = CreateDefaultSubobject<UTPSAbilitySystemComponent>(TEXT("ASC"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	NetUpdateFrequency = 30.0f;
}

ATPSPlayerState::~ATPSPlayerState() {
	// destruct
}

void ATPSPlayerState::BeginPlay() {
	Super::BeginPlay();

	if (HasAuthority())
	{
		SetupInitialAbilitiesAndEffects();
	}

	AbilitySystemComponent->InitAbilityActorInfo(this, nullptr);
}

void ATPSPlayerState::BindInputToASC(UEnhancedInputComponent* input)
{
	if (IsValid(input)) {
		if (HasAuthority())
		{
			UE_LOG(LogTemp, Log, TEXT("[SERVER] Binding Input for PlayerState[%s]..."), *GetName());
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("[CLIENT] Binding Input for PlayerState[%s]..."), *GetName());
		}

		for (const FAbilityInputToInputActionBinding& binding : AbilityInputBindings.Bindings)
		{
			UE_LOG(LogTemp, Log, TEXT("|-Binding Input[%s]..."), *binding.InputAction->GetName());

			input->BindAction(binding.InputAction, ETriggerEvent::Started, this, &ATPSPlayerState::AbilityInputBindingPressedHandler, binding.AbilityInput);
			input->BindAction(binding.InputAction, ETriggerEvent::Completed, this, &ATPSPlayerState::AbilityInputBindingReleasedHandler, binding.AbilityInput);
		}
	}
}

// EnhancedInput -> GAS plumbing
void ATPSPlayerState::AbilityInputBindingPressedHandler(EAbilityInput abilityInput) {
	if (HasAuthority())	
	{
		UE_LOG(LogTemp, Log, TEXT("[SERVER] PlayerASC::OnInputPressed[%i]"), abilityInput);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[CLIENT] PlayerASC::OnInputPressed[%i]"), abilityInput);
	}	

	AbilitySystemComponent->AbilityLocalInputPressed(static_cast<uint32>(abilityInput));
}
void ATPSPlayerState::AbilityInputBindingReleasedHandler(EAbilityInput abilityInput) {
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Log, TEXT("[SERVER] PlayerASC::OnInputReleased[%i]"), abilityInput);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[CLIENT] PlayerASC::OnInputReleased[%i]"), abilityInput);
	}

	AbilitySystemComponent->AbilityLocalInputReleased(static_cast<uint32>(abilityInput));
}



// Should only be called from SERVER when initializing.
void ATPSPlayerState::SetupInitialAbilitiesAndEffects() {
	UE_LOG(LogTemp, Log, TEXT("Initializing ASC for PlayerState[%s]..."), *GetPlayerName());

	UAbilitySystemComponent* asc = GetAbilitySystemComponent();
	if (!IsValid(asc)) {
		return;
	}

	//- Grant default abilities ---------------------------=
	//
	if (IsValid(InitialAbilitySet)) {
		InitiallyGrantedAbilitySpecHandles.Append(
			InitialAbilitySet->GrantAbilitiesToAbilitySystem(asc));
	}

	UE_LOG(LogTemp, Log, TEXT("ASC for PlayerState[%s] initialized."), *GetPlayerName());
	for (auto ability : AbilitySystemComponent->GetActivatableAbilities())
	{
		UE_LOG(LogTemp, Log, TEXT("|--- [%s]::[%i]"), *ability.Ability->GetName(), ability.InputID);
	}
}