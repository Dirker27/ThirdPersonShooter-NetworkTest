////////////////////////////////////////////////////////////////
/// DEPRECATED!!!
///
/// Moved to consolidated TPSCharacter
///
////////////////////////////////////////////////////////////////



#include "Character/TPSAICharacter.h"

ATPSAICharacter::ATPSAICharacter() {
	//ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	//ASC->SetIsReplicated(true);
	//ASC->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	//StandardAttrs = CreateDefaultSubobject<UStandardAttributeSet>(TEXT("StandardAttributes"));
	//CharacterHealthAttrs = CreateDefaultSubobject<UCharacterHealthAttributeSet>(TEXT("HealthAttributes"));
	//WeaponAttrs = CreateDefaultSubobject<UWeaponAttributeSet>(TEXT("WeaponAttributes"));
}

void ATPSAICharacter::BeginPlay() {
	Super::BeginPlay();
	//ASC->InitAbilityActorInfo(this, this);
}

UAbilitySystemComponent* ATPSAICharacter::GetAbilitySystemComponent() const {
	return ASC;
}