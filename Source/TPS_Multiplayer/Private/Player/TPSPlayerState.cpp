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

void ATPSPlayerState::Tick(float DeltaSeconds)
{
	//- Broadcast to UI Listeners -----------------------=
	//
	if (ShouldNotify) {
		NotifyDisplayWidgets.Broadcast();
		ShouldNotify = false;
	}
}


UAbilitySystemComponent* ATPSPlayerState::GetAbilitySystemComponent() const
{
	if (ATPSCharacter* character = Cast<ATPSCharacter>(GetPawn()))
	{
		return character->GetAbilitySystemComponent();
	}
	return nullptr;
}


void ATPSPlayerState::ReportProjectileHit(FHitResult hit)
{
	ShouldNotify = true;
}

