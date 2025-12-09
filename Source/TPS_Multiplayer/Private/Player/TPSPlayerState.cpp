// (C) ToasterCat Studios 2025

#include "Player/TPSPlayerState.h"
#include "Team/TPSTeamInstance.h"

#include "Character/TPSCharacter.h"
#include "Net/UnrealNetwork.h"

ATPSPlayerState::ATPSPlayerState()
{
	SetReplicates(true);
	NetUpdateFrequency = 1.f;

	SetIsABot(false);
}

ATPSPlayerState::~ATPSPlayerState() {
	// destruct
}

void ATPSPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ID);
	DOREPLIFETIME(ThisClass, AssignedArmy);
	DOREPLIFETIME(ThisClass, AssignedTeam);
	DOREPLIFETIME(ThisClass, FocusActor);
	DOREPLIFETIME(ThisClass, TargetActor);
	DOREPLIFETIME(ThisClass, ControlledActor);
	DOREPLIFETIME(ThisClass, ControlledUnit);
}


void ATPSPlayerState::BeginPlay() {
	Super::BeginPlay();
}


// TODO: [PC-257] Elevate to "ArmyAssignable" Interface
void ATPSPlayerState::AssignToArmy(UTPSArmyInstance* army)
{
	if (!IsValid(army)) { return; }

	AssignedArmy = army;
	AssignedTeam = army->GetAssignedTeam();

	PlayerStateUpdate.Broadcast();
}

// TODO: [PC-237] Elevate to "TeamAssignable" Interface
void ATPSPlayerState::AssignToTeam(UTPSTeamInstance* team)
{
	if (!IsValid(team)) { return; }

	AssignedTeam = team;

	PlayerStateUpdate.Broadcast();
}

// TODO: [PC-237] Elevate to "TeamAssignable" Interface
ETPSTeamID ATPSPlayerState::GetAssignedTeamID() const
{
	return AssignedTeam.IsValid()
		? AssignedTeam.Get()->TeamID
		: ETPSTeamID::UNAFFILIATED;
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
	PlayerStateUpdate.Broadcast();
}

