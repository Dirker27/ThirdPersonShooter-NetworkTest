// (C) ToasterCat Studios 2025

#include "Model/TPSHierarchicalCollection.h"

UTPSHierarchicalCollection::UTPSHierarchicalCollection()
{
	
}


bool UTPSHierarchicalCollection::AddMember(UTPSCharacterInstance* member)
{
	if (CanAddMember(member))
	{
		uint8 id = GetNextAvailableIDNumberForMember();
		Members.Add(id, member);
		UpdateUnitIDForMember(member, id);
		return true;
	}
	return false;
}

bool UTPSHierarchicalCollection::AddSubCollection(UTPSHierarchicalCollection* subCollection)
{
	if (CanAddSubCollection(subCollection))
	{
		uint8 id = GetNextAvailableIDNumberForSubCollection();
		ChildCollections.Add(id, subCollection);

		UpdateUnitIDForSubCollection(subCollection, id);

		return true;
	}
	return false;
}

bool UTPSHierarchicalCollection::RemoveMember(UTPSCharacterInstance* member)
{
	if (!IsValid(member))
	{
		return false;
	}

	if (Members.Remove(member->Identity->UnitID.UnitNumber) > 0)
	{
		member->Identity->UnitID.Hierarchy.Empty();
		member->Identity->UnitID.UnitNumber = 0;

		return true;
	}
	return false;
}

bool UTPSHierarchicalCollection::RemoveSubCollection(UTPSHierarchicalCollection* subCollection)
{
	if (!IsValid(subCollection))
	{
		return false;
	}
	if (ChildCollections.Remove(subCollection->UnitID.UnitNumber) > 0)
	{
		subCollection->UnitID.Hierarchy.Empty();
		subCollection->UnitID.UnitNumber = 0;
		return true;
	}
	return false;
}

bool UTPSHierarchicalCollection::CanAddMember(UTPSCharacterInstance* member) const
{
	if (!IsValid(member))
	{
		return false;
	}

	if (Members.Num() > 254)
	{
		return false;
	}

	return true;
	//return !Members.Contains(member);
}

// TODO: Guard against cyclical hierarchies
bool UTPSHierarchicalCollection::CanAddSubCollection(UTPSHierarchicalCollection* subCollection) const
{
	if (!IsValid(subCollection))
	{
		return false;
	}

	if (UnitID.UnitLevel <= subCollection->UnitID.UnitLevel)
	{
		return false;
	}

	if (ChildCollections.Num() > 254)
	{
		return false;
	}

	return true;
	//return !ChildCollections.Contains(subCollection);
}


UTPSHierarchicalCollection* UTPSHierarchicalCollection::GetChildCollection(FTPSUnitID id)
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
		if (auto childUnit = ChildCollections.Find(*targetChildUnitNumber))
		{
			return childUnit->Get()->GetChildCollection(id);
		}
	}

	return nullptr;
}

TArray<UTPSHierarchicalCollection*> UTPSHierarchicalCollection::GetAllChildCollections()
{
	TArray<UTPSHierarchicalCollection*> children;
	for (auto child : ChildCollections)
	{
		children.Add(child.Value);
	}
	return children;
}



UTPSCharacterInstance* UTPSHierarchicalCollection::GetMember(uint8 targetIdNumber)
{
	if (auto member = Members.Find(targetIdNumber))
	{
		return *member;
	}
	return nullptr;
}

TArray<UTPSCharacterInstance*> UTPSHierarchicalCollection::GetAllMembers()
{
	TArray<UTPSCharacterInstance*> children;
	for (auto child : Members)
	{
		children.Add(child.Value);
	}
	return children;
}


void UTPSHierarchicalCollection::UpdateUnitIDForMember(UTPSCharacterInstance* member, uint8 id)
{
	member->Identity->UnitID.UnitNumber = id;
	member->Identity->UnitID.TeamID = UnitID.TeamID;

	member->Identity->UnitID.Hierarchy = UnitID.Hierarchy;
	member->Identity->UnitID.Hierarchy.Add(UnitID.UnitLevel, UnitID.UnitNumber);
}

void UTPSHierarchicalCollection::UpdateUnitIDForSubCollection(UTPSHierarchicalCollection* subCollection, uint8 id)
{
	subCollection->UnitID.UnitNumber = id;
	subCollection->UnitID.TeamID     = UnitID.TeamID;

	subCollection->UnitID.Hierarchy  = UnitID.Hierarchy;
	subCollection->UnitID.Hierarchy.Add(UnitID.UnitLevel, UnitID.UnitNumber);

	for (auto subSubMember : subCollection->GetAllMembers())
	{
		subCollection->UpdateUnitIDForMember(subSubMember, subSubMember->Identity->UnitID.UnitNumber);
	}

	for (auto subSubUnit : subCollection->GetAllChildCollections())
	{
		subCollection->UpdateUnitIDForSubCollection(subSubUnit, subSubUnit->UnitID.UnitNumber);
	}

	subCollection->ParentCollection = this;
}



// TODO: Avoid linear probe
uint8 UTPSHierarchicalCollection::GetNextAvailableIDNumberForMember() const
{
	int i = 0;
	while (Members.Contains(i))
	{
		i++;
	}

	return i;
}

// TODO: Avoid linear probe
uint8 UTPSHierarchicalCollection::GetNextAvailableIDNumberForSubCollection() const
{
	int i = 1;
	while (ChildCollections.Contains(i))
	{
		i++;
	}

	return i;
}





ETPSHierarchicalLevel UTPSHierarchicalCollection::LevelDown(ETPSHierarchicalLevel level)
{
	return _TPSLevel_Down(level);
}

ETPSHierarchicalLevel UTPSHierarchicalCollection::LevelUp(ETPSHierarchicalLevel level)
{
	return _TPSLevel_Up(level);
}

