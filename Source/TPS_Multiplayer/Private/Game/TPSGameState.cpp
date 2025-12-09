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
    DOREPLIFETIME(ThisClass, Units);
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





void ATPSGameState::OnRep_Teams()
{
    IndexLocalTeamIds();
}
void ATPSGameState::IndexLocalTeamIds()
{
    UE_LOG(LogTemp, Log, TEXT("TPSGameState::IndexLocalTeamIds()..."));

    TeamsById.Empty(Teams.Num());

    for (auto team : Teams)
    {
        if (IsValid(team)) {
            TeamsById.Add(team->TeamID, team);
        }
    }

    InstanceRegistryUpdate.Broadcast();
}

void ATPSGameState::OnRep_Armies()
{
    IndexLocalArmyIds();
}
void ATPSGameState::IndexLocalArmyIds()
{
    UE_LOG(LogTemp, Log, TEXT("TPSGameState::IndexLocalArmyIds()..."));

    ArmiesById.Empty(Armies.Num());

    for (auto army : Armies)
    {
        if (IsValid(army))
        {
            ArmiesById.Add(army->ArmyID, army);
        }
    }

    InstanceRegistryUpdate.Broadcast();
}


void ATPSGameState::OnRep_Units()
{
    IndexLocalUnitIds();
}
void ATPSGameState::IndexLocalUnitIds()
{
    UE_LOG(LogTemp, Log, TEXT("TPSGameState::IndexLocalUnitIds()..."));

    UnitsById.Empty(Units.Num());

    for (auto unit : Units)
    {
        if (IsValid(unit))
        {
            UnitsById.Add(unit->UnitID, unit);
        }
    }

    InstanceRegistryUpdate.Broadcast();
}


void ATPSGameState::OnRep_Characters()
{
    IndexLocalCharacterIds();
}
void ATPSGameState::IndexLocalCharacterIds()
{
    UE_LOG(LogTemp, Log, TEXT("TPSGameState::IndexLocalCharacterIds()..."));

    CharactersById.Empty(Characters.Num());

    for (auto character : Characters)
    {
        if (IsValid(character))
        {
            // TODO: Use FTPSCharacterId
            CharactersById.Add(character->CharacterID, character);
        }
    }

    InstanceRegistryUpdate.Broadcast();
}

void ATPSGameState::OnRep_Players()
{
    IndexLocalPlayerIds();
}
void ATPSGameState::IndexLocalPlayerIds()
{
    UE_LOG(LogTemp, Log, TEXT("TPSGameState::IndexLocalPlayerIds()..."));

    PlayersById.Empty(Players.Num());

    for (auto player : Players)
    {
        if (IsValid(player))
        {
            // TODO: Use FTPSPlayerId?
            PlayersById.Add(player->ID, player);
        }
    }

    InstanceRegistryUpdate.Broadcast();
}


//~ ==================================================================== ~//
//  TEAM STORAGE
//~ ==================================================================== ~//

void ATPSGameState::RegisterTeam_Implementation(UTPSTeamInstance* team)
{
    Teams.Add(team);
    TeamsById.Add(team->TeamID, team);
    AddReplicatedSubObject(team);

    InstanceRegistryUpdate.Broadcast();
}

void ATPSGameState::UnRegisterTeam_Implementation(const ETPSTeamID tId)
{
    if (auto team = GetTeam(tId))
    {
        Teams.Remove(team);
        TeamsById.Remove(tId);
        RemoveReplicatedSubObject(team);

        InstanceRegistryUpdate.Broadcast();
    }
}



UTPSTeamInstance* ATPSGameState::GetTeam(const ETPSTeamID teamId)
{
    // perform quick fetch (use indexes)
    if (auto team = TeamsById.Find(teamId))
    {
        return *team;
    }

    return nullptr;
}

int ATPSGameState::GetActiveTeamMemberCount(const ETPSTeamID teamId)
{
    int count = 0;
    if (auto team = GetTeam(teamId))
    {
        count = team->GetMembers().Num();

        for (auto army : team->GetArmies())
        {
            for (auto member : army->GetMembers())
            {
                if (member->IsAlive)
                {
                    count++;
                }
            }
        }
    }
    return count;
}

