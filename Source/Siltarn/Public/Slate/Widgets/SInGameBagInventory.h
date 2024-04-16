#pragma once

#include "Siltarn/Public/Slate/Widgets/SInventoryWidget.h"
#include "SlateBasics.h"

DECLARE_LOG_CATEGORY_EXTERN(LogClass_SInGameBagInventory, Log, All);

class AItemBagActor;

class SILTARN_API SInGameBagInventory : public SInventoryWidget
{
public:

	~SInGameBagInventory();

	void Construct(const FArguments p_InArgs);
	void LoadItemsWidgetsNew(TArray<UPickupEntity*>& p_Items);

	void MoveItemToPlayerInventory(TSharedPtr<SItemWidget> p_ItemWidget);
	void RemoveItemCanvasSlot(int64 p_ItemKey);

	void SET_BagActor(AItemBagActor* p_BagActor);

private:

	TMap<int64, FInventoryItem*> m_InventoryItemsMap;
	AItemBagActor* m_BagActor = nullptr;

};
