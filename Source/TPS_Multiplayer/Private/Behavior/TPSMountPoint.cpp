// (C) ToasterCat Studios 2025

#include "Behavior/TPSMountPoint.h"

UTPSMountPoint::UTPSMountPoint()
{
    OffsetPosition = GetCurrentPosition();
    OffsetEulerRotation = GetCurrentEulerRotation();
}

void UTPSMountPoint::MountToTarget(AActor* actor)
{
    if (!TargetParentComponent.IsValid()) { return; }

    actor->AttachToComponent(
        TargetParentComponent.Get(),
        FAttachmentTransformRules::SnapToTargetNotIncludingScale,
        TargetSocketName);

    actor->SetActorRelativeLocation(OffsetPosition);
    actor->SetActorRelativeRotation(OffsetEulerRotation.Rotation());
}

void UTPSMountPoint::UnMountFromTarget(AActor* actor)
{
    actor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}



FVector UTPSMountPoint::GetCurrentPosition() const
{
    return GetComponentLocation();
}

FVector UTPSMountPoint::GetCurrentEulerRotation() const
{
    return GetComponentRotation().Euler();
}