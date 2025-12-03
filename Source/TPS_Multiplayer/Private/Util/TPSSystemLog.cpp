// (C) ToasterCat Studios 2025

#include "Util/TPSSystemLog.h"


FLinearColor UTPSSystemLog::GetColorForLog(ETPSLogSource source)
{
	switch (source)
	{
	case PlayerController:
		return FLinearColor::Yellow;
	case PlayerState:
		return FLinearColor::FromSRGBColor(FColor::FromHex("00FFFFAA"));
	case AIController:
		return FLinearColor::FromSRGBColor(FColor::Magenta);
	case Character:
		return FLinearColor::FromSRGBColor(FColor::Blue);
	case Weapon:
		return FLinearColor::FromSRGBColor(FColor::Red);
	case GameMode:
		return FLinearColor::FromSRGBColor(FColor::White);
	case GameState:
		return FLinearColor::Green;
	case Army:
		return FLinearColor::FromSRGBColor(FColor::FromHex("AAFFAAAA"));
	default:
		return FLinearColor::Black;
	}
}

float UTPSSystemLog::GetDurationForLogLevel(ETPSSystemLogLevel level)
{
	switch (level)
	{
	case ERROR:
		return 20.f;
	case WARN:
		return 10.f;
	case INFO:
		return 5.f;
	default:
		return 1.f;
	}
}



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