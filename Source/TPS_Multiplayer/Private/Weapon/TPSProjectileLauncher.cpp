// (C) ToasterCat Studios 2025

#include "Weapon/TPSProjectileLauncher.h"

#include "Util/TPSFunctionLibrary.h"

ATPSProjectileLauncher::ATPSProjectileLauncher()
{
	Muzzle = CreateDefaultSubobject<UTPSMountPoint>(TEXT("Muzzle"));
	Muzzle->SetupAttachment(Mesh);

	ShellEjectPort = CreateDefaultSubobject<UTPSMountPoint>(TEXT("ShellEjectPort"));
	ShellEjectPort->SetupAttachment(Mesh);

	#if WITH_EDITORONLY_DATA
	ArrowComponent->SetupAttachment(Muzzle);
	#endif
}

void ATPSProjectileLauncher::StartUse()
{
	Super::StartUse();
}

void ATPSProjectileLauncher::StopUse()
{
	Super::StopUse();
}

void ATPSProjectileLauncher::PerformFire()
{
	// Adjust the shooter's aim.
	FRotator adjustedDirection = TargetDirection;
	FVector2D noise = CalculateAccuracyNoise();
	adjustedDirection.Add(noise.X, noise.Y, 0);

	if (ShowDebugTrace)
	{
		FVector debugVector = adjustedDirection.Vector() * 400;
		UTPSFunctionLibrary::DrawDebugTrace(this, Muzzle->GetComponentTransform().GetLocation(), debugVector,
			FLinearColor::Yellow, FLinearColor::Red, 2.f);
	}

	int projectileCount = 1;
	if (Configuration->ProjectileBehavior == Spread)
	{
		projectileCount = Configuration->SpreadCount;
	}

	TArray<FRotator> targetTrajectories;
	for (int i = 0; i < projectileCount; i++)
	{
		// Adjust the weapon's spread.
		FRotator spreadDirection = adjustedDirection;
		noise = CalculateSpreadNoise();
		spreadDirection.Add(noise.X, noise.Y, 0);

		if (ShowDebugTrace)
		{
			FVector debugVector = spreadDirection.Vector() * 200;
			UTPSFunctionLibrary::DrawDebugTrace(this, Muzzle->GetComponentTransform().GetLocation(), debugVector,
				FLinearColor::Gray, FLinearColor::Red, 2.f);
		}

		targetTrajectories.Add(spreadDirection);
	}

	LaunchProjectiles_Multicast(targetTrajectories);
}


void ATPSProjectileLauncher::LaunchProjectiles_Multicast_Implementation(const TArray<FRotator> &trajectories)
{
	if (!IsValid(ProjectileTemplate)) { return; }

	FProjectileLaunchInfo launchInfo = FProjectileLaunchInfo();
	for (auto trajectory : trajectories)
	{
		ATPSProjectile* p = GetWorld()->SpawnActor<ATPSProjectile>(ProjectileTemplate,
			Muzzle->GetComponentTransform().GetLocation(), trajectory);

		p->SetReplicates(true);
		p->SetReplicateMovement(true);

		if (IsValid(OwnerAsc.Get()))
		{
			p->OwnerAsc = OwnerAsc;
		}

		if (IsValid(p)) {
			p->Launch();
		}

		launchInfo.ProjectilePaths.Add(trajectory.Vector());
	}
	
	OnFirePerformed(launchInfo);
}


FVector2D ATPSProjectileLauncher::CalculateAccuracyNoise() const
{
	return UTPSFunctionLibrary::CalculateNoise2D(TargetAccuracyTolerance.X, TargetAccuracyTolerance.Y);
}

// Apply MOA Noise (independent of character's accuracy, this is just the Weapon's spread)
FVector2D ATPSProjectileLauncher::CalculateSpreadNoise() const
{
	// MOA == "Minute of Angle" -> 1/60 of a 360-degree.
	//  1 MOA ~= 1 inch @ 100 yards
	float pitchDegrees;
	float yawDegrees;
	if (Configuration->SpreadMode == MOA) {
		pitchDegrees = Configuration->SpreadMOA / 60.f;
		yawDegrees = Configuration->SpreadMOA / 60.f;
	}
	else
	{
		pitchDegrees = Configuration->SpreadDegrees.X;
		yawDegrees = Configuration->SpreadDegrees.Y;
	}

	return UTPSFunctionLibrary::CalculateNoise2D(pitchDegrees, yawDegrees);
}
