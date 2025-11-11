// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "TPSSystemLog.generated.h"

UENUM(BlueprintType)
enum ETPSSystemLogLevel : int
{
	VERBOSE = 0,
	INFO    = 1,
	WARN    = 2,
	ERROR   = 3
};


UENUM(BlueprintType)
enum ETPSLogEventType : int
{
	MESSAGE,
	SYSTEM
};


UENUM(BlueprintType)
enum ETPSLogSource : int
{
	PlayerController,
	PlayerState,

	AIController,
	Character,

	GameMode,

	GameState
};


UCLASS(BlueprintType)
class TPS_MULTIPLAYER_API UTPSSystemLog : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FLinearColor GetColorForLog(ETPSLogSource source);


	UFUNCTION(BlueprintCallable)
	static void Log(const FText message, const ETPSSystemLogLevel level = INFO);

	UFUNCTION(BlueprintCallable)
	static void LogAuthority(const FText message, const bool authority = true, const ETPSSystemLogLevel level = INFO);


	UFUNCTION(BlueprintCallable)
	static void SetLogLevel(const ETPSSystemLogLevel level) { LogLevel = level; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static ETPSSystemLogLevel GetLogLevel() { return LogLevel; }

private:
	static inline ETPSSystemLogLevel LogLevel = INFO;
};
