#include "Inventory/TPSEquipableItem.h"

#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"

ATPSEquipableItem::ATPSEquipableItem()
{
	//USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	//SetRootComponent(root);

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	SetRootComponent(CollisionComponent);
	//CollisionComponent->SetupAttachment(root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(CollisionComponent);
	Mesh->SetSimulatePhysics(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

#if WITH_EDITORONLY_DATA
	ArrowComponent = CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	if (ArrowComponent)
	{
		ArrowComponent->ArrowColor = FColor(150, 200, 255);
		ArrowComponent->bTreatAsASprite = true;
		ArrowComponent->SetupAttachment(CollisionComponent);
		ArrowComponent->bIsScreenSizeScaled = true;
		ArrowComponent->SetSimulatePhysics(false);
	}
#endif // WITH_EDITORONLY_DATA
}

void ATPSEquipableItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATPSEquipableItem, IsOwned);
	DOREPLIFETIME(ATPSEquipableItem, IsEquipped);
}

void ATPSEquipableItem::BeginPlay()
{
	Super::BeginPlay();
}

void ATPSEquipableItem::BeginDestroy()
{
	Super::BeginDestroy();

	UE_LOG(LogTemp, Log, TEXT("Destroying EquipableItemInstance[%s]..."), *Name);
}

//~ ============================================================= ~//
//  DEFAULT INTERFACE BEHAVIOR
//~ ============================================================= ~//

//- Equip --//

void ATPSEquipableItem::Pickup()
{
	IsOwned = true;

	//DisableWorldCollision();

	PerformPickup();
}
void ATPSEquipableItem::Drop()
{
	if (IsMounted())
	{
		UnMount();
	}
	MountPoint = nullptr;
	IsOwned = false;
	IsEquipped = false;

	//EnableWorldCollision();

	PerformDrop();
}

//- Equip --//

void ATPSEquipableItem::Equip()
{
	IsEquipped = true;
	OnEquip();
}
void ATPSEquipableItem::UnEquip()
{
	IsEquipped = false;
	OnUnEquip();
}

//- Use --//

void ATPSEquipableItem::StartUse()
{
	OnStartUse();
}

void ATPSEquipableItem::StopUse()
{
	OnStopUse();
}

//~ ============================================================= ~//
//  STATE MODIFIERS
//~ ============================================================= ~//

void ATPSEquipableItem::EnableWorldCollision()
{
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComponent->SetSimulatePhysics(true);
}

void ATPSEquipableItem::DisableWorldCollision()
{
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionComponent->SetSimulatePhysics(false);
}

