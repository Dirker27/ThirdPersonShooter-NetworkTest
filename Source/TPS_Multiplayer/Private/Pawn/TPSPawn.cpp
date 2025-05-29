// (C) ToasterCat Studios 2025

#include "Pawn/TPSPawn.h"

#include "Net/UnrealNetwork.h"
#include "Player/TPSPlayerState.h"

ATPSPawn::ATPSPawn()
{
	SetReplicates(true);

	AbilitySystem = CreateDefaultSubobject<UTPSAbilitySystemComponent>(TEXT("AbilitySystem"));
	AbilitySystem->SetIsReplicated(true);
	AbilitySystem->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

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

void ATPSPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATPSPawn, TargetLookRotation);
}


void ATPSPawn::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystem->InitAbilityActorInfo(this, this);
	if (HasAuthority())
	{
		AbilitySystem->InitializeBaseAbilitiesAndEffects();
	}
}

void ATPSPawn::SetupPlayerInputComponent(UInputComponent* inputComponent)
{
	// Super::SetupPlayerInputComponent(inputComponent);
	// ^^
	// Don't use parent's setup. We just want DefaultPawn's functions, not its
	//   control mappings. (flying pawn w/ camera pitch+yaw)

	AbilitySystem->BindToInputComponent(inputComponent);
}


void ATPSPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Tick
}


void ATPSPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (HasAuthority())
	{
		UE_LOG(LogTemp, Log, TEXT("[SERVER] Pawn[%s]::PossessedBy()"), *GetName());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[CLIENT] Pawn[%s]::PossessedBy()"), *GetName());
	}

	if (ATPSPlayerState* ps = GetPlayerState<ATPSPlayerState>())
	{
		if (HasAuthority())
		{
			AbilitySystem->GrantPlayerBasedAbilities(ps->PlayerAbilitySet);
		}
	}
}

void ATPSPawn::UnPossessed()
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Log, TEXT("[SERVER] Pawn[%s]::UnPossessed()"), *GetName());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[CLIENT] Pawn[%s]::UnPossessed()"), *GetName());
	}

	if (HasAuthority())
	{
		AbilitySystem->RevokePlayerBasedAbilities();
	}
}



void ATPSPawn::OnRep_PlayerState() { // client
	Super::OnRep_PlayerState();

	if (HasAuthority())
	{
		UE_LOG(LogTemp, Log, TEXT("[SERVER] Pawn[%s]::OnRep_PlayerState()"), *GetName());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[CLIENT] Pawn[%s]::OnRep_PlayerState()"), *GetName());
	}
}


