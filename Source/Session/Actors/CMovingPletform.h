#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "CMovingPletform.generated.h"

UCLASS()
class SESSION_API ACMovingPletform : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	ACMovingPletform();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, Category = "Platform")
		float Speed = 20.0f;
	
	UPROPERTY(EditAnywhere, Category = "Platform", meta = (MakeEditWidget))
		FVector Target;


};
