#include "Log/TPSCombatLog.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


void UTPSCombatLogEntry::IsSupportedForNetworking() const
{
	return true;
}















UTPSCombatLog::UTPSCombatLog()
{

}

void UTPSCombatLog::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UTPSCombatLog, ClientFeedMessages);
}


FString UTPSCombatLogEntry::ToString()
{
	return FString::Printf(TEXT("[%f]-[%s]::[%s]"),
		Timestamp,
		*ID.ToString(),
		*FString::FromInt(Type));
}

FString UTPSKillEvent::ToString()
{
	return FString::Printf(TEXT("[%s]:|Character[%s] KILLED Character[%s] with [%s]|"),
		*Super::ToString(),
		*KillerName.ToString(),
		*VictimName.ToString(),
		*KillMethod.ToString());
}


void UTPSCombatLog::LogEliminationEvent(FTPSEliminationReport report)
{
	UTPSKillEvent* entry = NewObject<UTPSKillEvent>(this);
	entry->ID = FGuid::NewGuid();
	entry->Type = PlayerKilled;
	entry->Timestamp = report.Timestamp;
	entry->VictimName = IsValid(report.VictimCharacter)
		? FName(report.VictimCharacter->Identity->FirstName)
		: FName();
	entry->KillerName = IsValid(report.KillerCharacter)
		? FName(report.KillerCharacter->Identity->FirstName)
		: FName();
	entry->KillMethod = report.KillMethod;


	LogEntries.Add(entry);
	ClientFeedMessages.Add(entry->ToString());

	AddReplicatedSubObject(entry);
}

