// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "TPSLog.generated.h"

UENUM(BlueprintType)
enum ETPSLogLevel : int
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
	SYSTEM,
};


UCLASS()
class TPS_MULTIPLAYER_API UTPSLog : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	static void Log(const FText message, const ETPSLogLevel level = INFO);

	UFUNCTION(BlueprintCallable)
	static void LogAuthority(const FText message, const bool authority = true, const ETPSLogLevel level = INFO);


	UFUNCTION(BlueprintCallable)
	static void SetLogLevel(const ETPSLogLevel level) { LogLevel = level; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static ETPSLogLevel GetLogLevel() { return LogLevel; }

private:
	static inline ETPSLogLevel LogLevel = INFO;
};
