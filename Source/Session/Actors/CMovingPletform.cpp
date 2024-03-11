#include "CMovingPletform.h"
#include "Materials/MaterialInstanceConstant.h"


ACMovingPletform::ACMovingPletform()
{
	PrimaryActorTick.bCanEverTick = true;

	SetMobility(EComponentMobility::Movable);

	ConstructorHelpers::FObjectFinder<UStaticMesh> meshAsset(L"/Game/Geometry/Meshes/1M_Cube");

	if (meshAsset.Succeeded())
		GetStaticMeshComponent()->SetStaticMesh(meshAsset.Object);

	ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> materialAsset(L"/Game/Geometry/Meshes/Materials/MAP_MovingPletform_Rad");

	if (materialAsset.Succeeded())
		GetStaticMeshComponent()->SetMaterial(0, materialAsset.Object);

	GetStaticMeshComponent()->SetWorldScale3D(FVector(1.5f, 1.5f, 0.25f));
}


void ACMovingPletform::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	}
	

}

void ACMovingPletform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		FVector currentLocation = GetActorLocation();
		FVector targetWS = GetTransform().TransformPosition(Target);

		FVector direction = (targetWS - currentLocation).GetSafeNormal();

		currentLocation += direction * Speed * DeltaTime;
		SetActorLocation(currentLocation);
	}

}

