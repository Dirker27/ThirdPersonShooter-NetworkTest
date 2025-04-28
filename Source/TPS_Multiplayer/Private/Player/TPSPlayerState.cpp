// (C) ToasterCat Studios 2024

#include "Player/TPSPlayerState.h"

ATPSPlayerState::ATPSPlayerState() {
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
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

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

UAbilitySystemComponent* ATPSPlayerState::GetAbilitySystemComponent() const {
	return AbilitySystemComponent;
}


// EnhancedInput -> GAS plumbing
void ATPSPlayerState::AbilityInputBindingPressedHandler(EAbilityInput abilityInput) {
	UE_LOG(LogTemp, Log, TEXT("PlayerASC_OnInputPressed[%i]"), abilityInput);

	AbilitySystemComponent->AbilityLocalInputPressed(static_cast<uint32>(abilityInput));
}
void ATPSPlayerState::AbilityInputBindingReleasedHandler(EAbilityInput abilityInput) {
	UE_LOG(LogTemp, Log, TEXT("PlayerASC_OnInputReleased[%i]"), abilityInput);

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

	//- Initialize attributes -----------------------------=
	//
	if (IsValid(InitialGameplayEffect)) {
		asc->ApplyGameplayEffectToSelf(
			InitialGameplayEffect->GetDefaultObject<UGameplayEffect>(),
			0,
			asc->MakeEffectContext());
	}

	UE_LOG(LogTemp, Log, TEXT("ASC for PlayerState[%s] initialized."), *GetPlayerName());
}