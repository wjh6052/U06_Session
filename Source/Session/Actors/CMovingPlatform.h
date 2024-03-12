#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "CMovingPlatform.generated.h"

UCLASS()
class SESSION_API ACMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	ACMovingPlatform();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	void AddActiveTriggers();
	void RemoveActiveTriggers();

private:
	UPROPERTY(EditAnywhere, Category = "Platform")
		float Speed = 20.f;

	UPROPERTY(EditAnywhere, Category = "Platform")
		int32 ActiveTriggers = 0;

	UPROPERTY(EditAnywhere, Category = "Platform", meta = (MakeEditWidget))
		FVector Target;




private:
	FVector OriginWS;
	FVector TargetWS;

};
