// (C) ToasterCat Studios 2025

#include "Behavior/TPSMountable.h"

#include "Net/UnrealNetwork.h"

ATPSMountableActor::ATPSMountableActor()
{
    bReplicates = true;
}


void ATPSMountableActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ATPSMountableActor, MountPoint);
}

void ATPSMountableActor::Tick(float DeltaSeconds)
{
    /*if (IsValid(MountPoint))
    {
        SetActorLocationAndRotation(
            MountPoint->GetCurrentPosition(),
            MountPoint->GetActorTransform().GetRotation()
        );
    }*/
}


void ATPSMountableActor::Mount(UTPSMountPoint* target)
{
    MountPoint = target;
    MountPoint->MountToTarget(this);

    OnMount();
    UE_LOG(LogTemp, Log, TEXT("Mounted to Target[%s]-[%s]."),
        *target->TargetParentComponent.Get()->GetName(), *target->TargetSocketName.ToString());
}

void ATPSMountableActor::UnMount()
{
    MountPoint->UnMountFromTarget(this);
    MountPoint = nullptr;

    OnUnMount();
    UE_LOG(LogTemp, Log, TEXT("Un-Mounted."));
}