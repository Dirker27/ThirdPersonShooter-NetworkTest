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
struct TPS_MULTIPLAYER_API FTPSUnitID
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ETPSTeamID> TeamID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETPSHierarchicalLevel UnitLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int UnitNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<ETPSHierarchicalLevel, int> Hierarchy;

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
};


static FString TPSUnitIdToString(const FTPSUnitID unitId)
{
	FString str = _TPSHierarchicalLevelToString(unitId.UnitLevel) + "-"
		+ TPSTeamIdToString(unitId.TeamID) + "_";

	for (auto lvl : unitId.Hierarchy)
	{
		str += _TPSHierarchicalLevelAbbreviationToString(lvl.Key);
		str.AppendInt(lvl.Value);
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