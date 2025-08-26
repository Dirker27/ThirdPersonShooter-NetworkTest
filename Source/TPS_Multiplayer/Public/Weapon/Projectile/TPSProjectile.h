// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Character/TPSCharacter.h"
#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"

#include "Weapon/Ammunition/TPSAmmunitionType.h"

#include "TPSProjectile.generated.h"

UCLASS()
class TPS_MULTIPLAYER_API ATPSProjectile : public AActor
{
    GENERATED_BODY()

public:
    ATPSProjectile();

    // UE Implementables
protected:
    virtual void BeginPlay() override;
    virtual void Tick(float deltaSeconds) override;
    //virtual bool IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<USphereComponent> CollisionComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UStaticMeshComponent> Mesh;

#if WITH_EDITORONLY_DATA
    UPROPERTY()
    TObjectPtr<UArrowComponent> ArrowComponent;
#endif

    // Simple Decal to apply to target surface/character on hit.
    //   Will OVERRIDE any applied impact effects if set.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<UMaterial*> HitDecals;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector HitDecalSize;

//~ ============================================================= ~//
//  ATTRIBUTES
//~ ============================================================= ~//
public:
    ////////////////////////////////////////////////////////
    // Identity
    
    // Name
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSProjectile|Identity")
    FName Name;
    //
    // Type
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSProjectile|Identity")
    TEnumAsByte<ETPSAmmunitionType> Type;


    ////////////////////////////////////////////////////////
    // Configuration
    
    // Initial Launch Velocity [m/s]
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSProjectile|Configuration")
    float InitialVelocity; 
    //
    // Lifetime Seconds (TTL)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSProjectile|Configuration")
    float LifetimeSeconds = 5;
    //
    // How much velocity is lost on hit (divides current velocity)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSProjectile|Configuration", meta = (EditCondition = "ShouldDestroyOnHit == false", EditConditionHides))
    float OnHitVelocityDamper = 3.f;
    //
    // Should Destroy on first impact.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSProjectile|Configuration")
    bool ShouldDestroyOnHit = true;
    //
    // On-Hit Effects
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSProjectile|Configuration")
    TArray<TSubclassOf<UGameplayEffect>> AppliedEffects;
    //
    // Show Debug Collision Trace
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSProjectile|Configuration")
    bool ShowCollisionTrace = false;


    ////////////////////////////////////////////////////////
    // State
    
    // EquipmentOwner (who shot me?)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSProjectile|State")
    ATPSCharacter* OwningCharacter;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UAbilitySystemComponent> OwnerAsc;

    // Lifetime
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TPSProjectile|State")
    float ElapsedLifetimeSeconds;

//~ ============================================================= ~//
//  BEHAVIOR
//~ ============================================================= ~//
public:
    UFUNCTION(BlueprintCallable)
    void Launch();
    UFUNCTION(BlueprintImplementableEvent)
    void OnLaunch();

protected:
    UFUNCTION(BlueprintCallable)
    bool DetectCollisionByLineTrace(const float deltaSeconds, FHitResult& outHitResult);

    void CharacterHit(ATPSCharacter* character, FHitResult hit);
    UFUNCTION(BlueprintImplementableEvent)
    void OnCharacterHit(ATPSCharacter* character, FHitResult hit);

    void SurfaceHit(AActor* actor, FHitResult hit);
    UFUNCTION(BlueprintImplementableEvent)
    void OnSurfaceHit(AActor* actor, FHitResult hit);
};