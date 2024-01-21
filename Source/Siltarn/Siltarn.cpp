#include "Siltarn.h"
#include "Modules/ModuleManager.h"
#include "Public/Slate/Styles/SiltarnStyleController.h"

class FSiltarnGameModule : public FDefaultGameModuleImpl
{
	virtual void StartupModule() override
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(FSiltarnStyleController::GET_StyleSetName());
		FSiltarnStyleController::INITIALIZE();
	}

	virtual void ShutdownModule() override
	{
		FSiltarnStyleController::SHUTDOWN();
	}
};

IMPLEMENT_PRIMARY_GAME_MODULE( FSiltarnGameModule, Siltarn, "Siltarn" );
