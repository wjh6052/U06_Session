#include "CMainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"


bool UCMainMenu::Initialize()
{
	bool bSuccess = Super::Initialize();
	if (bSuccess == false) return false;


	if (Host_Button == nullptr) return false;
	Host_Button->OnClicked.AddDynamic(this, &UCMainMenu::HostServer);

	if (Join_Button == nullptr) return false;
	Join_Button->OnClicked.AddDynamic(this, &UCMainMenu::OpenJoinMenu);

	if (Quit_Button == nullptr) return false;
	Quit_Button->OnClicked.AddDynamic(this, &UCMainMenu::QuitGame);

	if (JoinManu_Confirm_Button == nullptr) return false;
	JoinManu_Confirm_Button->OnClicked.AddDynamic(this, &UCMainMenu::JoinServer);

	if (JoinManu_Back_Button == nullptr) return false;
	JoinManu_Back_Button->OnClicked.AddDynamic(this, &UCMainMenu::OpenMainMenu);


	return true;
}


void UCMainMenu::HostServer()
{
	if (!!OwingGameInstance)
		OwingGameInstance->Host();
}

void UCMainMenu::OpenJoinMenu()
{
	if (ManuSwitcher == nullptr) return;
	if (MainManu == nullptr) return;

	ManuSwitcher->SetActiveWidget(JoinManu);
}

void UCMainMenu::OpenMainMenu()
{
	if (ManuSwitcher == nullptr) return;
	if (MainManu == nullptr) return;

	ManuSwitcher->SetActiveWidget(MainManu);
}

void UCMainMenu::JoinServer()
{
	if (IPAddressField == nullptr) return;
	if (OwingGameInstance == nullptr) return;


	const FString address = IPAddressField->GetText().ToString();
	OwingGameInstance->Join(address);
}

void UCMainMenu::QuitGame()
{
	UWorld* world = GetWorld();
	if (world == nullptr) return;


	APlayerController* controller = world->GetFirstPlayerController();
	if (controller == nullptr) return;

	controller->ConsoleCommand("Quit");

}
