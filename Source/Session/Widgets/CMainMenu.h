#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IMenuInterface.h"
#include "CMainMenu.generated.h"


UCLASS()
class SESSION_API UCMainMenu : public UUserWidget
{
	GENERATED_BODY()


protected:
	virtual bool Initialize() override;


public:
	void SetOwingGameInstance(IIMenuInterface* InOwningGameInstance);


public:
	void SetInputGameMode();
	void SetInputUIMode();


private:
	UFUNCTION()
		void HostServer();

	UFUNCTION()
		void OpenJoinMenu();

	UFUNCTION()
		void OpenMainMenu();

	UFUNCTION()
		void JoinServer();


private:
	UPROPERTY(meta = (BindWidget))
		class UButton* Host_Button;

	UPROPERTY(meta = (BindWidget))
		class UButton* Join_Button;


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


private:
	IIMenuInterface* OwingGameInstance;

};
