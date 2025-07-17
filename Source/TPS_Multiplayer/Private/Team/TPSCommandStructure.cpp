// (C) ToasterCat Studios 2025

#include "Team/TPSCommandStructure.h"

UTPSCommandStructure::UTPSCommandStructure()
{
}


void UTPSCommandStructure::GrantLeader(UTPSCharacterInstance* member)
{
	UpdateUnitIDForMember(member, 0);

	Leader = member;
}

UTPSCharacterInstance* UTPSCommandStructure::GetLeader() const
{
	return Leader;
}
