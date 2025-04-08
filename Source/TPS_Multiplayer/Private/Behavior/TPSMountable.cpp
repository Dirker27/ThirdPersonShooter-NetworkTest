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
        Mount(MountPoint.Get());
    }
}


void ATPSMountableActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}


void ATPSMountableActor::Mount(UTPSMountPoint* target)
{
    MountWithOffset(target, target->Offset);
}

void ATPSMountableActor::MountWithOffset(UTPSMountPoint* target, FTPSMountOffset offset)
{
    if (!IsValid(target) || !IsValid(target->Target.ParentComponent.Get())) { return; }

    MountPoint = target;

    // Mount to Target Mesh/Bone
    AttachToComponent(
        target->Target.ParentComponent.Get(),
        FAttachmentTransformRules::SnapToTargetNotIncludingScale,
        target->Target.SocketName);

    // Apply Offset
    SetActorRelativeLocation(offset.RelativeLocation + GlobalMountOffset.RelativeLocation);
    SetActorRelativeRotation(offset.RelativeRotation + GlobalMountOffset.RelativeRotation);

    OnMount();
    UE_LOG(LogTemp, Log, TEXT("Mountable[%s] Mounted to Target[%s] Bone[%s]-[%s] with Offset[%s]."),
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
    UE_LOG(LogTemp, Log, TEXT("Mountable[%s] Un-Mounted."),
        *GetName());
}
