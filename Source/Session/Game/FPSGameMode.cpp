#include "FPSGameMode.h"
#include "Global.h"

#include "EngineUtils.h"
#include "Engine/TargetPoint.h"
#include "UObject/ConstructorHelpers.h"

#include "FPSHUD.h"
#include "CGameState.h"
#include "CPlayerState.h"
#include "Characters/FPSCharacter.h"
#include "Actors/CSpawnPoint.h"



AFPSGameMode::AFPSGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(L"/Game/Player/BP_CPlayer");
	DefaultPawnClass = PlayerPawnClassFinder.Class;


	HUDClass = AFPSHUD::StaticClass();
	GameStateClass = ACGameState::StaticClass();
	PlayerStateClass = ACPlayerState::StaticClass();

}


void AFPSGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);


	AFPSCharacter* playerPawn = Cast<AFPSCharacter>(NewPlayer->GetPawn());
	ACPlayerState* playerState = Cast<ACPlayerState>(NewPlayer->PlayerState);


	if (!!playerPawn && !!playerState)
	{
		//Set PlayerState to Character from Controller
		playerPawn->SetPlayerState(playerState);
		

		//Team Contact
		if (RedTeamCharacters.Num() > BlueTeamCharacters.Num())
		{
			playerState->Team = ETeamType::BlueTeam;
			BlueTeamCharacters.Add(playerPawn);
		}
		else
		{
			playerState->Team = ETeamType::RedTeam;
			RedTeamCharacters.Add(playerPawn);
		}

		//Team Information to Character
		playerPawn->CurrentTeam = playerState->Team;
		playerPawn->SetTeamColor(playerState->Team);

		MoveToSpawnPoint(playerPawn);
	}
}


void AFPSGameMode::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<ACSpawnPoint> iter(GetWorld()); iter; ++iter) //UGameStatis::GetAllActors
	{
		if (iter->GetTeam() == ETeamType::RedTeam)
			RedTeamSpawnPoints.Add(*iter);
		else
			BlueTeamSpawnPoints.Add(*iter);
	}
}


void AFPSGameMode::MoveToSpawnPoint(AFPSCharacter* InPlayer)
{
	TArray<AActor*> centerPoint;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), centerPoint);
	

	TArray<ACSpawnPoint*>* targetPoints;


	if (InPlayer->CurrentTeam == ETeamType::RedTeam)
		targetPoints = &RedTeamSpawnPoints;
	else
		targetPoints = &BlueTeamSpawnPoints;

	for (const auto& point : *targetPoints)
	{
		//SpawnPoint is no blocked
		if (point->IsBlocked() == false)
		{
			InPlayer->SetActorLocation(point->GetActorLocation());
			FRotator rotation = FRotator::ZeroRotator;

			if (!!centerPoint[0])
			{
				rotation = UKismetMathLibrary::FindLookAtRotation(InPlayer->GetActorLocation(), centerPoint[0]->GetActorLocation());
				InPlayer->SetActorRotation(rotation);
			}


			point->UpdateOverlaps();

			return;
		}
	}


}
