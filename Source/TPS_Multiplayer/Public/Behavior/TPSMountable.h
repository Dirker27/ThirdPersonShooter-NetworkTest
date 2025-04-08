// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "Behavior/TPSMountPoint.h"

#include "TPSMountable.generated.h"

// TODO: Migrate to Interface?
UCLASS()
class TPS_MULTIPLAYER_API ATPSMountableActor : public AActor
{
    GENERATED_BODY()

public:
    ATPSMountableActor();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;

//~ ============================================================= ~//
//  ATTRIBUTES
//~ ============================================================= ~//
public:
    //- Configuration -----------------------------------=
    //
    // Offset to apply to ALL mounting targets
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mountable|Configuration")
    FTPSMountOffset GlobalMountOffset;

    //- State ---------------------------------------------=
    //
    // Active(?) Mount Point
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mountable|State", Replicated)
    TWeakObjectPtr<UTPSMountPoint> MountPoint = nullptr;
    //
    // Is Currently Mounted to a MountPoint Target.
    UFUNCTION(BlueprintCallable)
    bool IsMounted() { return MountPoint.IsValid(); }

//~ ============================================================= ~//
//  BEHAVIOR
//~ ============================================================= ~//
public:
    UFUNCTION(BlueprintCallable)
    void Mount (UTPSMountPoint* target);
    UFUNCTION(BlueprintCallable)
    void MountWithOffset(UTPSMountPoint* target, FTPSMountOffset offset);
    UFUNCTION(BlueprintImplementableEvent)
    void OnMount();

    UFUNCTION(BlueprintCallable)
    void UnMount();
    UFUNCTION(BlueprintImplementableEvent)
    void OnUnMount();
};