#include "CGameInstance.h"

UCGameInstance::UCGameInstance(const FObjectInitializer& ObjectInitializer)
{
	UE_LOG(LogTemp, Error, L"GameInstance Constructor")
}

void UCGameInstance::Init()
{
	UE_LOG(LogTemp, Error, L"GameInstance Init")
}

void UCGameInstance::Host()
{
	UEngine* engine = GetEngine();

	if (engine == nullptr) return;

	engine->AddOnScreenDebugMessage(-1, 2, FColor::Green, L"Host", true, FVector2D(5));

	UWorld* world = GetWorld();
	if (world == nullptr) return;

	world->ServerTravel("/Game/ThirdPersonCPP/Maps/Play?listen");
}

void UCGameInstance::Join(const FString InAddress)
{
	UEngine* engine = GetEngine();

	if (engine == nullptr) return;

	engine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(L"Join to %s", *InAddress), true, FVector2D(5));

	
	APlayerController* controller = GetFirstLocalPlayerController();

	if (controller == nullptr) return;

	controller->ClientTravel(InAddress, ETravelType::TRAVEL_Absolute);


}
