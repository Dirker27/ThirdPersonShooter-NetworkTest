// (C) ToasterCat Studios 2025

#include "Model/TPSHierarchicalCollection.h"

UTPSHierarchicalCollection::UTPSHierarchicalCollection()
{
	
}


bool UTPSHierarchicalCollection::Add(ATPSCharacter* member)
{
	if (CanAddToCollection(member))
	{
		Members.Add(member);
		return true;
	}
	return false;
}

// TODO: Guard against cyclical hierarchies
bool UTPSHierarchicalCollection::Add(UTPSHierarchicalCollection* subCollection)
{
	if (!ChildCollections.Contains(subCollection))
	{
		ChildCollections.Add(subCollection);
		return true;
	}
	return false;
}

bool UTPSHierarchicalCollection::Remove(ATPSCharacter* member)
{
	if (!IsValid(member))
	{
		return false;
	}
	return (Members.Remove(member) > 0);
}

bool UTPSHierarchicalCollection::Remove(UTPSHierarchicalCollection* subCollection)
{
	if (!IsValid(subCollection))
	{
		return false;
	}
	return (ChildCollections.Remove(subCollection) > 0);
}


bool UTPSHierarchicalCollection::CanAddToCollection(ATPSCharacter* member) const
{
	if (!IsValid(member))
	{
		return false;
	}

	return !Members.Contains(member);
}

// TODO: Guard against cyclical hierarchies
bool UTPSHierarchicalCollection::CanAddToCollection(UTPSHierarchicalCollection* subCollection) const
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
