#include "CMovingPlatformTrigger.h"
#include "Components/BoxComponent.h"

#include "CMovingPlatform.h"

ACMovingPlatformTrigger::ACMovingPlatformTrigger()
{
	Box = CreateDefaultSubobject<UBoxComponent>(FName("Box"));

	if (!!Box)
		RootComponent = Box;

	Box->bHiddenInGame = false;

}

void ACMovingPlatformTrigger::BeginPlay()
{
	Super::BeginPlay();
	

	OnActorBeginOverlap.AddDynamic(this, &ACMovingPlatformTrigger::OnBginOverlap);
	OnActorEndOverlap.AddDynamic(this, &ACMovingPlatformTrigger::OnEndOverlap);

	
}

void ACMovingPlatformTrigger::OnBginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	for (const auto& Platform : Platforms)
		Platform->AddActiveTriggers();
}

void ACMovingPlatformTrigger::OnEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	for (const auto& Platform : Platforms)
		Platform->RemoveActiveTriggers();

}

