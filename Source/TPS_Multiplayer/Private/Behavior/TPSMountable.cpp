// (C) ToasterCat Studios 2025

#include "Behavior/TPSMountable.h"

#include "Net/UnrealNetwork.h"

//template<> const FTPSMountOffset FTPSMountOffset::Zero(FVector::Zero(), FVector::Zero());

ATPSMountableActor::ATPSMountableActor()
{
    bReplicates = true;
    bLockLocation = false;
}


void ATPSMountableActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ATPSMountableActor, MountPoint);
}

void ATPSMountableActor::BeginPlay()
{
    Super::BeginPlay();

    if (MountPoint.IsValid()) {
        if (ShouldCaptureOffsetOnStart)
        {
            MountOffset.RelativeLocation = GetTransform().GetLocation();
            MountOffset.RelativeLocation = GetTransform().GetRotation().Euler();
        }
        Mount(MountPoint.Get());
    }
}


void ATPSMountableActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}


void ATPSMountableActor::Mount(UTPSMountPoint* target)
{
    MountWithOffset(target, MountOffset);
}

void ATPSMountableActor::MountWithOffset(UTPSMountPoint* target, FTPSMountOffset offset)
{
    if (!IsValid(target) || !IsValid(target->Target.ParentComponent.Get())) { return; }

    MountPoint = target;

    AttachToComponent(
        target,
        FAttachmentTransformRules::SnapToTargetNotIncludingScale);

    SetActorRelativeLocation(offset.RelativeLocation);
    if (ShouldApplyRotation) {
        SetActorRelativeRotation(offset.RelativeEulerRotation.Rotation());
    }

    OnMount();
    UE_LOG(LogTemp, Log, TEXT("Actor[%s] Mounted to Target[%s] Bone[%s]-[%s] with Offset[%s]."),
        *GetName(),
        *target->GetName(),
        *target->Target.ParentComponent.Get()->GetName(), *target->Target.SocketName.ToString(),
        *offset.RelativeLocation.ToString());
}

void ATPSMountableActor::UnMount()
{
    MountPoint = nullptr;

    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

    OnUnMount();
    UE_LOG(LogTemp, Log, TEXT("Un-Mounted."));
}
