// (C) ToasterCat Studios 2025

#include "Army/Unit/TPSHierarchicalCollection.h"

UTPSHierarchicalCollection::UTPSHierarchicalCollection()
{
}


bool UTPSHierarchicalCollection::AddMember(UTPSCharacterInstance* member)
{
	if (CanAddMember(member))
	{
		int idNumber = Members.Add(member);
		return true;
	}
	return false;
}

bool UTPSHierarchicalCollection::AddSubCollection(UTPSHierarchicalCollection* subCollection)
{
	if (CanAddSubCollection(subCollection))
	{
		int index = SubCollections.Add(subCollection);
		return true;
	}
	return false;
}

bool UTPSHierarchicalCollection::RemoveMember(const int idNumber)
{
	if (idNumber >= Members.Num()) { return false; }

	if (auto member = Members[idNumber])
	{
		member->AssignedUnit = nullptr;
		member->AssignedArmy = nullptr;
		member->AssignedTeam = nullptr;

		return true;
	}
	return false;
}

bool UTPSHierarchicalCollection::RemoveSubCollection(const int idNumber)
{
	if (idNumber >= SubCollections.Num()) { return false; }

	if (auto subCollection = SubCollections[idNumber])
	{
		subCollection->Hierarchy = FTPSUnitHierarchy();
		return true;
	}
	return false;
}

bool UTPSHierarchicalCollection::CanAddMember(const UTPSCharacterInstance* member) const
{
	if (!IsValid(member))
	{
		return false;
	}

	return true;
}

// TODO: Guard against cyclical hierarchies
// - [PC-235]: https://toastercatstudios.atlassian.net/browse/PC-235
bool UTPSHierarchicalCollection::CanAddSubCollection(const UTPSHierarchicalCollection* subCollection) const
{
	if (!IsValid(subCollection))
	{
		return false;
	}

	if (UnitLevel <= subCollection->UnitLevel)
	{
		return false;
	}

	return true;
}

TArray<UTPSHierarchicalCollection*> UTPSHierarchicalCollection::GetAllSubCollections()
{
	return SubCollections;
}



UTPSCharacterInstance* UTPSHierarchicalCollection::GetMember(int targetIdNumber)
{
	if (targetIdNumber >= Members.Num()) { return nullptr; }

	return Members[targetIdNumber];
}

TArray<UTPSCharacterInstance*> UTPSHierarchicalCollection::GetAllMembers()
{
	return Members;
}


ETPSUnitLevel UTPSHierarchicalCollection::LevelDown(ETPSUnitLevel level)
{
	return _TPSLevel_Down(level);
}

ETPSUnitLevel UTPSHierarchicalCollection::LevelUp(ETPSUnitLevel level)
{
	return _TPSLevel_Up(level);
}

