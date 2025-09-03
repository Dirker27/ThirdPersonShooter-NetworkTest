// (C) ToasterCat Studios 2025

#include "Util/TPSSystemLog.h"

void UTPSSystemLog::Log(const FText message, const ETPSSystemLogLevel level)
{
	if (level > LogLevel) { return; }

	FString m = message.ToString();
	if (level >= LogLevel)
	{
		UE_LOG(LogTemp, Log, TEXT("%s"), *m);
	}
}


void UTPSSystemLog::LogAuthority(const FText message, const bool authority, const ETPSSystemLogLevel level)
{
	if (level > LogLevel) { return; }

	FString m = (authority) ? "[SERVER] " : "[CLIENT] ";
	m.Append(message.ToString());
	if (level >= LogLevel)
	{
		UE_LOG(LogTemp, Log, TEXT("%s"), *m);
	}
}