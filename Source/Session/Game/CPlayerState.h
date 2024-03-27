#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CGameState.h"
#include "CPlayerState.generated.h"

UCLASS()
class SESSION_API ACPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ACPlayerState(const FObjectInitializer& ObjectInitializer);


public:
	UPROPERTY(Replicated)
		float Health;

	UPROPERTY(Replicated)
		float Death;

	UPROPERTY(Replicated)
		ETeamType Team;


};
