// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"

#include "Components/SphereComponent.h"

#include "TPSMountPoint.generated.h"

USTRUCT(BlueprintType)
struct TPS_MULTIPLAYER_API FTPSMountTarget
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MountPoint|Configuration")
    TWeakObjectPtr<USceneComponent> ParentComponent = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MountPoint|Configuration")
    FName SocketName = FName();
};

USTRUCT(BlueprintType)
struct TPS_MULTIPLAYER_API FTPSMountOffset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MountPoint|Configuration")
    FVector RelativeLocation = FVector::Zero();
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MountPoint|Configuration")
    FVector RelativeEulerRotation = FVector::Zero();
};

UCLASS(BlueprintType)
class TPS_MULTIPLAYER_API UTPSMountPoint : public USceneComponent
{
    GENERATED_BODY()

public:
    UTPSMountPoint();

    void Attach(USceneComponent* InParent, FName InSocketName = NAME_None);

//~ ============================================================= ~//
//  Configuration
//~ ============================================================= ~//
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MountPoint|Configuration")
    FTPSMountTarget Target;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MountPoint|Configuration")
    FTPSMountOffset Offset;

//~ ============================================================= ~//
//  BEHAVIOR
//~ ============================================================= ~//
public:
    UFUNCTION(BlueprintCallable)
    FVector GetCurrentPosition() const;
    UFUNCTION(BlueprintCallable)
    FVector GetCurrentEulerRotation() const;
};