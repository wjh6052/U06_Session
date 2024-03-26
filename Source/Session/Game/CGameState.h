#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CGameState.generated.h"


UENUM(BlueprintType)
enum class ETeamType : uint8
{
	RedTeam,
	BlueTeam
};


UCLASS()
class SESSION_API ACGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ETeamType TestTeam;

private:

};
