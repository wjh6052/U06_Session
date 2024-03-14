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


	if (JoinManu_Confirm_Button == nullptr) return false;
	JoinManu_Confirm_Button->OnClicked.AddDynamic(this, &UCMainMenu::JoinServer);

	if (JoinManu_Back_Button == nullptr) return false;
	JoinManu_Back_Button->OnClicked.AddDynamic(this, &UCMainMenu::OpenMainMenu);

	return true;
}

void UCMainMenu::SetOwingGameInstance(IIMenuInterface* InOwingGameInstance)
{
	OwingGameInstance = InOwingGameInstance;
}

void UCMainMenu::SetInputGameMode()
{
	RemoveFromParent();

	bIsFocusable = false;

	FInputModeGameOnly inputMode;

	UWorld* world = GetWorld();
	if (world == nullptr) return;

	APlayerController* controller = world->GetFirstPlayerController();
	if (controller == nullptr) return;

	controller->SetInputMode(inputMode);
	controller->bShowMouseCursor = false;
}

void UCMainMenu::SetInputUIMode()
{
	AddToViewport();

	bIsFocusable = true;

	FInputModeUIOnly inputMode;
	inputMode.SetWidgetToFocus(TakeWidget());
	inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	UWorld* world = GetWorld();
	if (world == nullptr) return;

	APlayerController* controller = world->GetFirstPlayerController();
	if (controller == nullptr) return;

	controller->SetInputMode(inputMode);
	controller->bShowMouseCursor = true;
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

}
