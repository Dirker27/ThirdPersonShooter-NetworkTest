// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "Team/TPSTeamInstance.h"

#include "TPSCombatLog.generated.h"

UENUM(BlueprintType)
enum class ECombatLogEntryType : uint8
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


USTRUCT(BlueprintType)
struct FCombatLogEntry
{
	GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int ID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ECombatLogEntryType Type;
};


// To be owned by the GameMode
UCLASS(BlueprintType)
class TPS_MULTIPLAYER_API UTPSCombatLog : public UActorComponent
{
    GENERATED_BODY()

public:
    UTPSCombatLog();

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FCombatLogEntry> LogEntries;


};
