#include "CGameMenu.h"
#include "Components/Button.h"


bool UCGameMenu::Initialize()
{
	bool bSuccess = Super::Initialize();
	if (bSuccess == false) return false;


	if (Back_Button == nullptr) return false;
	Back_Button->OnClicked.AddDynamic(this, &UCGameMenu::BackButtonPressed);

	if (Quit_Button == nullptr) return false;
	Quit_Button->OnClicked.AddDynamic(this, &UCGameMenu::QuitButtonPressed);

	return true;

}

void UCGameMenu::BackButtonPressed()
{
	SetInputGameMode();
}

void UCGameMenu::QuitButtonPressed()
{
	if (OwingGameInstance == nullptr) return;

	SetInputGameMode();
	OwingGameInstance->TravelToMainMenu();
}
