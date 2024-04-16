#pragma once

#include "Siltarn/Public/Slate/Widgets/SInventoryWidget.h"
#include "SlateBasics.h"

DECLARE_LOG_CATEGORY_EXTERN(LogClass_SPlayerInventoryWidget, Log, All);

class UPickupEntity;

class SILTARN_API SPlayerInventoryWidget : public SInventoryWidget
{
public:

	SPlayerInventoryWidget();
	~SPlayerInventoryWidget();

	bool AddItemToInventoryNew(UPickupEntity* p_ItemEntity);
	bool RemoveItemCanvasSlot(int64 p_InventoryItemId);
	bool RemoveItemsCanvasSlotNew();

private:

	// Wouldn't the memmove issue come from here ? That this is in the child class rather than in the mother class ? 
	// After checking... it doesn't come from here
	TMap<int64, FInventoryItem*> m_InventoryItemsMapNew;
};
