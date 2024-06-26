#pragma once

#include "Siltarn/Public/Slate/Styles/SiltarnGeneralStyleContainer.h"
#include "Siltarn/Public/Slate/Styles/SiltarnStyleController.h"
#include "SlateBasics.h"

DECLARE_LOG_CATEGORY_EXTERN(LogClass_SProfileMenu, Log, All);

class AGameplayHUD            ;
class SInventoryWidget        ;
class SCharacterProfileWidget ;
class SItemWidget    ;
class UPickupEntity           ;
class SCharacterProfileWidget ;
class SPlayerInventoryWidget;
class SExternalInventoryWidget;
class UInventoryManager;
class AItemBagActor;

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
	virtual FReply OnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	void Construct(const FArguments& p_InArgs);

	bool DOES_InventoryHasRoomForItem(const FIntPoint& p_ItemSize);
	bool MOVE_ItemToCharacterProfileMenu(SItemWidget* p_ItemWidget);
	FORCEINLINE TSharedPtr<SPlayerInventoryWidget> GET_InventoryWidget() const { return m_PlayerInventoryWidget; }

	void SET_InventoryManager(UInventoryManager* p_InventoryManager);

	// Dealing with external inventory (Bag, Chestbox...)
	void OpenExternalInventoryWidget(AItemBagActor* p_BagActor, const FIntPoint& p_InventorySize, int32 p_TileSize, TArray<UPickupEntity*>& p_Items);
	void CloseExternalInventoryWidget();

private:

	int32 m_PlayerInventoryNumberOfColumns;
	int32 m_PlayerInventoryNumberOfRows;
	int32 m_ExternalInventoryNumberOfColumns;
	int32 m_ExternalInventoryNumberOfRows;
	int32 m_TileSize;

	//

	TWeakObjectPtr<AGameplayHUD> m_HUDOwner = nullptr;

	TSharedPtr<SPlayerInventoryWidget> m_PlayerInventoryWidget = nullptr;
	TSharedPtr<SExternalInventoryWidget> m_ExternalInventoryWidget = nullptr;

	TSharedPtr<SCharacterProfileWidget> m_CharacterProfileWidget = nullptr;


	UInventoryManager* m_InventoryManager = nullptr;

	const FSiltarnGeneralStyleContainerStruct m_GeneralStyle = FSiltarnStyleController::GET_SiltarnGeneralStyleContainerStruct();
};
