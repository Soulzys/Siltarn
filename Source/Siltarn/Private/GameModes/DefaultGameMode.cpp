#include "GameModes/DefaultGameMode.h"
#include "Siltarn/Public/HUDs/GameplayHUD.h"
#include "Siltarn/Public/Characters/SiltarnPlayerController.h"

ADefaultGameMode::ADefaultGameMode()
{
	HUDClass              = AGameplayHUD            ::StaticClass();
	PlayerControllerClass = ASiltarnPlayerController::StaticClass();
}
