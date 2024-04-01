#include "CBullet.h"
#include "Global.h"

#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Materials/MaterialInstanceConstant.h"


ACBullet::ACBullet()
{
	CHelpers::CreateSceneComponent(this, &Mesh, "Mesh");
	CHelpers::CreateActorComponent(this, &Projectile, "Projectile");

	UStaticMesh* meshAsset;
	CHelpers::GetAsset(&meshAsset, "StaticMesh'/Game/StaticMeshes/SM_Sphere.SM_Sphere'");
	Mesh->SetStaticMesh(meshAsset);

	UMaterialInstanceConstant* materialAsset;
	CHelpers::GetAsset(&materialAsset, "MaterialInstanceConstant'/Game/Materials/MAT_bullet_Inst.MAT_Bullet_Inst'");
	Mesh->SetMaterial(0, materialAsset);
	Mesh->SetRelativeScale3D(FVector(1.0f, 0.025f, 0.025f));
	Mesh->SetCollisionProfileName("Bullet");

	Projectile->InitialSpeed = 2e+4f;
	Projectile->MaxSpeed = 2e+4f;
	Projectile->ProjectileGravityScale = 0.0f;
}

void ACBullet::BeginPlay()
{
	Super::BeginPlay();
	
	OnActorBeginOverlap.AddDynamic(this, &ACBullet::BeginOvlap);
	SetLifeSpan(3.0f);
}

void ACBullet::BeginOvlap(AActor* OverlappedActor, AActor* OtherActor)
{
	Destroy(true);
}


