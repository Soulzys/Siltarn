#include "GameModes/DefaultGameMode.h"
#include "Siltarn/Public/HUDs/GameplayHUD.h"
#include "Siltarn/Public/Characters/SiltarnPlayerController.h"

ADefaultGameMode::ADefaultGameMode()
{
	HUDClass              = AGameplayHUD            ::StaticClass();
	PlayerControllerClass = ASiltarnPlayerController::StaticClass();

	UE_LOG(LogTemp, Error, TEXT("I'm a default game mode !"));
}

void ADefaultGameMode::BeginPlay()
{
	Super::BeginPlay();
}
