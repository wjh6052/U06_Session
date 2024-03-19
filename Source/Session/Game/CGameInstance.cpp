#include "CGameInstance.h"

#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"

#include "Widgets/CMainMenu.h"
#include "Widgets/CGameMenu.h"


const static FName SESSION_NAME = L"GameSesstion";



UCGameInstance::UCGameInstance(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> menuWidgetClass(TEXT("/Game/Widgets/WB_MainMenu"));
	if (menuWidgetClass.Succeeded())
		MenuWidgetClass = menuWidgetClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> gameWidgetClass(TEXT("/Game/Widgets/WB_GameMenu"));
	if (gameWidgetClass.Succeeded())
		GameWidgetClass = gameWidgetClass.Class;
}


void UCGameInstance::Init()
{
	IOnlineSubsystem* oss = IOnlineSubsystem::Get();
	if (!!oss)
	{

		SessionInterface = oss->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UCGameInstance::OnCreateSessionCompleted);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UCGameInstance::OnDestroySessionCompleted);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UCGameInstance::OnFindSessionCompleted);
		
			//Find Session
			SessionSearch = MakeShareable(new FOnlineSessionSearch());
			if (SessionSearch.IsValid())
			{
				UE_LOG(LogTemp, Warning, L"Start Find Sessions");
				SessionSearch->bIsLanQuery = true;
				SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
			}

			//---
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("OSS Not Found"));
	}
}


void UCGameInstance::Host()
{
	if (SessionInterface.IsValid() == false) return;


	auto exsistingSession = SessionInterface->GetNamedSession(SESSION_NAME);

	if (!!exsistingSession)
	{
		SessionInterface->DestroySession(SESSION_NAME);
	}
	else
	{
		CreateSession();	
	}
	
}


void UCGameInstance::CreateSession()
{
	if (SessionInterface.IsValid() == false) return;


	FOnlineSessionSettings sessionSettings;
	sessionSettings.bIsLANMatch = true;
	sessionSettings.NumPublicConnections = 5;
	sessionSettings.bShouldAdvertise = true;

	SessionInterface->CreateSession(0, SESSION_NAME, sessionSettings);
}


void UCGameInstance::Join(const FString& InAddress)
{
	if (!!MenuWidget)
		MenuWidget->SetInputGameMode();

	UEngine* engine = GetEngine();
	if (engine == nullptr) return;
	engine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("Join to %s"), *InAddress), true, FVector2D(2));

	APlayerController* controller = GetFirstLocalPlayerController();
	if (controller == nullptr) return;

	controller->ClientTravel(InAddress, ETravelType::TRAVEL_Absolute);
}


void UCGameInstance::LoadMenu()
{
	if (MenuWidgetClass == nullptr) return;

	MenuWidget = CreateWidget<UCMainMenu>(this, MenuWidgetClass);
	if (MenuWidget == nullptr) return;
	MenuWidget->SetInputUIMode();

	MenuWidget->SetOwingGameInstance(this);
}


void UCGameInstance::LoadGameMenu()
{
	if (GameWidgetClass == nullptr) return;

	GameWidget = CreateWidget<UCGameMenu>(this, GameWidgetClass);
	if (GameWidget == nullptr) return;
	GameWidget->SetInputUIMode();

	GameWidget->SetOwingGameInstance(this);
}


void UCGameInstance::TravelToMainMenu()
{
	APlayerController* controller = GetFirstLocalPlayerController();
	if (controller == nullptr) return;

	controller->ClientTravel("/Game/Maps/MainMenu", ETravelType::TRAVEL_Absolute);
}


void UCGameInstance::OnCreateSessionCompleted(FName InSessionName, bool InSuccess)
{
	UE_LOG(LogTemp, Error, L"CREATED");

	if (InSuccess == false)
	{
		UE_LOG(LogTemp, Error, L"Could not create sesstion");
		return;
	}

	UE_LOG(LogTemp, Warning, L"SesstionName : %s", *InSessionName.ToString());


	if (!!MenuWidget)
		MenuWidget->SetInputGameMode();

	UEngine* engine = GetEngine();
	if (engine == nullptr) return;
	engine->AddOnScreenDebugMessage(-1, 2, FColor::Green, TEXT("Host"), true, FVector2D(2));

	UWorld* world = GetWorld();
	if (world == nullptr) return;

	world->ServerTravel("/Game/Maps/Play?listen");

}

void UCGameInstance::OnFindSessionCompleted(bool InSuccess)
{
	if (InSuccess == true && SessionSearch.IsValid())
	{
		UE_LOG(LogTemp, Warning, L"Finished Find Session");

		for (const auto& serchResult : SessionSearch->SearchResults)
		{
			UE_LOG(LogTemp, Warning, L"Found Session ID : %s", *serchResult.GetSessionIdStr());
			UE_LOG(LogTemp, Warning, L"Ping ID : %d", serchResult.PingInMs);
		}
	}

}


void UCGameInstance::OnDestroySessionCompleted(FName InSessionName, bool InSuccess)
{
	UE_LOG(LogTemp, Error, L"DESTROIED");

	if (SessionInterface.IsValid() == false) return;


	CreateSession();
}