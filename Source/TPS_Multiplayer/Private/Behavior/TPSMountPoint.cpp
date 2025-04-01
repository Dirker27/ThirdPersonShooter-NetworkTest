// (C) ToasterCat Studios 2025

#include "Behavior/TPSMountPoint.h"

UTPSMountPoint::UTPSMountPoint()
{
    //bVisualizeComponent = true;
    bHiddenInGame = false;
}

void UTPSMountPoint::BindToParentSocket(USceneComponent* InParent, FName InSocketName)
{
    Target.ParentComponent = InParent;
    Target.SocketName = InSocketName;

    SetupAttachment(InParent, InSocketName);

    AddRelativeLocation(Offset.RelativeLocation);
    AddRelativeRotation(Offset.RelativeEulerRotation.Rotation());

    UE_LOG(LogTemp, Log, TEXT("MountPoint attached to parent with Offset Location[%s]-Rotation[%s]."),
        *Offset.RelativeLocation.ToString(), *Offset.RelativeEulerRotation.ToString());
}


FVector UTPSMountPoint::GetCurrentPosition() const
{
    return GetComponentTransform().GetLocation();
}

FVector UTPSMountPoint::GetCurrentEulerRotation() const
{
    return GetComponentTransform().GetRotation().Euler();
}