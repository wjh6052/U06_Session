#include "FPSHUD.h"
#include "Global.h"

#include "Engine/Canvas.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Texture2D.h"

#include "Characters/FPSCharacter.h"
#include "CPlayerState.h"
#include "Widgets/CHUD.h"


AFPSHUD::AFPSHUD()
{
	// Set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/FirstPerson/Textures/FirstPersonCrosshair"));
	CrosshairTex = CrosshairTexObj.Object;

	CHelpers::GetClass<UCHUD>(&HUDWidgetClass, "/Game/Widgets/WB_HUD");
}

void AFPSHUD::BeginPlay()
{
	Super::BeginPlay();

	AFPSCharacter* playerPawn = Cast<AFPSCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	CheckNull(playerPawn);
	CheckNull(HUDWidgetClass);


	HUDWidget = CreateWidget<UCHUD>(playerPawn->GetController<APlayerController>(), HUDWidgetClass);
	HUDWidget->AddToViewport();
}

void AFPSHUD::DrawHUD()
{
	Super::DrawHUD();

	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	const FVector2D CrosshairDrawPosition( (Center.X - (CrosshairTex->GetSurfaceWidth() * 0.5)),
										   (Center.Y - (CrosshairTex->GetSurfaceHeight() * 0.5f)) );

	FCanvasTileItem TileItem( CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem( TileItem );


	AFPSCharacter* playerPawn = Cast<AFPSCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	CheckNull(playerPawn);

	ACPlayerState* playerState = playerPawn->GetSelfPlayerState();
	CheckNull(playerState);

	FString str;
	str = "Spawn Rotation : " + playerState->SpawnRotation.ToString();
	DrawText(str, FLinearColor::Black, 50, 50);

	str = "Pawn Rotation : " + playerPawn->GetActorRotation().ToString();
	DrawText(str, FLinearColor::Black, 50, 50 + 15);

	str = "Self Player State : " + (playerState ? playerState->GetName() : "Player State Is NULL!!");
	DrawText(str, FLinearColor::Black, 50, 50 + 30);

	str = "Pawn Location : " + playerPawn->GetActorLocation().ToString();
	DrawText(str, FLinearColor::Black, 50, 50 + 45);


	CheckNull(HUDWidget);
	float health = playerState->Health;
	float score = playerState->Score;
	float death = playerState->Death;

	HUDWidget->SetHealthText(FString::FromInt((int32)health));
	HUDWidget->SetScoreText(FString::FromInt((int32)score));
	HUDWidget->SetDeathText(FString::FromInt((int32)death));

	if (health <= 0)
		HUDWidget->SetVisibleDeadImage(true);
	else
		HUDWidget->SetVisibleDeadImage(false);
}
