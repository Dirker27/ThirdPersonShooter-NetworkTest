// (C) ToasterCat Studios 2025

#include "Behavior/TPSMountable.h"

#include "Net/UnrealNetwork.h"

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

    if (IsValid(MountPoint)) {
        Mount(MountPoint);
    }
}


void ATPSMountableActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}


void ATPSMountableActor::Mount(UTPSMountPoint* target)
{
    if (!IsValid(target) || !IsValid(target->Target.ParentComponent.Get())) { return; }

    MountPoint = target;

    AttachToComponent(
        target,//target->Target.ParentComponent.Get(),
        FAttachmentTransformRules::SnapToTargetNotIncludingScale,
        FName());//target->Target.SocketName);

    SetActorRelativeLocation(MountOffset);
    //SetActorRelativeRotation(target->Offset.RelativeEulerRotation.Rotation());

    OnMount();
    UE_LOG(LogTemp, Log, TEXT("Mounted to Target[%s]-[%s] with Offset[%s]."),
        *target->Target.ParentComponent.Get()->GetName(), *target->Target.SocketName.ToString(),
        *target->Offset.RelativeLocation.ToString());
}

void ATPSMountableActor::UnMount()
{
    MountPoint = nullptr;

    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

    OnUnMount();
    UE_LOG(LogTemp, Log, TEXT("Un-Mounted."));
}