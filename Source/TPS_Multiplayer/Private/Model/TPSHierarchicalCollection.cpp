// (C) ToasterCat Studios 2025

#include "Model/TPSHierarchicalCollection.h"

UTPSHierarchicalCollection::UTPSHierarchicalCollection()
{
}


bool UTPSHierarchicalCollection::AddMember(UTPSCharacterInstance* member)
{
	if (CanAddMember(member))
	{
		int idNumber = Members.Add(member);
		UpdateUnitIdForMember(member, idNumber);
		return true;
	}
	return false;
}

bool UTPSHierarchicalCollection::AddSubCollection(UTPSHierarchicalCollection* subCollection)
{
	if (CanAddSubCollection(subCollection))
	{
		int index = SubCollections.Add(subCollection);
		UpdateUnitIdForSubCollection(subCollection, index);
		return true;
	}
	return false;
}

bool UTPSHierarchicalCollection::RemoveMember(const int idNumber)
{
	if (idNumber >= Members.Num()) { return false; }

	if (auto member = Members[idNumber])
	{
		member->Identity.UnitID.Hierarchy = FTPSUnitHierarchy();
		member->Identity.UnitID.UnitNumber = 0;

		return true;
	}
	return false;
}

bool UTPSHierarchicalCollection::RemoveSubCollection(const int idNumber)
{
	if (idNumber >= SubCollections.Num()) { return false; }

	if (auto subCollection = SubCollections[idNumber])
	{
		subCollection->UnitID.Hierarchy = FTPSUnitHierarchy();
		subCollection->UnitID.UnitNumber = 0;
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
	//return !Members.Contains(member);
}

// TODO: Guard against cyclical hierarchies
bool UTPSHierarchicalCollection::CanAddSubCollection(const UTPSHierarchicalCollection* subCollection) const
{
	if (!IsValid(subCollection))
	{
		return false;
	}

	if (UnitID.UnitLevel <= subCollection->UnitID.UnitLevel)
	{
		return false;
	}

	return true;
	//return !SubCollections.Contains(subCollection);
}


/*UTPSHierarchicalCollection* UTPSHierarchicalCollection::GetSubCollection(FTPSUnitID id)
{
	if (id.UnitLevel > UnitID.UnitLevel)
	{
		return nullptr;
	}

	if (id.UnitLevel == UnitID.UnitLevel)
	{
		return id.UnitNumber == UnitID.UnitNumber
			? this
			: nullptr;
	}


	if (UnitID.UnitLevel == ETPSHierarchicalLevel::UNIT)
	{
		return nullptr;
	}

	ETPSHierarchicalLevel targetChildLvl = LevelDown(UnitID.UnitLevel);
	if (auto targetChildUnitNumber = id.Hierarchy.Find(targetChildLvl))
	{
		if (*targetChildUnitNumber >= SubCollections.Num()) { return nullptr; }
		if (auto childUnit = SubCollections[*targetChildUnitNumber])
		{
			return childUnit->GetSubCollection(id);
		}
	}

	return nullptr;
}*/

TArray<UTPSHierarchicalCollection*> UTPSHierarchicalCollection::GetAllSubCollections()
{
	/*TArray<UTPSHierarchicalCollection*> children;
	for (auto child : SubCollections)
	{
		children.Add(child.Value);
	}
	return children;*/
	return SubCollections;
}



UTPSCharacterInstance* UTPSHierarchicalCollection::GetMember(int targetIdNumber)
{
	if (targetIdNumber >= Members.Num()) { return nullptr; }

	return Members[targetIdNumber];
}

TArray<UTPSCharacterInstance*> UTPSHierarchicalCollection::GetAllMembers()
{
	/*TArray<UTPSCharacterInstance*> children;
	for (auto child : Members)
	{
		children.Add(child.Value);
	}
	return children;*/
	return Members;
}


void UTPSHierarchicalCollection::UpdateUnitIdForMember(UTPSCharacterInstance* member, int id)
{
	member->Identity.UnitID.UnitNumber = id;
	member->Identity.UnitID.TeamID = UnitID.TeamID;

	member->Identity.UnitID.Hierarchy = FTPSUnitHierarchy::AppendLevelNumber(
		UnitID.Hierarchy, UnitID.UnitLevel, UnitID.UnitNumber);
}

void UTPSHierarchicalCollection::UpdateUnitIdForSubCollection(UTPSHierarchicalCollection* subCollection, int id)
{
	subCollection->UnitID.UnitNumber = id;
	subCollection->UnitID.TeamID     = UnitID.TeamID;

	subCollection->UnitID.Hierarchy  = FTPSUnitHierarchy::AppendLevelNumber(
		UnitID.Hierarchy, UnitID.UnitLevel, UnitID.UnitNumber);

	for (auto subSubMember : subCollection->GetAllMembers())
	{
		subCollection->UpdateUnitIdForMember(subSubMember, subSubMember->Identity.UnitID.UnitNumber);
	}

	for (auto subSubUnit : subCollection->GetAllSubCollections())
	{
		subCollection->UpdateUnitIdForSubCollection(subSubUnit, subSubUnit->UnitID.UnitNumber);
	}

	subCollection->ParentCollection = this;
}


ETPSHierarchicalLevel UTPSHierarchicalCollection::LevelDown(ETPSHierarchicalLevel level)
{
	return _TPSLevel_Down(level);
}

ETPSHierarchicalLevel UTPSHierarchicalCollection::LevelUp(ETPSHierarchicalLevel level)
{
	return _TPSLevel_Up(level);
}

