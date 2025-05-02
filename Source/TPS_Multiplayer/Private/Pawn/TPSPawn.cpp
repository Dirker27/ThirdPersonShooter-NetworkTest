// (C) ToasterCat Studios 2025

#include "Pawn/TPSPawn.h"

ATPSPawn::ATPSPawn()
{
	SetReplicates(true);

	AbilitySystemComponent = CreateDefaultSubobject<UTPSAbilitySystemComponent>(TEXT("ASC"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

#if WITH_EDITORONLY_DATA
	ArrowComponent = CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	if (ArrowComponent)
	{
		ArrowComponent->ArrowColor = FColor(150, 200, 255);
		ArrowComponent->bTreatAsASprite = true;
		ArrowComponent->SetupAttachment(GetRootComponent());
		ArrowComponent->bIsScreenSizeScaled = true;
		ArrowComponent->SetSimulatePhysics(false);
	}
#endif // WITH_EDITORONLY_DATA
}

ATPSPawn::~ATPSPawn()
{
	// destruct
}

void ATPSPawn::BeginPlay()
{
	Super::BeginPlay();
	// init
}

void ATPSPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Don't use parent's config.
}


void ATPSPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Tick
}

UAbilitySystemComponent* ATPSPawn::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
