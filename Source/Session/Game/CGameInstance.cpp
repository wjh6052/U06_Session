#include "CGameInstance.h"

#include "OnlineSessionSettings.h"

#include "Widgets/CMainMenu.h"
#include "Widgets/CGameMenu.h"


const static FName SESSION_NAME = L"GameSesstion";
const static FName SESSION_KEY = L"SessionKey";



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
		UE_LOG(LogTemp, Error, TEXT("OSS Name : %s"), *oss->GetSubsystemName().ToString());

		SessionInterface = oss->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UCGameInstance::OnCreateSessionCompleted);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UCGameInstance::OnDestroySessionCompleted);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UCGameInstance::OnFindSessionCompleted);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UCGameInstance::OnJoinSessionCompleted);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("OSS Not Found"));
	}
	

	if (!!GEngine)
		GEngine->OnNetworkFailure().AddUObject(this, &UCGameInstance::OnNetworkFailure);
}


void UCGameInstance::Host(FString InSessionName)
{
	CustomSessionName = InSessionName;


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
	IOnlineSubsystem* oss = IOnlineSubsystem::Get();
	if (oss->GetSubsystemName().ToString() == "NULL")
	{
		sessionSettings.bIsLANMatch = true;

	}
	else
	{		
		sessionSettings.bIsLANMatch = false;
	}
	sessionSettings.NumPublicConnections = 5;
	sessionSettings.bShouldAdvertise = true;
	sessionSettings.bUsesPresence = true;

	sessionSettings.Set(SESSION_KEY, CustomSessionName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);


	SessionInterface->CreateSession(0, SESSION_NAME, sessionSettings);
}


void UCGameInstance::Join(uint32 InIndex)
{
	if (SessionInterface.IsValid() == false) return;
	if (SessionSearch.IsValid() == false) return;


	if (!!MenuWidget)
	{
		MenuWidget->SetInputGameMode();
		SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[InIndex]);
	}
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


void UCGameInstance::ShowJoinableSessionList()
{
	//Find Session
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		UE_LOG(LogTemp, Warning, L"Start Find Sessions");

		SessionSearch->bIsLanQuery = false;
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}
void UCGameInstance::StartSession()
{
	if (SessionInterface.IsValid())
		SessionInterface->StartSession(SESSION_NAME);

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

	world->ServerTravel("/Game/Maps/Lobby?listen");

}

void UCGameInstance::OnFindSessionCompleted(bool InSuccess)
{
	if (InSuccess == true && SessionSearch.IsValid() && MenuWidget != nullptr)
	{
		UE_LOG(LogTemp, Warning, L"Finished Find Session");


		TArray<FSessionData> sessionDatas;

		for (const auto& serchResult : SessionSearch->SearchResults)
		{
			UE_LOG(LogTemp, Error, L"Found Session ID : %s", *serchResult.GetSessionIdStr());
			UE_LOG(LogTemp, Error, L"Ping ID : %d", serchResult.PingInMs);

			FSessionData sessionData;
			sessionData.MaxPlayers = serchResult.Session.SessionSettings.NumPublicConnections;
			sessionData.CurrentPlayers = sessionData.MaxPlayers - serchResult.Session.NumOpenPublicConnections;
			sessionData.HostUserName = serchResult.Session.OwningUserName;
			
			FString sessionName;
			if (serchResult.Session.SessionSettings.Get(SESSION_KEY, sessionName))
			{
				UE_LOG(LogTemp, Warning, L"Value : %s", *sessionName);
				sessionData.Name = sessionName;
			}
			else
			{
				UE_LOG(LogTemp, Warning, L"Value : NULLValue");
			}

			sessionDatas.Add(sessionData);
		}

		MenuWidget->SetSessionList(sessionDatas);

	}

}

void UCGameInstance::OnJoinSessionCompleted(FName InSessionName, EOnJoinSessionCompleteResult::Type InResult)
{
	FString address;
	if (SessionInterface->GetResolvedConnectString(InSessionName, address) == false)
	{
		UE_LOG(LogTemp, Error, L"could not Resulve IP address");
		return;
	}


	UEngine* engine = GetEngine();
	if (engine == nullptr) return;
	engine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(L"Join to %s", *address), true, FVector2D(2));

	APlayerController* controller = GetFirstLocalPlayerController();
	if (controller == nullptr) return;

	controller->ClientTravel(address, ETravelType::TRAVEL_Absolute);

}

void UCGameInstance::OnNetworkFailure(UWorld* InWorld, UNetDriver* InNetDriver, ENetworkFailure::Type InFailure, const FString& InErrorMessage)
{
	UEngine* engine = GetEngine();
	if (engine == nullptr) return;
	engine->AddOnScreenDebugMessage(-1, 2, FColor::Red, InErrorMessage);

	TravelToMainMenu();

}


void UCGameInstance::OnDestroySessionCompleted(FName InSessionName, bool InSuccess)
{
	UE_LOG(LogTemp, Error, L"DESTROIED");

	if (SessionInterface.IsValid() == false) return;


	CreateSession();
}