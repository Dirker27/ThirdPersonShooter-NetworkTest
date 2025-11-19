// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "TPSMountPoint.generated.h"

USTRUCT(BlueprintType)
struct TPS_MULTIPLAYER_API FTPSMountTarget
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MountPoint|Schema")
    TWeakObjectPtr<USceneComponent> ParentComponent = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MountPoint|Schema")
    FName SocketName = FName();
};

USTRUCT(BlueprintType)
struct TPS_MULTIPLAYER_API FTPSMountOffset
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MountPoint|Schema")
    FVector RelativeLocation = FVector::Zero();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MountPoint|Schema")
    FRotator RelativeRotation = FRotator::ZeroRotator;

    /*TPS_MULTIPLAYER_API static inline const FTPSMountOffset Zero = {
        FVector::Zero(),
        FVector::Zero()
    };*/
};

UCLASS(BlueprintType)
class TPS_MULTIPLAYER_API UTPSMountPoint : public USceneComponent
{
    GENERATED_BODY()

public:
    UTPSMountPoint();

    void BindToParentSocket(USceneComponent* InParent, FName InSocketName = NAME_None);

//~ ============================================================= ~//
//  Schema
//~ ============================================================= ~//
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MountPoint|Schema")
    FTPSMountTarget Target;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MountPoint|Schema")
    FTPSMountOffset Offset = FTPSMountOffset();

//~ ============================================================= ~//
//  BEHAVIOR
//~ ============================================================= ~//
public:
    UFUNCTION(BlueprintCallable)
    FVector GetCurrentPosition() const;
    UFUNCTION(BlueprintCallable)
    FVector GetCurrentEulerRotation() const;
};