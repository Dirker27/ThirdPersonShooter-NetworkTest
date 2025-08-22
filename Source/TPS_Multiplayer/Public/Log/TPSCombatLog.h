// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "Types/TPSReport.h"

#include "TPSCombatLog.generated.h"

UENUM(BlueprintType)
enum ETPSCombatLogEntryType : int
{
    GameStart,
    GameOver,

    PlayerKilled,
    PlayerJoined,
    PlayerLeft,

    BotKilled,

    SquadOrder,
    PlatoonOrder,
    TeamOrder,

    ScoreAlert,
    ObjectiveUpdated
};


UCLASS(BlueprintType)
class UTPSCombatLogEntry : public UObject
{
	GENERATED_BODY()

protected:
    virtual bool IsSupportedForNetworking() const;

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FGuid ID;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    float Timestamp;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    TEnumAsByte<ETPSCombatLogEntryType> Type;

public:
    UFUNCTION(BlueprintCallable)
    virtual FString ToString();
};


UCLASS(BlueprintType)
class UTPSKillEvent : public UTPSCombatLogEntry
{
    GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FName VictimName;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FName KillerName;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FName KillMethod;

    virtual FString ToString() override;
};


USTRUCT(BlueprintType)
struct TPS_MULTIPLAYER_API FTPSLogMessage
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FName Message = FName();

    /*UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float Timestamp;*/

    FTPSLogMessage() { }

    FTPSLogMessage(FName newMessage)
    {
        Message = newMessage;
        //Timestamp = newTimestamp;
    }
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

protected:
    // Full log - SERVER-only
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<TObjectPtr<UTPSCombatLogEntry>> LogEntries;

    // Client-visible log messages.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
    TArray<FString> ClientFeedMessages;

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
