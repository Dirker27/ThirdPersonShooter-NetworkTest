// (C) ToasterCat Studios 2025

#pragma once

#include "Components/ActorComponent.h"

#include "Model/TPSHierarchicalCollectionLevel.h"
#include "Team/TPSTeamID.h"

#include "TPSUnitID.generated.h"


/**
 * Identifier to specify an exact unit in a given Team.
 *
 * Will be unique for each team, but may be duplicated across multiple teams.
 *   ie: there can be an 'Abel Squad' on the other Team as well as your own.
 */
USTRUCT(BlueprintType)
struct TPS_MULTIPLAYER_API FTPSUnitHierarchy
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int UnitNumber = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int FireteamNumber = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int SquadNumber = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int PlatoonNumber = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CompanyNumber = 0;

	FTPSUnitHierarchy()
	{		
	}

	static FTPSUnitHierarchy AppendLevelNumber(FTPSUnitHierarchy hierarchy, ETPSHierarchicalLevel level, int number)
	{
		FTPSUnitHierarchy h = hierarchy;
		switch (level)
		{
		case ETPSHierarchicalLevel::UNIT:
			h.UnitNumber = number;
			break;
		case ETPSHierarchicalLevel::FIRE_TEAM:
			h.FireteamNumber = number;
			break;
		case ETPSHierarchicalLevel::SQUAD:
			h.SquadNumber = number;
			break;
		case ETPSHierarchicalLevel::PLATOON:
			h.PlatoonNumber = number;
			break;
		case ETPSHierarchicalLevel::COMPANY:
			h.CompanyNumber = number;
			break;
		}
		return h;
	}

	static int GetNumberForLevel(FTPSUnitHierarchy hierarchy, ETPSHierarchicalLevel level)
	{
		switch (level)
		{
		case ETPSHierarchicalLevel::UNIT:
			return hierarchy.UnitNumber;
		case ETPSHierarchicalLevel::FIRE_TEAM:
			return hierarchy.FireteamNumber;
		case ETPSHierarchicalLevel::SQUAD:
			return hierarchy.SquadNumber;
		case ETPSHierarchicalLevel::PLATOON:
			return hierarchy.PlatoonNumber;
		case ETPSHierarchicalLevel::COMPANY:
			return hierarchy.CompanyNumber;
		default:
			return 0;
		}
	}
};


/**
 * Identifier to specify an exact unit in a given Team.
 *
 * Will be unique for each team, but may be duplicated across multiple teams.
 *   ie: there can be an 'Abel Squad' on the other Team as well as your own.
 */
USTRUCT(BlueprintType)
struct TPS_MULTIPLAYER_API FTPSUnitID
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ETPSTeamID> TeamID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ETPSHierarchicalLevel> UnitLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int UnitNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTPSUnitHierarchy Hierarchy;

	/*TPS_MULTIPLAYER_API static inline const FTPSUnitID UnSet = {
		ETPSTeamID::Independent,
		ETPSHierarchicalLevel::UNIT,
		0
	};*/

public:
	bool operator == (const FTPSUnitID other) const
	{
		return TeamID == other.TeamID
			&& UnitLevel == other.UnitLevel
			&& _hash(Hierarchy) == _hash(other.Hierarchy);
	}

	static uint64 HashHierarchy(FTPSUnitID id)
	{
		return _hash(id.Hierarchy);
	}

	static uint64 HashUnitIdentifier(FTPSUnitID id)
	{
		return _hash(id.Hierarchy)
			+ (id.UnitNumber << (static_cast<int>(id.UnitLevel) * 4))
			+ (id.TeamID << (7 * 4)); // TEAM -> First nibble of 32-bit int.
	}

private:
	static uint64 _hash(TMap<ETPSHierarchicalLevel, int> hierarchyMap)
	{
		uint64 hash = 0;
		for (auto lvl : hierarchyMap.Array())
		{
			hash += lvl.Value << (static_cast<int>(lvl.Key) * 4);
		}
		return hash;
	}

	static uint64 _hash(FTPSUnitHierarchy hierarchy)
	{
		uint64 hash = 0;
		hash += hierarchy.UnitNumber;
		hash += hierarchy.FireteamNumber << 4;
		hash += hierarchy.SquadNumber << 8;
		hash += hierarchy.PlatoonNumber << 12;
		hash += hierarchy.CompanyNumber << 16;
		return hash;
	}
};


static FString TPSUnitIdToString(const FTPSUnitID unitId)
{
	FString str = _TPSHierarchicalLevelToString(unitId.UnitLevel) + "-"
		+ TPSTeamIdToString(unitId.TeamID) + "_";


	if (unitId.Hierarchy.CompanyNumber > 0)
	{
		str += "C";
		str.AppendInt(unitId.Hierarchy.CompanyNumber);
	}
	if (unitId.Hierarchy.PlatoonNumber > 0)
	{
		str += "P";
		str.AppendInt(unitId.Hierarchy.PlatoonNumber);
	}
	if (unitId.Hierarchy.SquadNumber > 0)
	{
		str += "S";
		str.AppendInt(unitId.Hierarchy.SquadNumber);
	}
	if (unitId.Hierarchy.FireteamNumber > 0)
	{
		str += "F";
		str.AppendInt(unitId.Hierarchy.FireteamNumber);
	}

	str += _TPSHierarchicalLevelAbbreviationToString(unitId.UnitLevel);
	str.AppendInt(unitId.UnitNumber);

	return str;
}


UCLASS(BlueprintType)
class UTPSUnitIdentifier : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FTPSUnitID ID;
};