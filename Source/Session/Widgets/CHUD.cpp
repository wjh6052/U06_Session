#include "CHUD.h"
#include "Global.h"
#include "Components/TextBlock.h"

void UCHUD::SetHealthText(FString InText)
{
	HealthText->SetText(FText::FromString(InText));
}
void UCHUD::SetScoreText(FString InText)
{
	ScoreText->SetText(FText::FromString(InText));
}

void UCHUD::SetDeathText(FString InText)
{
	DeathText->SetText(FText::FromString(InText));
}
