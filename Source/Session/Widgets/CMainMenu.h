#pragma once

#include "CoreMinimal.h"
#include "CMenu.h"
#include "CMainMenu.generated.h"

USTRUCT()
struct FSessionData
{
	GENERATED_BODY()

public:
	FString Name;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;
	FString HostUserName;
};

UCLASS()
class SESSION_API UCMainMenu : public UCMenu
{
	GENERATED_BODY()


public:
	UCMainMenu(const FObjectInitializer& ObjectInitializer);


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
		void OpenHostMenu();

	UFUNCTION()
		void JoinServer();

	UFUNCTION()
		void QuitGame();


public:
	void SetSessionList(TArray<FSessionData> InFSessionDatas);

	void SetSelectedSessionIndex(uint32 InIndex);


private:
	//MainManu
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
		class UWidget* HostManu;


	//JoinManu
	UPROPERTY(meta = (BindWidget))
		class UButton* JoinManu_Confirm_Button;

	UPROPERTY(meta = (BindWidget))
		class UButton* JoinManu_Back_Button;

	UPROPERTY(meta = (BindWidget))
		class UPanelWidget* SessionList;


	//HostManu
	UPROPERTY(meta = (BindWidget))
		class UButton* HostManu_Confirm_Button;

	UPROPERTY(meta = (BindWidget))
		class UButton* HostManu_Back_Button;

	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* SessionNameText;


private:
	TSubclassOf<class UUserWidget> SessionRowClass;
	TOptional<uint32> SelectedSessionIndex;

};
