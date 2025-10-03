#include "Log/TPSCombatLog.h"

#include "Game/TPSGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Util/TPSFunctionLibrary.h"

FString UTPSLogEntry::ToString()
{
	return FString::Printf(TEXT("[%f]-[%s]::[%s]"),
		Timestamp,
		*ID.ToString(),
		*FString::FromInt(Type));
}

void UTPSLogEntry::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ID);
	DOREPLIFETIME(ThisClass, Timestamp);
	DOREPLIFETIME(ThisClass, Type);
	DOREPLIFETIME(ThisClass, Message);
}


FString UTPSEliminationEvent::ToString()
{
	return FString::Printf(TEXT("[%s]:|Character[%s] KILLED Character[%s] with [%s]|"),
		*Super::ToString(),
		*Elimination.KillerCharacter->Identity.Guid.ToString(),
		*Elimination.VictimCharacter->Identity.Guid.ToString(),
		*Elimination.KillMethod);
}

void UTPSEliminationEvent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Elimination);
}





UTPSCombatLog::UTPSCombatLog()
{
	SetIsReplicatedByDefault(true);
}

void UTPSCombatLog::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}


void UTPSCombatLog::LogElimination(FTPSEliminationReport report)
{
	ATPSGameState* state = Cast<ATPSGameState>(UGameplayStatics::GetGameState(this));

	UTPSEliminationEvent* entry = NewObject<UTPSEliminationEvent>(state);
	entry->ID = FGuid::NewGuid();
	entry->Timestamp = report.Timestamp;
	entry->Elimination = report;
	entry->Message.Text = entry->ToString();

	state->LogEvent(entry);
}


void UTPSCombatLog::LogMessage(FTPSLogMessage message)
{
	ATPSGameState* state = Cast<ATPSGameState>(UGameplayStatics::GetGameState(this));

	UTPSCombatLogEntry* entry = NewObject<UTPSCombatLogEntry>(state);
	entry->ID = FGuid::NewGuid();
	entry->Timestamp = UGameplayStatics::GetTimeSeconds(this);
	entry->Message.Text = message.Text;
	entry->Message.SubText = message.SubText;

	state->LogEvent(entry);
}


