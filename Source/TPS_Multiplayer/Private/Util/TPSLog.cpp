// (C) ToasterCat Studios 2025

#include "Util/TPSLog.h"

void UTPSLog::Log(const FText message, const ETPSLogLevel level)
{
	if (level > LogLevel) { return; }

	FString m = message.ToString();
	if (level >= LogLevel)
	{
		UE_LOG(LogTemp, Log, TEXT("%s"), *m);
	}
}


void UTPSLog::LogAuthority(const FText message, const bool authority, const ETPSLogLevel level)
{
	if (level > LogLevel) { return; }

	FString m = (authority) ? "[SERVER] " : "[CLIENT] ";
	m.Append(message.ToString());
	if (level >= LogLevel)
	{
		UE_LOG(LogTemp, Log, TEXT("%s"), *m);
	}
}