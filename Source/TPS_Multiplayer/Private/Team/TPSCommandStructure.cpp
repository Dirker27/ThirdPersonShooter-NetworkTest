// (C) ToasterCat Studios 2025

#include "Team/TPSCommandStructure.h"

UTPSCommandStructure::UTPSCommandStructure()
{
}


void UTPSCommandStructure::SetLeader(UTPSCharacterInstance* member)
{
	Leader = member;
}

UTPSCharacterInstance* UTPSCommandStructure::GetLeader() const
{
	return Leader.Get();
}
