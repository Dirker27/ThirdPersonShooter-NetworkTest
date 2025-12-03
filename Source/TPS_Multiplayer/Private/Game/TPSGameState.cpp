// (C) ToasterCat Studios 2025

#include "Game/TPSGameState.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

#include "Team/TPSTeamInstanceFactory.h"

ATPSGameState::ATPSGameState()
{
    bReplicates = true;
    bReplicateUsingRegisteredSubObjectList = true;
}


void ATPSGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Match State
    DOREPLIFETIME(ThisClass, MatchPhase);
    DOREPLIFETIME(ThisClass, MatchTimeLimitSeconds);
    DOREPLIFETIME(ThisClass, TimeMatchStarted);
    DOREPLIFETIME(ThisClass, TimeMatchEnded);
    DOREPLIFETIME(ThisClass, WinningTeam);

    // WorldState
    DOREPLIFETIME(ThisClass, Teams);
    DOREPLIFETIME(ThisClass, Armies);
    DOREPLIFETIME(ThisClass, TeamUnits);
    DOREPLIFETIME(ThisClass, Characters);
    DOREPLIFETIME(ThisClass, Players);

    // Log
    DOREPLIFETIME(ThisClass, LogEntries);

    // Message Queue
    DOREPLIFETIME(ThisClass, BroadcastMessage);
}


void ATPSGameState::UpdateMatchPhase_Implementation(ETPSMatchPhase newPhase)
{
    MatchPhase = newPhase;
    MatchStateUpdate.Broadcast();
}

void ATPSGameState::OnRep_MatchPhase()
{
    MatchStateUpdate.Broadcast();
}





double ATPSGameState::GetTimeRemainingSeconds() const
{
    if (TimeMatchStarted < 0)
    {
        return 0;
    }

    double timeElapsed = UGameplayStatics::GetTimeSeconds(this)
        - TimeMatchStarted;

    return MatchTimeLimitSeconds - timeElapsed;
}

FString ATPSGameState::GetTimeRemainingSecondsAsString() const
{
    double timeRemainingSeconds = GetTimeRemainingSeconds();

    if (timeRemainingSeconds <= 0)
    {
        return "00.00";
    }

    int hours = static_cast<int>(timeRemainingSeconds) / 3600;
    int minutes = (static_cast<int>(timeRemainingSeconds) / 60) % 60;
    double seconds = static_cast<int>(timeRemainingSeconds) % 60;

    if (hours > 0)
    {
        return FString::Printf(TEXT("%02d:%02d:%02.0f"),
            hours, minutes, seconds);
    }
    if (minutes > 0)
    {
        return FString::Printf(TEXT("%02d:%02.0f"), minutes, seconds);
    }

    return FString::Printf(TEXT("%02.02f"), timeRemainingSeconds);
}


int ATPSGameState::GetTeamScore(const ETPSTeamID teamId)
{
    if (auto team = GetTeam(teamId))
    {
        return team->ScoredPoints;
    }
    return 0;
}


void ATPSGameState::IndexTeams()
{
    UE_LOG(LogTemp, Log, TEXT("TPSGameState::IndexTeams()..."));

    TeamsById.Empty();

    for (auto team : Teams)
    {
        TeamsById.Add(team->TeamID, team);
    }
}

void ATPSGameState::IndexArmies()
{
    UE_LOG(LogTemp, Log, TEXT("TPSGameState::IndexArmies()..."));

    ArmiesById.Empty();

    for (auto army : Armies)
    {
        ArmiesById.Add(army->ArmyID, army);
    }
}

void ATPSGameState::IndexTeamUnits()
{
    UE_LOG(LogTemp, Log, TEXT("TPSGameState::IndexTeamUnits()..."));

    TeamUnitsById.Empty();

    for (auto unit : TeamUnits)
    {
        TeamUnitsById.Add(unit->UnitID, unit);
    }
}

void ATPSGameState::IndexCharacters()
{
    UE_LOG(LogTemp, Log, TEXT("TPSGameState::IndexCharacters()..."));

    CharactersById.Empty();

    for (auto character : Characters)
    {
        // TODO: Use FTPSCharacterId
        CharactersById.Add(character->CharacterID, character);
    }
}



//~ ==================================================================== ~//
//  TEAM Reads
//~ ==================================================================== ~//

