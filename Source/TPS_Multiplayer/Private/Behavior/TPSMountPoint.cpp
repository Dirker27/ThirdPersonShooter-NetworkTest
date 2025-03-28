// (C) ToasterCat Studios 2025

#include "Behavior/TPSMountPoint.h"

#include "Components/BoxComponent.h"

UTPSMountPoint::UTPSMountPoint()
{
    //bVisible = true;
    //bVisualizeComponent = true;
    bHiddenInGame = false;
    //Offset.RelativeLocation = GetRelativeLocation();
    //Offset.RelativeEulerRotation = GetComponentRotation().Euler();
}

void UTPSMountPoint::Attach(USceneComponent* InParent, FName InSocketName)
{
    Target.ParentComponent = InParent;
    Target.SocketName = InSocketName;

    //Offset.RelativeLocation = InParent->GetSocketLocation(InSocketName) - GetComponentLocation();
    //Offset.RelativeEulerRotation = InParent->GetComponentRotation().Euler() - GetCurrentEulerRotation();

    SetupAttachment(InParent, InSocketName);

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