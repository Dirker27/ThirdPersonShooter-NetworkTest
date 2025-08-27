#include "Log/TPSCombatLog.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

FString UTPSCombatLogEntry::ToString()
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
}

void UTPSCombatLog::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UTPSCombatLog, ClientFeedMessages);
}


void UTPSCombatLog::LogEliminationEvent(FTPSEliminationReport report)
{
	UTPSEliminationEvent* entry = NewObject<UTPSEliminationEvent>(this);
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


	LogEntries.Add(entry);
	ClientFeedMessages.Add(entry->ToString());

	AddReplicatedSubObject(entry);
}

