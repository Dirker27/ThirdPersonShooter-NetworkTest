// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "Components/SphereComponent.h"

#include "TPSMountPoint.generated.h"

USTRUCT(BlueprintType)
struct TPS_MULTIPLAYER_API FTPSMountTarget
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MountPoint|Configuration")
    TWeakObjectPtr<USceneComponent> TargetParentComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MountPoint|Configuration")
    FName TargetSocketName;
};

USTRUCT(BlueprintType)
struct TPS_MULTIPLAYER_API FTPSMountOffset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MountPoint|Configuration")
    FVector OffsetPosition = FVector::Zero();
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MountPoint|Configuration")
    FVector OffsetEulerRotation = FVector::Zero();
};

UCLASS(BlueprintType)
class TPS_MULTIPLAYER_API UTPSMountPoint : public USceneComponent
{
    GENERATED_BODY()

public:
    UTPSMountPoint();

#if WITH_EDITORONLY_DATA
/** Component shown in the editor only to indicate character facing */
    UPROPERTY()
    TObjectPtr<USphereComponent> SphereComponent;
#endif

//~ ============================================================= ~//
//  Configuration
//~ ============================================================= ~//
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MountPoint|Configuration")
    TWeakObjectPtr<USceneComponent> TargetParentComponent;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MountPoint|Configuration")
    FName TargetSocketName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MountPoint|Configuration")
    FTPSMountTarget Target;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MountPoint|Configuration")
    FVector OffsetPosition = FVector::Zero();
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MountPoint|Configuration")
    FVector OffsetEulerRotation = FVector::Zero();

//~ ============================================================= ~//
//  BEHAVIOR
//~ ============================================================= ~//
public:
    UFUNCTION(BlueprintCallable)
    void MountToTarget(AActor* actor);
    UFUNCTION(BlueprintCallable)
    void UnMountFromTarget(AActor* actor);

    UFUNCTION(BlueprintCallable)
    FVector GetCurrentPosition() const;
    UFUNCTION(BlueprintCallable)
    FVector GetCurrentEulerRotation() const;
};