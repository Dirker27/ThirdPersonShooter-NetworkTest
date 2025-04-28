// (C) ToasterCat Studios 2025

#include "Behavior/TPSMountPoint.h"

UTPSMountPoint::UTPSMountPoint()
{
    //bVisualizeComponent = true;
    //bHiddenInGame = false;
    //SetIsReplicated(true);
}

void UTPSMountPoint::BindToParentSocket(USceneComponent* InParent, FName InSocketName)
{
    Target.ParentComponent = InParent;
    Target.SocketName = InSocketName;

    SetupAttachment(InParent, InSocketName);

    SetRelativeLocation(Offset.RelativeLocation);
    SetRelativeRotation(Offset.RelativeRotation);

    /*UE_LOG(LogTemp, Log, TEXT("MountPoint[%s] attached to parent with Offset Location[%s]-Rotation[%s]."),
        *GetName(),
        *Offset.RelativeLocation.ToString(), *Offset.RelativeRotation.ToString());*/
}


FVector UTPSMountPoint::GetCurrentPosition() const
{
    return GetComponentTransform().GetLocation();
}

FVector UTPSMountPoint::GetCurrentEulerRotation() const
{
    return GetComponentTransform().GetRotation().Euler();
}