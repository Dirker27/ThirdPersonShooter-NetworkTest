#include "Inventory/TPSEquipableItem.h"

#include "Components/BoxComponent.h"

ATPSEquipableItem::ATPSEquipableItem()
{
	//USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	//SetRootComponent(root);

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionComponent->SetSimulatePhysics(false);
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

//~ ============================================================= ~//
//  DEFAULT INTERFACE BEHAVIOR
//~ ============================================================= ~//

//- Equip --//

void ATPSEquipableItem::Pickup()
{
	IsOwned = true;

	//CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//CollisionComponent->SetSimulatePhysics(false);

	OnPickup();
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

	//CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//CollisionComponent->SetSimulatePhysics(true);

	OnDrop();
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
