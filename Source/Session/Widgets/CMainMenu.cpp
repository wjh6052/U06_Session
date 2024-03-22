#include "CMainMenu.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"

#include "CSessionRow.h"


UCMainMenu::UCMainMenu(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> sessionRowClassAsset(L"/Game/Widgets/WB_SessionRow");
	if (sessionRowClassAsset.Succeeded())
		SessionRowClass = sessionRowClassAsset.Class;
}


bool UCMainMenu::Initialize()
{
	bool bSuccess = Super::Initialize();
	if (bSuccess == false) return false;


	if (Host_Button == nullptr) return false;
	Host_Button->OnClicked.AddDynamic(this, &UCMainMenu::OpenHostMenu);

	if (Join_Button == nullptr) return false;
	Join_Button->OnClicked.AddDynamic(this, &UCMainMenu::OpenJoinMenu);

	if (Quit_Button == nullptr) return false;
	Quit_Button->OnClicked.AddDynamic(this, &UCMainMenu::QuitGame);


	if (JoinManu_Confirm_Button == nullptr) return false;
	JoinManu_Confirm_Button->OnClicked.AddDynamic(this, &UCMainMenu::JoinServer);

	if (JoinManu_Back_Button == nullptr) return false;
	JoinManu_Back_Button->OnClicked.AddDynamic(this, &UCMainMenu::OpenMainMenu);


	if (HostManu_Confirm_Button == nullptr) return false;
	HostManu_Confirm_Button->OnClicked.AddDynamic(this, &UCMainMenu::HostServer);

	if (HostManu_Back_Button == nullptr) return false;
	HostManu_Back_Button->OnClicked.AddDynamic(this, &UCMainMenu::OpenMainMenu);


	return true;
}


void UCMainMenu::HostServer()
{
	if (!!OwingGameInstance && !!SessionNameText)
	{
		FString customSessionName = SessionNameText->Text.ToString();
		OwingGameInstance->Host(customSessionName);

	}
}


void UCMainMenu::OpenJoinMenu()
{
	if (ManuSwitcher == nullptr) return;
	if (MainManu == nullptr) return;

	ManuSwitcher->SetActiveWidget(JoinManu);

	if (!!OwingGameInstance)
		OwingGameInstance->ShowJoinableSessionList();
}


void UCMainMenu::OpenMainMenu()
{
	if (ManuSwitcher == nullptr) return;
	if (MainManu == nullptr) return;

	ManuSwitcher->SetActiveWidget(MainManu);
}

void UCMainMenu::OpenHostMenu()
{
	if (ManuSwitcher == nullptr) return;
	if (HostManu == nullptr) return;

	ManuSwitcher->SetActiveWidget(HostManu);
}


void UCMainMenu::JoinServer()
{
	if (OwingGameInstance == nullptr) return;


	if (SelectedSessionIndex.IsSet())
	{
		UE_LOG(LogTemp, Warning, L"SelectedSessionIndex : %d", SelectedSessionIndex.GetValue());
		OwingGameInstance->Join(SelectedSessionIndex.GetValue());
	}
	else
	{
		UE_LOG(LogTemp, Warning, L"SelectedSessionIndex : Not Set");

	}

}


void UCMainMenu::QuitGame()
{
	UWorld* world = GetWorld();
	if (world == nullptr) return;


	APlayerController* controller = world->GetFirstPlayerController();
	if (controller == nullptr) return;

	controller->ConsoleCommand("Quit");

}


void UCMainMenu::SetSessionList(TArray<FSessionData> InFSessionDatas)
{
	UWorld* world = GetWorld();
	if (world == nullptr) return;

	SessionList->ClearChildren();

	uint32 i = 0;
	for (const auto& sessionData : InFSessionDatas)
	{
		UCSessionRow* row = CreateWidget<UCSessionRow>(world, SessionRowClass);
		if (row == nullptr) return;

		row->SessionName->SetText(FText::FromString(sessionData.Name));
		row->HostUserName->SetText(FText::FromString(sessionData.HostUserName));

		FString fractionSrt = FString::Printf(L"%d / %d", sessionData.CurrentPlayers, sessionData.MaxPlayers);
		row->ConnectionFraction->SetText(FText::FromString(fractionSrt));


		row->PostCreated(this, i++);


		if (SessionList == nullptr) return;

		SessionList->AddChild(row);
	}
	
}

void UCMainMenu::SetSelectedSessionIndex(uint32 InIndex)
{
	SelectedSessionIndex = InIndex;

	for (int32 i = 0 ; i < SessionList->GetChildrenCount(); i++)
	{
		UCSessionRow* row = Cast<UCSessionRow>(SessionList->GetChildAt(i));
		if (!!row)
		{
			row->SetClicked((SelectedSessionIndex.IsSet()) && (i == SelectedSessionIndex.GetValue()));
		}
	}
}
