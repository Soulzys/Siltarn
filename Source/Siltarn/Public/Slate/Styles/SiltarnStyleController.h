#pragma once

#include "SlateBasics.h"
#include "Siltarn/Public/Slate/Styles/SiltarnGeneralStyleContainer.h"

class FSlateStyleSet;

class SILTARN_API FSiltarnStyleController
{

public:

	static void INITIALIZE();
	static void SHUTDOWN();

	// *** GETTERS *** //
	//
	static FName GET_StyleSetName();
	static const ISlateStyle& GET_StyleSet();

	static const struct FSiltarnGeneralStyleContainerStruct& GET_SiltarnGeneralStyleContainerStruct()
	{
		return FSiltarnStyleController::GET_StyleSet().GetWidgetStyle<FSiltarnGeneralStyleContainerStruct>("SWS_SiltarnGeneralStyle");
	}

private:

	static TSharedRef<FSlateStyleSet> CREATE();

private:

	static TSharedPtr<FSlateStyleSet> m_ClassInstance;
};
