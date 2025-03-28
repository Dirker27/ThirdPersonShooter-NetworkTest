// (C) ToasterCat Studios 2025

#include "Weapon/TPSRangedWeapon.h"

#include "Net/UnrealNetwork.h"

ATPSRangedWeapon::ATPSRangedWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

    Configuration = CreateDefaultSubobject<UTPSWeaponConfiguration>(TEXT("DefaultConfiguration"));
}

void ATPSRangedWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATPSRangedWeapon, MountPoint);

	DOREPLIFETIME(ATPSRangedWeapon, CurrentAmmunitionCount);
	DOREPLIFETIME(ATPSRangedWeapon, IsReloading);
	DOREPLIFETIME(ATPSRangedWeapon, IsAiming);
	DOREPLIFETIME(ATPSRangedWeapon, IsFiring);
}

void ATPSRangedWeapon::BeginPlay()
{
    Super::BeginPlay();

    CurrentAmmunitionCount = Configuration->AmmunitionCapacity;

	CurrentWeaponState = Ready;
	PreviousWeaponState = Arming;
	IsFiring = false;
	IsAiming = false;
	IsEquipped = false;
	IsReloading = false;

	HasEverFired = false;

	HasTriggerCompleted = false;
	TimeLastFired = 0;
	SuccessiveFireCount = 0;
}

void ATPSRangedWeapon::BeginDestroy()
{
	Super::BeginDestroy();
}


void ATPSRangedWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Performed in GameplayAbility
	//
	/*if (IsFiring)
	{
		if (CanFire())
		{
			Fire();
		}
		//else { UE_LOG(LogTemp, Log, TEXT("-<click>-")); }
	}*/
}

//~ ============================================================= ~//
//  Usable / Equipment Interfaces
//~ ============================================================= ~//

void ATPSRangedWeapon::StartUse()
{
    Super::StartUse();

    IsFiring = true;
	ApplyWeaponState(Firing);
}

void ATPSRangedWeapon::StopUse()
{
    Super::StopUse();

    IsFiring = false;
	ApplyWeaponState(PreviousWeaponState);

	// Reset Trigger
	HasTriggerCompleted = false;
	SuccessiveFireCount = 0;
}


void ATPSRangedWeapon::Equip()
{
    Super::Equip();
}

void ATPSRangedWeapon::UnEquip()
{
    Super::UnEquip();
}

//~ ============================================================= ~//
//  Fire Control
//~ ============================================================= ~//

void ATPSRangedWeapon::Fire()
{
	PerformFire();

	// apply state updates
	CurrentAmmunitionCount--;
	TimeLastFired = FApp::GetGameTime();
	SuccessiveFireCount++;
	HasEverFired = true;

	// determine if trigger sequence has completed (semi/burst modes)
	switch (Configuration->FireMode)
	{
	case SingleShot:
		HasTriggerCompleted = true;
		break;
	case Burst:
		if (SuccessiveFireCount >= Configuration->BurstFireCount)
		{
			HasTriggerCompleted = true;
		}
		break;
	case FullAuto:
	default:
		break;
	}
	if (CurrentAmmunitionCount <= 0)
	{
		HasTriggerCompleted = true;
	}

	OnFire();
}

bool ATPSRangedWeapon::CanFire()
{
	if (HasTriggerCompleted)
	{
		return false;
	}

	float cycleRateSeconds = 1.0f / Configuration -> CycleRate;

	float currentGameTime = FApp::GetGameTime();
	if (CurrentAmmunitionCount > 0
		&& (currentGameTime - cycleRateSeconds) > TimeLastFired) {
		return true;
	}
	return false;
}

//~ ============================================================= ~//
//  Loading Control
//~ ============================================================= ~//

void ATPSRangedWeapon::StartReload()
{
    ApplyWeaponState(Arming);

	if (CurrentAmmunitionCount > 0 && Configuration->HasChamber)
	{
		CurrentAmmunitionCount = 1;
	}
}

void ATPSRangedWeapon::CommitReload(int newAmmunitionCount)
{
	CurrentAmmunitionCount += newAmmunitionCount;
	ApplyWeaponState(Ready);
}

void ATPSRangedWeapon::ApplyWeaponState(ETPSWeaponState newState)
{
	if (newState == CurrentWeaponState) { return; }

	PreviousWeaponState = CurrentWeaponState;
	CurrentWeaponState = newState;
}
