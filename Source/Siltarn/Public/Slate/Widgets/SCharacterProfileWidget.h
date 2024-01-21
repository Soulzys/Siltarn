#pragma once

#include "SlateBasics.h"

DECLARE_LOG_CATEGORY_EXTERN(LogClass_SCharacterProfileWidget, Log, All);

class SILTARN_API SCharacterProfileWidget : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SCharacterProfileWidget) {}

	SLATE_END_ARGS()

public:

	virtual bool SupportsKeyboardFocus() const override { return true; }

	void Construct(const FArguments& p_InArgs);
};