int ATPSGameState::GetTotalTeamMemberCount(const ETPSTeamID teamId)
{
    int count = 0;
    if (auto team = GetTeam(teamId))
    {
        count = team->GetMembers().Num();
        for (auto army : team->GetArmies())
        {
            count += army->GetMembers().Num();
        }
    }
    return count;
}


//~ ==================================================================== ~//
//  ARMY / UNIT STORAGE
//~ ==================================================================== ~//

void ATPSGameState::RegisterArmy_Implementation(UTPSArmyInstance* army)
{
    Armies.Add(army);
    ArmiesById.Add(army->ArmyID, army);
    AddReplicatedSubObject(army);

    InstanceRegistryUpdate.Broadcast();
}

void ATPSGameState::UnRegisterArmy_Implementation(const FTPSArmyID aId)
{
    if (auto army = GetArmy(aId))
    {
        Armies.Remove(army);
        ArmiesById.Remove(aId);
        RemoveReplicatedSubObject(army);

        InstanceRegistryUpdate.Broadcast();
    }
}



UTPSArmyInstance* ATPSGameState::GetArmy(const FTPSArmyID armyId)
{
    // perform quick fetch (use indexes)
    if (auto team = ArmiesById.Find(armyId))
    {
        return *team;
    }
    return nullptr;
}


void ATPSGameState::RegisterUnit_Implementation(UTPSCommandUnit* unit)
{
    Units.Add(unit);
    UnitsById.Add(unit->UnitID, unit);
    AddReplicatedSubObject(unit);

    InstanceRegistryUpdate.Broadcast();
}

void ATPSGameState::UnRegisterUnit_Implementation(const FTPSUnitID uId)
{
    if (auto unit = GetUnit(uId))
    {
        Units.Remove(unit);
        UnitsById.Remove(uId);
        RemoveReplicatedSubObject(unit);

        InstanceRegistryUpdate.Broadcast();
    }
}


UTPSCommandUnit* ATPSGameState::GetUnit(const FTPSUnitID unitId)
{
    // perform quick fetch (use indexes)
    if (auto unit = UnitsById.Find(unitId))
    {
        return *unit;
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
//  CHARACTER CRUD
//~ ==================================================================== ~//


void ATPSGameState::RegisterCharacter_Implementation(UTPSCharacterInstance* character)
{
    Characters.Add(character);
    CharactersById.Add(character->CharacterID, character);
    AddReplicatedSubObject(character);

    InstanceRegistryUpdate.Broadcast();
}

void ATPSGameState::UnRegisterCharacter_Implementation(const FTPSCharacterID cId)
{
    if (auto character = GetCharacter(cId))
    {
        Characters.Remove(character);
        CharactersById.Remove(cId);
        RemoveReplicatedSubObject(character);

        InstanceRegistryUpdate.Broadcast();
    }
}

UTPSCharacterInstance* ATPSGameState::GetCharacter(const FTPSCharacterID characterId)
{
    // perform quick fetch (use indexes)
    if (auto character = CharactersById.Find(characterId))
    {
        return *character;
    }
    return nullptr;
}



//~ ==================================================================== ~//
//  PLAYER CRUD
//~ ==================================================================== ~//


void ATPSGameState::RegisterPlayer_Implementation(ATPSPlayerState* player)
{
    Players.Add(player);
    PlayersById.Add(player->ID, player);
    AddReplicatedSubObject(player);

    InstanceRegistryUpdate.Broadcast();
}

void ATPSGameState::UnRegisterPlayer_Implementation(const FTPSPlayerID pId)
{
    if (auto player = GetPlayer(pId))
    {
        Players.Remove(player);
        PlayersById.Remove(pId);
        RemoveReplicatedSubObject(player);

        InstanceRegistryUpdate.Broadcast();
    }
}

ATPSPlayerState* ATPSGameState::GetPlayer(const FTPSPlayerID pId)
{
    // perform quick fetch (use indexes)
    if (auto player = PlayersById.Find(pId))
    {
        return *player;
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
}

void ATPSGameState::OnRep_BroadcastMessage()
{
    MatchStateUpdate.Broadcast();
}
