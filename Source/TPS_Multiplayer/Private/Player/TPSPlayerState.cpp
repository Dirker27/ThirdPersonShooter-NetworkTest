// (C) ToasterCat Studios 2024

#include "Player/TPSPlayerState.h"

ATPSPlayerState::ATPSPlayerState() {
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	//StandardAttributes = CreateDefaultSubobject<UStandardAttributeSet>(TEXT("StandardAttributes"));
	//CharacterHealthAttributes = CreateDefaultSubobject<UCharacterHealthAttributeSet>(TEXT("HealthAttributes"));
	//WeaponAttributes = CreateDefaultSubobject<UWeaponAttributeSet>(TEXT("WeaponAttributes"));

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





// Should only be called from SERVER when initializing.
void ATPSPlayerState::SetupInitialAbilitiesAndEffects() {
	UE_LOG(LogTemp, Log, TEXT("Initializing ASC for PlayerState[%s]..."), *GetPlayerName());

	UAbilitySystemComponent* asc = GetAbilitySystemComponent();
	if (!IsValid(asc)) {
		return;
	}
	/*asc->GetGameplayAttributeValueChangeDelegate(UCharacterHealthAttributeSet::GetHealthAttribute())
		.AddUObject(this, &ThisClass::OnHealthAttributeChanged);
	asc->GetGameplayAttributeValueChangeDelegate(UCharacterHealthAttributeSet::GetArmorAttribute())
		.AddUObject(this, &ThisClass::OnArmorAttributeChanged);
	asc->GetGameplayAttributeValueChangeDelegate(UStandardAttributeSet::GetMovementSpeedModifierAttribute())
		.AddUObject(this, &ThisClass::OnMovementAttributeChanged);*/

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