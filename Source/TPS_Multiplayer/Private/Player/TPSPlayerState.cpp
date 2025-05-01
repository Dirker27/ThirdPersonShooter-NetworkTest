// (C) ToasterCat Studios 2025

#include "Player/TPSPlayerState.h"

#include "Character/TPSCharacter.h"

ATPSPlayerState::ATPSPlayerState()
{
	SetReplicates(true);
	NetUpdateFrequency = 1.f;
}

ATPSPlayerState::~ATPSPlayerState() {
	// destruct
}

void ATPSPlayerState::BeginPlay() {
	Super::BeginPlay();
}

UAbilitySystemComponent* ATPSPlayerState::GetAbilitySystemComponent() const
{
	if (ATPSCharacter* character = Cast<ATPSCharacter>(GetPawn()))
	{
		return character->GetAbilitySystemComponent();
	}
	return nullptr;
}
