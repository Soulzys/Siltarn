#pragma once

#include "Siltarn/Public/Slate/Styles/SiltarnGeneralStyleContainer.h"
#include "Siltarn/Public/Slate/Styles/SiltarnStyleController.h"
#include "SlateBasics.h"

DECLARE_LOG_CATEGORY_EXTERN(LogClass_SProfileMenu, Log, All);

class AGameplayHUD            ;
class SInventoryWidget        ;
class SCharacterProfileWidget ;
class UPickupEntity           ;
class SCharacterProfileWidget ;

class SILTARN_API SProfileMenu : public SCompoundWidget
{

public:

	SLATE_BEGIN_ARGS(SProfileMenu) : 
			_a_NumberOfColumns(10) , 
			_a_NumberOfRows(10)    , 
			_a_TileSize(50.0f)
	{}

	SLATE_ATTRIBUTE(int32, a_NumberOfColumns)
	SLATE_ATTRIBUTE(int32, a_NumberOfRows)
	SLATE_ATTRIBUTE(int32, a_TileSize)

	SLATE_ARGUMENT(TWeakObjectPtr<AGameplayHUD>, a_HUDOwner)

	SLATE_END_ARGS()

public:

	virtual bool SupportsKeyboardFocus() const override { return true; }
	virtual FReply OnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent) override;

	void Construct(const FArguments& p_InArgs);

	bool DOES_InventoryHasRoomForItem(const FIntPoint& p_ItemSize);
	bool ADD_ItemToInventory(UPickupEntity* p_Item);

private:

	int32 m_NumberOfColumns;
	int32 m_NumberOfRows;
	int32 m_TileSize;

	//

	TWeakObjectPtr<AGameplayHUD> m_HUDOwner = nullptr;

	TSharedPtr<SInventoryWidget> m_InventoryWidget = nullptr;
	TSharedPtr<SCharacterProfileWidget> m_CharacterProfileWidget = nullptr;


	const FSiltarnGeneralStyleContainerStruct m_GeneralStyle = FSiltarnStyleController::GET_SiltarnGeneralStyleContainerStruct();
};
