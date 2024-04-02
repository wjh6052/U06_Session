 #include "CLobbyGameMode.h"
#include "CGameInstance.h"



void ACLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	++NumberOfPlayers;

	UE_LOG(LogTemp, Warning, L"Current Players : %d", NumberOfPlayers);


	if (NumberOfPlayers >= PlayerMax)
	{
		UE_LOG(LogTemp, Warning, L"Reached Max Players");


		GetWorldTimerManager().SetTimer(GameStartHandle, this, &ACLobbyGameMode::StartGame, StartTime);
	}
}


void ACLobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	--NumberOfPlayers;

	UE_LOG(LogTemp, Warning, L"Current Players : %d", NumberOfPlayers);
}


void ACLobbyGameMode::StartGame()
{
	UCGameInstance* gameInstance = Cast<UCGameInstance>(GetGameInstance());

	if (gameInstance == nullptr) return;
		gameInstance->StartSession();



	UWorld* world = GetWorld();
	if (world == nullptr) return;

	//bUseSeamlessTravel = true;
	world->ServerTravel("/Game/Maps/Fps_Map?listen");
}



