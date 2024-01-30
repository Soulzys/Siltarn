#pragma once

#include "SlateBasics.h"

DECLARE_LOG_CATEGORY_EXTERN(LogClass_SInventoryBagWidget, Log, All);

class AGameplayHUD;

class SILTARN_API SInventoryBagWidget : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SInventoryBagWidget) {}

	SLATE_ARGUMENT(TWeakObjectPtr<AGameplayHUD>, a_HUDOwner)

	SLATE_END_ARGS()

public:

	virtual bool SupportsKeyboardFocus() const override { return true; }

	void Construct(const FArguments& p_InArgs);

private:

	TWeakObjectPtr<AGameplayHUD> m_HUDOwner = nullptr;
};
