// (C) ToasterCat Studios 2025

#include "Team/TPSCommandGroup.h"

#include "Model/TPSHierarchicalCollectionLevel.h"

static int _incrementingCommandID;
UTPSCommandGroup::UTPSCommandGroup()
{
	Level = ANY;

	ID = _incrementingCommandID++;
}


void UTPSCommandGroup::Command()
{
	// TODO: Command Members and Sub-Collections
}
