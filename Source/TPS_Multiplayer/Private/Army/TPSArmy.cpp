// (C) ToasterCat Studios 2025

#include "Army/TPSArmy.h"

#include "Net/UnrealNetwork.h"

void UTPSArmyInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UTPSArmyInstance, TeamID);
    DOREPLIFETIME(UTPSArmyInstance, RootUnit);
}

bool UTPSArmyInstance::IsSupportedForNetworking() const
{
    return true;
}
