// (C) ToasterCat Studios 2025

#include "Game/TPSGameState.h"

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

    DOREPLIFETIME(ThisClass, Teams);
    DOREPLIFETIME(ThisClass, TeamUnits);
    DOREPLIFETIME(ThisClass, Characters);
    DOREPLIFETIME(ThisClass, Players);

    DOREPLIFETIME(ThisClass, CombatLog);
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

void ATPSGameState::IndexTeamUnits()
{
    UE_LOG(LogTemp, Log, TEXT("TPSGameState::IndexTeamUnits()..."));

    TeamUnitsById.Empty();

    for (auto unit : TeamUnits)
    {
        TeamUnitsById.Add(FTPSUnitID::HashUnitIdentifier(unit->UnitID), unit);
    }
}

void ATPSGameState::IndexCharacters()
{
    UE_LOG(LogTemp, Log, TEXT("TPSGameState::IndexCharacters()..."));

    CharactersById.Empty();

    for (auto character : Characters)
    {
        CharactersById.Add(character->Identity->Guid, character);
    }
}



//~ ==================================================================== ~//
//  TEAM Reads
//~ ==================================================================== ~//

/*void ATPSGameState::CreateTeam(const ETPSTeamID teamId, const FTPSTeamConfiguration teamConfig)
{
    UTPSTeamInstance* instance = UTPSTeamInstanceFactory::NewTeamInstance(teamId, teamConfig, this);

    Teams.Add(instance);
    AddReplicatedSubObject(instance);
}*/

UTPSTeamInstance* ATPSGameState::GetTeam(const ETPSTeamID teamId)
{
    // lazy JIT indexing
    if (TeamsById.IsEmpty())
    {
        IndexTeams();
    }

    // perform quick fetch (use indexes)
    if (auto team = TeamsById.Find(teamId))
    {
        return *team;
    }

    // perform slow fetch (fallback)
    for (auto team : Teams)
    {
        if (team->TeamID == teamId)
        {
            return team;
        }
    }
    return nullptr;
}

int ATPSGameState::GetActiveTeamMemberCount(const ETPSTeamID teamId)
{
    int count = 0;
    if (auto team = GetTeam(teamId))
    {
        for (auto member : team->ActiveMembers)
        {
            if (member->IsAlive)
            {
                count++;
            }
        }
    }
    return count;
}

int ATPSGameState::GetTotalTeamMemberCount(const ETPSTeamID teamId)
{
    if (auto team = GetTeam(teamId))
    {
        return team->Members.Num();
    }
    return 0;
}


//~ ==================================================================== ~//
//  UNIT Reads
//~ ==================================================================== ~//

/*void ATPSGameState::CreateTeamUnit(const FTPSUnitID unitId, const FTPSCommandUnitConfiguration unitConfig)
{
    UTPSCommandStructure* instance = UTPSTeamInstanceFactory::NewTeamUnitInstance(unitId, unitConfig, this);

    TeamUnits.Add(instance);
    AddReplicatedSubObject(instance);
}*/

UTPSCommandStructure* ATPSGameState::GetUnit(const FTPSUnitID unitId)
{
    // lazy JIT index
    if (TeamUnitsById.IsEmpty())
    {
        IndexTeamUnits();
    }

    // perform quick fetch (use indexes)
    int indexHash = FTPSUnitID::HashUnitIdentifier(unitId);
    if (auto unit = TeamUnitsById.Find(indexHash))
    {
        return *unit;
    }

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


UTPSCharacterInstance* ATPSGameState::GetCharacter(const FGuid characterId)
{
    // lazy JIT index
    if (CharactersById.IsEmpty())
    {
        IndexTeamUnits();
    }

    // perform quick fetch (use indexes)
    if (auto character = CharactersById.Find(characterId))
    {
        return *character;
    }

    // perform slow fetch (fallback)
    for (auto character : Characters)
    {
        if (character->Identity->Guid == characterId)
        {
            return character;
        }
    }

    return nullptr;
}