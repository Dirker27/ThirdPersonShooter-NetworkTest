// (C) ToasterCat Studios 2025

#include "Weapon/Projectile/TPSProjectile.h"

#include "Character/TPSCharacter.h"
#include "Engine/DamageEvents.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Util/TPSFunctionLibrary.h"

ATPSProjectile::ATPSProjectile()
{
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionComponent->SetSimulatePhysics(true);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	SetRootComponent(CollisionComponent);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(CollisionComponent);

#if WITH_EDITORONLY_DATA
	ArrowComponent = CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	if (ArrowComponent)
	{
		ArrowComponent->ArrowColor = FColor(200, 255, 200);
		ArrowComponent->bTreatAsASprite = true;
		ArrowComponent->SetupAttachment(CollisionComponent);
		ArrowComponent->bIsScreenSizeScaled = true;
		ArrowComponent->SetSimulatePhysics(false);
	}
#endif
}

void ATPSProjectile::BeginPlay()
{
	Super::BeginPlay();

	ElapsedLifetimeSeconds = 0;
}

void ATPSProjectile::Tick(float deltaSeconds)
{
	Super::Tick(deltaSeconds);

	FHitResult hitResult;
	bool isHit = DetectCollisionByLineTrace(deltaSeconds, hitResult);

	if (isHit)
	{
		AActor* hitActor = hitResult.GetActor();
		ATPSCharacter* character = Cast<ATPSCharacter>(hitActor);
		if (IsValid(character))
		{
			UE_LOG(LogTemp, Log, TEXT("Hit Character: [%s]"), *character->Name);
			CharacterHit(character, hitResult);

			// Apply Damage & Effects
			if (HasAuthority())
			{
				//FPointDamageEvent event = FPointDamageEvent();
				//event.Damage = 1.0f;
				//event.HitInfo = hitResult;

				//character->TakeDamage(1.f, event, nullptr, nullptr);

				for (TSubclassOf<UGameplayEffect> e: AppliedEffects)
				{
					UAbilitySystemComponent* asc = character->GetAbilitySystemComponent();
					if (IsValid(asc)) {
						//FGameplayTagContainer tags;
						//e->GetAssetRegistryTags();
						//asc->RemoveActiveEffectsWithTags(tags);

						FGameplayEffectContextHandle context = asc->MakeEffectContext();
						FGameplayEffectSpecHandle spec = asc->MakeOutgoingSpec(e, 1.f, context);
						asc->ApplyGameplayEffectSpecToSelf(*spec.Data.Get(), asc->GetPredictionKeyForNewAction());
					}
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Hit Surface: [%s]"), *hitActor->GetHumanReadableName());
			SurfaceHit(hitActor, hitResult);
		}

		if (ShouldDestroyOnHit)
		{
			Destroy();
		}
		else
		{
			FVector forward = GetVelocity() / OnHitVelocityDamper;
			CollisionComponent->SetPhysicsLinearVelocity(forward, false, "None");
		}
	}

	ElapsedLifetimeSeconds += deltaSeconds;
	if (ElapsedLifetimeSeconds > LifetimeSeconds)
	{
		UE_LOG(LogTemp, Log, TEXT("[TTL] Projectile Destroyed"));
		Destroy();
	}
}


void ATPSProjectile::Launch()
{
	FVector forward = GetActorForwardVector() * InitialVelocity;
	CollisionComponent->SetPhysicsLinearVelocity(forward, true, "None");

	OnLaunch();
}


bool ATPSProjectile::DetectCollisionByLineTrace(const float deltaSeconds, FHitResult& outHitResult) {
	AActor* hitActor = NULL;

	FVector interpVector = GetVelocity() * deltaSeconds;

	FVector startLoc = GetActorLocation();
	FVector endLoc = startLoc + interpVector;

	ETraceTypeQuery channel = TraceTypeQuery_MAX;
	TArray<AActor*> actorsToIgnore;
	EDrawDebugTrace::Type debugTrace = EDrawDebugTrace::Type::ForDuration;

	bool isHit = false;
	if (ShowCollisionTrace)
	{
		isHit = UKismetSystemLibrary::LineTraceSingle(this, startLoc, endLoc,
			channel, false, actorsToIgnore, debugTrace,
			outHitResult,
			true,
			FLinearColor::Green, FLinearColor::Red, 0.5f);
	}
	else
	{
		isHit = UKismetSystemLibrary::LineTraceSingle(this, startLoc, endLoc,
			channel, false, actorsToIgnore, debugTrace,
			outHitResult,
			true);
	}

	return isHit;
}

void ATPSProjectile::CharacterHit(ATPSCharacter* character, FHitResult hit)
{
	if (!hit.IsValidBlockingHit() || !IsValid(character)) { return; }

	// Carry-Over Physics to Mesh
	if (HasAuthority() && hit.GetComponent()->IsSimulatingPhysics())
	{
		FVector impulse = UTPSFunctionLibrary::CalculateImpulseJoules(
			GetVelocity(), CollisionComponent->GetMass());
		hit.GetComponent()->AddImpulseAtLocation(impulse, hit.Location, "None");
	}

	OnCharacterHit(character, hit);
}


void ATPSProjectile::SurfaceHit(AActor* sceneActor, FHitResult hit)
{
	if (!hit.IsValidBlockingHit() 
		|| !IsValid(sceneActor)
		|| !IsValid(hit.GetComponent())) { return; }

	// Carry-Over Physics
	if (HasAuthority() && hit.GetComponent()->IsSimulatingPhysics())
	{
		FVector impulse = UTPSFunctionLibrary::CalculateImpulseJoules(
			GetVelocity(), CollisionComponent->GetMass());
		hit.GetComponent()->AddImpulseAtLocation(impulse, hit.Location, "None");
	}

	//Instigator->GetVelocity();

	// TODO: Target Interface Execution

	OnSurfaceHit(sceneActor, hit);
}
