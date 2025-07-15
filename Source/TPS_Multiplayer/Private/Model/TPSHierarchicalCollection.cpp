// (C) ToasterCat Studios 2025

#include "Model/TPSHierarchicalCollection.h"

UTPSHierarchicalCollection::UTPSHierarchicalCollection()
{
	
}


bool UTPSHierarchicalCollection::AddMember(UTPSCharacterInstance* member)
{
	if (CanAddMember(member))
	{
		Members.Add(member);
		return true;
	}
	return false;
}

// TODO: Guard against cyclical hierarchies
bool UTPSHierarchicalCollection::AddSubCollection(UTPSHierarchicalCollection* subCollection)
{
	if (!ChildCollections.Contains(subCollection))
	{
		ChildCollections.Add(subCollection);
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
	return (Members.Remove(member) > 0);
}

bool UTPSHierarchicalCollection::RemoveSubCollection(UTPSHierarchicalCollection* subCollection)
{
	if (!IsValid(subCollection))
	{
		return false;
	}
	return (ChildCollections.Remove(subCollection) > 0);
}


bool UTPSHierarchicalCollection::CanAddMember(UTPSCharacterInstance* member) const
{
	if (!IsValid(member))
	{
		return false;
	}

	return !Members.Contains(member);
}

// TODO: Guard against cyclical hierarchies
bool UTPSHierarchicalCollection::CanAddSubCollection(UTPSHierarchicalCollection* subCollection) const
{
	if (!IsValid(subCollection))
	{
		return false;
	}

	if (Level <= subCollection->Level)
	{
		return false;
	}

	return !ChildCollections.Contains(subCollection);
}
