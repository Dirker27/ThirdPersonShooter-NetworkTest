// (C) ToasterCat Studios 2025

#include "Army/Unit/TPSHierarchicalCollection.h"

#include "Net/UnrealNetwork.h"

UTPSHierarchicalCollection::UTPSHierarchicalCollection()
{
}

void UTPSHierarchicalCollection::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, UnitID);
	DOREPLIFETIME(ThisClass, Hierarchy);
	DOREPLIFETIME(ThisClass, UnitLevel);
	DOREPLIFETIME(ThisClass, UnitNumber);
	DOREPLIFETIME(ThisClass, Members);
	DOREPLIFETIME(ThisClass, ParentCollection);
	DOREPLIFETIME(ThisClass, SubCollections);
}

bool UTPSHierarchicalCollection::IsSupportedForNetworking() const
{
	return true;
}



bool UTPSHierarchicalCollection::AddMember(UTPSCharacterInstance* member)
{
	if (CanAddMember(member))
	{
		int idNumber = Members.Add(member);
		//member->AssignToUnit(this);

		UnitUpdate.Broadcast();
		return true;
	}
	return false;
}

bool UTPSHierarchicalCollection::AddSubCollection(UTPSHierarchicalCollection* subCollection)
{
	if (CanAddSubCollection(subCollection))
	{
		int index = SubCollections.Add(subCollection);
		UnitUpdate.Broadcast();
		return true;
	}
	return false;
}

bool UTPSHierarchicalCollection::RemoveMember(const int idNumber)
{
	if (idNumber >= Members.Num()) { return false; }

	if (auto member = Members[idNumber])
	{
		member->ClearAssignment();

		UnitUpdate.Broadcast();
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
		UnitUpdate.Broadcast();
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



UTPSCharacterInstance* UTPSHierarchicalCollection::GetMember(int targetIdNumber) const
{
	if (targetIdNumber >= Members.Num()) { return nullptr; }

	return Members[targetIdNumber];
}

TArray<UTPSCharacterInstance*> UTPSHierarchicalCollection::GetAllMembers() const
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