UTPSTeamInstance* ATPSGameState::GetTeam(const ETPSTeamID teamId)
{
    // lazy JIT indexing
    /*if (TeamsById.IsEmpty())
    {
        IndexTeams();
    }

    // perform quick fetch (use indexes)
    if (auto team = TeamsById.Find(teamId))
    {
        return *team;
    }*/

    // perform slow fetch (fallback)
    for (auto team : Teams)
    {
        if (IsValid(team) && team->TeamID == teamId)
        {
            return team;
        }
    }
    return nullptr;
}

int ATPSGameState::GetActiveTeamMemberCount(const ETPSTeamID teamId)
{
    int count = 0;
    /*if (auto team = GetTeam(teamId))
    {
        for (auto member : team->ActiveMembers)
        {
            if (member->IsAlive)
            {
                count++;
            }
        }
    }*/
    return count;
}

int ATPSGameState::GetTotalTeamMemberCount(const ETPSTeamID teamId)
{
    /*if (auto team = GetTeam(teamId))
    {
        return team->Members.Num();
    }*/
    return 0;
}


//~ ==================================================================== ~//
//  UNIT Reads
//~ ==================================================================== ~//

UTPSArmyInstance* ATPSGameState::GetArmy(const FTPSArmyID armyId)
{
    for (auto army : Armies)
    {
        if (armyId.Guid == army->ArmyID.Guid)
        {
            return army;
        }
    }
    return nullptr;
}


UTPSCommandUnit* ATPSGameState::GetUnit(const FTPSUnitID unitId)
{
    // lazy JIT index
    /*if (TeamUnitsById.IsEmpty())
    {
        IndexTeamUnits();
    }

    // perform quick fetch (use indexes)
    if (auto unit = TeamUnitsById.Find(unitId))
    {
        return *unit;
    }*/

    // perform slow fetch (fallback)
    for (auto unit : TeamUnits)
    {
        if (unit->UnitID == unitId)
        {
            return unit;
        }
    }
    return nullptr;
}

UTPSCharacterInstance* ATPSGameState::GetUnitLeader(const FTPSUnitID unitId)
{
    if (auto unit = GetUnit(unitId))
    {
        return unit->GetLeader();
    }
    return nullptr;
}


//~ ==================================================================== ~//
//  CHARACTER Reads
//~ ==================================================================== ~//


UTPSCharacterInstance* ATPSGameState::GetCharacter(const FTPSCharacterID characterId)
{
    // lazy JIT index
    /*if (CharactersById.IsEmpty())
    {
        IndexCharacters();
    }

    // perform quick fetch (use indexes)
    if (auto character = CharactersById.Find(characterId))
    {
        return *character;
    }*/

    // perform slow fetch (fallback)
    for (auto character : Characters)
    {
        if (character->CharacterID == characterId)
        {
            return character;
        }
    }

    return nullptr;
}



//~ ==================================================================== ~//
//  LOGGING Operations
//~ ==================================================================== ~//


void ATPSGameState::LogEvent_Implementation(UTPSCombatLogEntry* entry)
{
    LogEntries.Add(entry);
    AddReplicatedSubObject(entry);

    LogUpdate.Broadcast();
}

void ATPSGameState::OnRep_LogEntries()
{
    LogUpdate.Broadcast();
}



TArray<UTPSCombatLogEntry*> ATPSGameState::GetRecentLogEvents(int feedLength) const
{
    TArray<UTPSCombatLogEntry*> feed;

    int len = feedLength;
    if (feedLength > LogEntries.Num())
    {
        len = LogEntries.Num();
    }

    for (int i = LogEntries.Num() - len; i < LogEntries.Num(); i++)
    {
        feed.Add(LogEntries[i]);
    }

    return feed;
}




void ATPSGameState::UpdateBroadcastMessage_Implementation(FTPSBroadcastMessage message)
{
    if (IsValid(BroadcastMessage))
    {
        RemoveReplicatedSubObject(BroadcastMessage);
        BroadcastMessage = nullptr;
    }

    UTPSBroadcastMessageObject* messageObj = NewObject<UTPSBroadcastMessageObject>(this);
    messageObj->Message = message;
    messageObj->TimestampDisplayStarted = UGameplayStatics::GetRealTimeSeconds(this);
    BroadcastMessage = messageObj;
    AddReplicatedSubObject(messageObj);

    MessageUpdate.Broadcast();
    //BroadcastMessageUpdate();
}

void ATPSGameState::OnRep_BroadcastMessage()
{
    MatchStateUpdate.Broadcast();
}
