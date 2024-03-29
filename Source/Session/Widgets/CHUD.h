#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CHUD.generated.h"

UCLASS()
class SESSION_API UCHUD : public UUserWidget
{
	GENERATED_BODY()


public:
	void SetHealthText(FString InText);
	void SetScoreText(FString InText);
	void SetDeathText(FString InText);

private:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* HealthText;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ScoreText;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* DeathText;
	
};
