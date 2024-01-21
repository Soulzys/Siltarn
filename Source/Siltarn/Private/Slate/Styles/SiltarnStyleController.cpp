#include "Slate/Styles/SiltarnStyleController.h"
#include "Slate/SlateGameResources.h"

TSharedPtr<FSlateStyleSet> FSiltarnStyleController::m_ClassInstance = nullptr;

void FSiltarnStyleController::INITIALIZE()
{
	if (m_ClassInstance.IsValid() == false)
	{
		m_ClassInstance = CREATE();
		FSlateStyleRegistry::RegisterSlateStyle(*m_ClassInstance);
	}
}

void FSiltarnStyleController::SHUTDOWN()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*m_ClassInstance);
	ensure(m_ClassInstance.IsUnique());
	m_ClassInstance.Reset();
}

FName FSiltarnStyleController::GET_StyleSetName()
{
	static FName _StyleSetName(TEXT("SiltarnStyleController"));
	return _StyleSetName;
}

const ISlateStyle& FSiltarnStyleController::GET_StyleSet()
{
	return *m_ClassInstance;
}

TSharedRef<FSlateStyleSet> FSiltarnStyleController::CREATE()
{
	TSharedRef<FSlateStyleSet> _StyleRef = FSlateGameResources::New
	(
		FSiltarnStyleController::GET_StyleSetName(), 
		"/Game/UI/Slate/Styles", 
		"/Game/UI/Slate/Styles"
	);

	return _StyleRef;
}