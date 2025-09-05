// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "Types/TPSReport.h"

#include "TPSCombatLog.generated.h"

UENUM(BlueprintType)
enum ETPSLogEntryType : int
{
    MATCH_START,
    MATCH_OVER,

    ELIMINATION,

    PLAYER_JOINED,
    PLAYER_LEFT,

    ORDER_ISSUED,
    ORDER_COMPLETE,

    SCORE
};

USTRUCT(BlueprintType)
struct TPS_MULTIPLAYER_API FTPSLogMessage
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Text;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SubText;

    FTPSLogMessage() { }

    FTPSLogMessage(FString newMessage)
    {
        Text = newMessage;
    }

    FTPSLogMessage(FString newMessage, FString subText)
    {
        Text = newMessage;
        SubText = subText;
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
    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
    FGuid ID;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
    float Timestamp;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
    TEnumAsByte<ETPSLogEntryType> Type;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
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
    UTPSEliminationEvent() { Type = ELIMINATION; }

protected:
    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
    FTPSEliminationReport Elimination;

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
    void LogElimination(FTPSEliminationReport report);

    UFUNCTION(BlueprintCallable)
    void LogMessage(FTPSLogMessage message);
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
