// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "Types/TPSReport.h"

#include "TPSCombatLog.generated.h"

UENUM(BlueprintType)
enum ETPSLogEntryType : int
{
    GameStart,
    GameOver,

    Elimination,

    PlayerKilled,
    PlayerJoined,
    PlayerLeft,

    BotKilled,

    OrderIssued,
    OrderComplete,

    ScoreAlert,
    ObjectiveUpdated
};

USTRUCT(BlueprintType)
struct TPS_MULTIPLAYER_API FTPSLogMessage
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FString Message;

    /*UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float Timestamp;*/

    FTPSLogMessage() { }

    FTPSLogMessage(FString newMessage)
    {
        Message = newMessage;
    }
};




UCLASS(BlueprintType)
class UTPSLogEntry : public UObject
{
    GENERATED_BODY()

public:
    UTPSLogEntry() { }

protected:
    virtual bool IsSupportedForNetworking() const override { return true; }

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FGuid ID;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    float Timestamp;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    TEnumAsByte<ETPSLogEntryType> Type;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FTPSLogMessage Message;

public:
    UFUNCTION(BlueprintCallable, BlueprintPure)
    virtual FString ToString();
};




UCLASS(BlueprintType)
class UTPSCombatLogEntry : public UTPSLogEntry
{
	GENERATED_BODY()

public:
    UTPSCombatLogEntry() { }
};


UCLASS(BlueprintType)
class UTPSEliminationEvent : public UTPSCombatLogEntry
{
    GENERATED_BODY()

public:
    UTPSEliminationEvent() { Type = Elimination; }

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FName VictimName;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FName KillerName;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FName KillMethod;

    virtual FString ToString() override;
};



// Owned by GameState
//   NOT replicated to individual clients - they will receive an "after-action" report on conclusion.
UCLASS(BlueprintType)
class TPS_MULTIPLAYER_API UTPSCombatLog : public UActorComponent
{
    GENERATED_BODY()

public:
    UTPSCombatLog();

protected:
    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
    virtual bool IsSupportedForNetworking() const override { return true; }

public:
    UFUNCTION(BlueprintCallable)
    void LogEliminationEvent(FTPSEliminationReport report);
};


// Local Log that will be displayed to a Client's log UI.
/*UCLASS(BlueprintType)
class TPS_MULTIPLAYER_API UTPSMessageLog : public UActorComponent
{
    GENERATED_BODY()

public:
    UTPSMessageLog();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> LogEntries;
};*/
