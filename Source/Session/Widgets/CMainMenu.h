#pragma once

#include "CoreMinimal.h"
#include "CMenu.h"
#include "CMainMenu.generated.h"


UCLASS()
class SESSION_API UCMainMenu : public UCMenu
{
	GENERATED_BODY()


protected:
	virtual bool Initialize() override;


private:
	UFUNCTION()
		void HostServer();

	UFUNCTION()
		void OpenJoinMenu();

	UFUNCTION()
		void OpenMainMenu();

	UFUNCTION()
		void JoinServer();

	UFUNCTION()
		void QuitGame();
		

private:
	UPROPERTY(meta = (BindWidget))
		class UButton* Host_Button;

	UPROPERTY(meta = (BindWidget))
		class UButton* Join_Button;

	UPROPERTY(meta = (BindWidget))
		class UButton* Quit_Button;

	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* ManuSwitcher;

	UPROPERTY(meta = (BindWidget))
		class UWidget* MainManu;

	UPROPERTY(meta = (BindWidget))
		class UWidget* JoinManu;


	UPROPERTY(meta = (BindWidget))
		class UButton* JoinManu_Confirm_Button;

	UPROPERTY(meta = (BindWidget))
		class UButton* JoinManu_Back_Button;

	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* IPAddressField;

};
