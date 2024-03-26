#include "FPSGameMode.h"
#include "Characters/FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"

#include "FPSHUD.h"
#include "CGameState.h"


AFPSGameMode::AFPSGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(L"/Game/Player/BP_CPlayer");
	DefaultPawnClass = PlayerPawnClassFinder.Class;


	HUDClass = AFPSHUD::StaticClass();
	GameStateClass = ACGameState::StaticClass();

}
