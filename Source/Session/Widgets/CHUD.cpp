#include "CHUD.h"
#include "Global.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

bool UCHUD::Initialize()
{
	bool bSuccess = Super::Initialize();
	CheckFalseResult(bSuccess, false);

	CheckNullResult(DaedImage, false);
	DaedImage->SetVisibility(ESlateVisibility::Hidden);

	return true;
}

void UCHUD::SetHealthText(FString InText)
{
	CheckNull(HealthText)
	HealthText->SetText(FText::FromString(InText));
}
void UCHUD::SetScoreText(FString InText)
{
	CheckNull(ScoreText)
	ScoreText->SetText(FText::FromString(InText));
}

void UCHUD::SetDeathText(FString InText)
{
	CheckNull(DeathText)
	DeathText->SetText(FText::FromString(InText));
}

void UCHUD::SetVisibleDeadImage(bool InVisible)
{
	InVisible ?
		DaedImage->SetVisibility(ESlateVisibility::Visible) :
		DaedImage->SetVisibility(ESlateVisibility::Hidden);
}
