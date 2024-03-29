#pragma once

#include "Siltarn/Public/Slate/Widgets/SInventoryWidget.h"
#include "SlateBasics.h"

DECLARE_LOG_CATEGORY_EXTERN(LogClass_SInGameBagInventory, Log, All);

class SILTARN_API SInGameBagInventory : public SInventoryWidget
{
public:

	~SInGameBagInventory();

	void Reconstruct(const SInventoryWidget::FArguments& p_Args);
	void LoadItemsWidgets(TArray<UPickupEntity*>& p_Items);

private:

	TMap<int64, FInventoryItem*> m_InventoryItemsMap;
};
