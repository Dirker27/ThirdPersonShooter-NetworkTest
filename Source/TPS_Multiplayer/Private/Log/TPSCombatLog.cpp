#include "Log/TPSCombatLog.h"

#include "Game/TPSGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

FString UTPSLogEntry::ToString()
{
	return FString::Printf(TEXT("[%f]-[%s]::[%s]"),
		Timestamp,
		*ID.ToString(),
		*FString::FromInt(Type));
}

FString UTPSEliminationEvent::ToString()
{
	return FString::Printf(TEXT("[%s]:|Character[%s] KILLED Character[%s] with [%s]|"),
		*Super::ToString(),
		*KillerName.ToString(),
		*VictimName.ToString(),
		*KillMethod.ToString());
}






UTPSCombatLog::UTPSCombatLog()
{
	SetIsReplicatedByDefault(true);

	//ReplicateSubobjects();
}

void UTPSCombatLog::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}


void UTPSCombatLog::LogEliminationEvent(FTPSEliminationReport report)
{
	ATPSGameState* state = Cast<ATPSGameState>(UGameplayStatics::GetGameState(this));

	UTPSEliminationEvent* entry = NewObject<UTPSEliminationEvent>(state);
	entry->ID = FGuid::NewGuid();
	entry->Type = PlayerKilled;
	entry->Timestamp = report.Timestamp;
	entry->VictimName = IsValid(report.VictimCharacter)
		? FName(report.VictimCharacter->Identity.FirstName)
		: FName();
	entry->KillerName = IsValid(report.KillerCharacter)
		? FName(report.KillerCharacter->Identity.FirstName)
		: FName();
	entry->KillMethod = report.KillMethod;

	//state->LogEntries.Add(entry);
	//state->ClientMessageFeed.Add(entry->Message);
	//state->AddReplicatedSubObject(entry);
	state->LogEvent(entry);
}

