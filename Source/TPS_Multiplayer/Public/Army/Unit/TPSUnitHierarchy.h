// (C) ToasterCat Studios 2025

#pragma once

#include "Army/Unit/TPSUnitLevel.h"

#include "TPSUnitHierarchy.generated.h"


/**
 * The Hierarchy of senior units that have domain over a given unit.
 */
USTRUCT(BlueprintType)
struct TPS_MULTIPLAYER_API FTPSUnitHierarchy
{
	GENERATED_BODY()

	FTPSUnitHierarchy()
	{
	}

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

	static FTPSUnitHierarchy AppendLevelNumber(FTPSUnitHierarchy hierarchy, ETPSUnitLevel level, int number)
	{
		FTPSUnitHierarchy h = hierarchy;
		switch (level)
		{
		case ETPSUnitLevel::UNIT:
			h.UnitNumber = number;
			break;
		case ETPSUnitLevel::FIRE_TEAM:
			h.FireteamNumber = number;
			break;
		case ETPSUnitLevel::SQUAD:
			h.SquadNumber = number;
			break;
		case ETPSUnitLevel::PLATOON:
			h.PlatoonNumber = number;
			break;
		case ETPSUnitLevel::COMPANY:
			h.CompanyNumber = number;
			break;
		}
		return h;
	}

	static int GetNumberForLevel(FTPSUnitHierarchy hierarchy, ETPSUnitLevel level)
	{
		switch (level)
		{
		case ETPSUnitLevel::UNIT:
			return hierarchy.UnitNumber;
		case ETPSUnitLevel::FIRE_TEAM:
			return hierarchy.FireteamNumber;
		case ETPSUnitLevel::SQUAD:
			return hierarchy.SquadNumber;
		case ETPSUnitLevel::PLATOON:
			return hierarchy.PlatoonNumber;
		case ETPSUnitLevel::COMPANY:
			return hierarchy.CompanyNumber;
		default:
			return 0;
		}
	}

public:
	bool operator == (const FTPSUnitHierarchy other) const
	{
		return _hash() == other._hash();
	}

private:
	uint64 _hash() const
	{
		uint64 hash = 0;
		hash += UnitNumber;
		hash += FireteamNumber << 4;
		hash += SquadNumber << 8;
		hash += PlatoonNumber << 12;
		hash += CompanyNumber << 16;
		return hash;
	}

	static uint64 _hash(TMap<ETPSUnitLevel, int> hierarchyMap)
	{
		uint64 hash = 0;
		for (auto lvl : hierarchyMap.Array())
		{
			hash += lvl.Value << (static_cast<int>(lvl.Key) * 4);
		}
		return hash;
	}
};


static FString TPSUnitHierarchyToString(const FTPSUnitHierarchy hierarchy)
{
	FString str = "";


	if (hierarchy.CompanyNumber > 0)
	{
		str += "C";
		str.AppendInt(hierarchy.CompanyNumber);
	}
	if (hierarchy.PlatoonNumber > 0)
	{
		str += "P";
		str.AppendInt(hierarchy.PlatoonNumber);
	}
	if (hierarchy.SquadNumber > 0)
	{
		str += "S";
		str.AppendInt(hierarchy.SquadNumber);
	}
	if (hierarchy.FireteamNumber > 0)
	{
		str += "F";
		str.AppendInt(hierarchy.FireteamNumber);
	}
	if (hierarchy.UnitNumber > 0)
	{
		str += "U";
		str.AppendInt(hierarchy.FireteamNumber);
	}

	return str;
}
