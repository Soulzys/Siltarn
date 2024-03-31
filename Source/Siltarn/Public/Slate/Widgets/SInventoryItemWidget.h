#pragma once

#include "Siltarn/Public/Slate/Styles/SiltarnGeneralStyleContainer.h"
#include "Siltarn/Public/Slate/Styles/SiltarnStyleController.h"
#include "Siltarn/Public/Slate/Widgets/SInventoryWidget.h" // It would be ideal if we coud get it in the .cpp
#include "Siltarn/Public/Inventory/InventoryEnumsLib.h"
#include "Input/DragAndDrop.h"
#include "SlateBasics.h"

#define DECORATOR_SIZE_PRECISION 10000
#define RESOLUTION_X 1920
#define RESOLUTION_Y 1080

DECLARE_LOG_CATEGORY_EXTERN(LogClass_SInventoryItemWidget   , Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogClass_FInventoryItemDragDrop , Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogClass_FItemTooltip           , Log, All);

class UMaterialInstance;
class SInventoryWidget;
class FItemTooltip;
class UPickupEntity;
class FInventoryItem;

UENUM()
enum class EInventoryItemWidgetState : uint8
{
	DEFAULT            = 0, 
	DRAGGED            = 1,
	SET_FOR_GROUP_DROP = 2
};

// Luciole
// To fix or/and to investigate : 
// - the padding the items have that is made obvious when hovered
// - The removal of FTile data (a_Tile) // WAIT ! Actually, we use it to Find() the item in the TMap, so we probably shouldn't remove it
// - The removal of SInventoryWidget (a_ParentWidget)
// - The removal of FInventoryItem (a_Item)
// 
// The variables aboved mentioned are never used and therefore should be removed. 
// In SInventoryWidget::OnDrop() where FInventoryItemDragDrop operation is used, only the UPickupEntity (a_ItemData) and the dragged item ref are used. 

class SILTARN_API SInventoryItemWidget : public SCompoundWidget
{

public:

	SLATE_BEGIN_ARGS(SInventoryItemWidget) : _a_ItemData       (nullptr)         , 
											 _a_Tile           (nullptr)         , 
											 _a_ItemSize       (FVector2D(0.0f)) , 
											 _a_InventoryOwner (nullptr)         ,
											 _a_InventoryItemId(-1), 
											 _a_InventoryItemWidgetLocation(EInventoryItemWidgetLocation::UNKNOWN)
											 {}

	SLATE_ARGUMENT(UPickupEntity*   , a_ItemData        )
	SLATE_ARGUMENT(FTile*           , a_Tile            )
	SLATE_ARGUMENT(FVector2D        , a_ItemSize        )
	SLATE_ARGUMENT(SInventoryWidget*, a_InventoryOwner  )
	SLATE_ARGUMENT(int32            , a_InventoryItemId )
	SLATE_ARGUMENT(FInventoryItem*, a_InventoryItemClass) // new
	SLATE_ARGUMENT(EInventoryItemWidgetLocation, a_InventoryItemWidgetLocation)

	SLATE_END_ARGS()

public:

	SInventoryItemWidget();
	~SInventoryItemWidget();

	virtual void   OnMouseEnter         (const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void   OnMouseLeave         (const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonDown    (const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnDragDetected       (const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual bool   SupportsKeyboardFocus() const override { return true; }
	virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;

	void Construct(const FArguments& p_InArgs);
	void UPDATE_Tile(FTile& p_NewTile);

	void UPDATE_Widget(SInventoryItemWidget* p_ItemWidget);

	FORCEINLINE UPickupEntity* GET_ItemData() const { return m_ItemData; }

private:

	float GET_ScreenToViewportRatio() const;

private:

	FSlateBrush  m_ItemIcon;
	FLinearColor m_ItemBackgroundColor;
	FVector2D    m_ItemSize;

	TSharedPtr<SBorder> m_BackgroundBorder = nullptr;
	FTile*                   m_Tile        = nullptr;
	TSharedPtr<SImage>       m_IconImage   = nullptr;
	TSharedPtr<FItemTooltip> m_ItemTooltip = nullptr;
	UPickupEntity*           m_ItemData    = nullptr;
	SInventoryWidget* m_InventoryOwner = nullptr;
	int32 m_InventoryItemId;
	FInventoryItem* m_InventoryItemClass = nullptr;

	bool m_bIsInInventory;
	bool m_bIsSelectedForGroupDrop;

	EInventoryItemWidgetState    m_InventoryItemState    ;
	EInventoryItemWidgetLocation m_InventoryItemLocation ;

	
	const FSiltarnGeneralStyleContainerStruct m_GeneralStyle = FSiltarnStyleController::GET_SiltarnGeneralStyleContainerStruct();
};




/**********************************
	Drag & drop operation class
***********************************

* Handles the illusion of dragging the item's widget through its "decorator".
* Also responsible for passing data back to SInventoryWidget in OnDrop()
*/

class SILTARN_API FInventoryItemDragDrop : public FDragDropOperation
{

public:

	DRAG_DROP_OPERATOR_TYPE(FInventoryItemDragDrop, FDragDropOperation)

public:

	FInventoryItemDragDrop();
	~FInventoryItemDragDrop();

	virtual void                OnDragged(const class FDragDropEvent& DragDropEvent) override;
	virtual TSharedPtr<SWidget> GetDefaultDecorator() const                          override;

	static TSharedRef<FInventoryItemDragDrop> CREATE_SingleItemDragOperation
	(
		SInventoryItemWidget* p_DraggedItem           , 
		const float           p_ScreenToViewportRatio , 
		const FVector2D&      p_WidgetSize            , 
		UPickupEntity*        p_ItemEntity            ,
		FTile*                p_Tile, 
		FInventoryItem* p_InventoryItemClass,
		int32 p_InventoryItemId
	);

	static TSharedRef<FInventoryItemDragDrop> CREATE_MultipleItemsDragOperation
	(
		//TArray<UPickupEntity*>& p_ItemEntities          , // old
		//TArray<FInventoryItem*>& p_InventoryItems, // new
		const float             p_ScreenToViewportRatio , 
		const FVector2D&        p_WidgetSize            , 
		const FSlateBrush*      p_DecoratorIcon
	);

	FORCEINLINE const FVector2D         GET_WidgetMiddlePosition()   const { return m_DecoratorSize * 0.5f * m_ScreenToViewportRatio ; }
	FORCEINLINE UPickupEntity*          GET_ItemEntity()             const { return m_ItemEntity                                     ; }
	FORCEINLINE TArray<UPickupEntity*>* GET_ItemsArray()             const { return m_ItemEntities                                   ; }
	FORCEINLINE TArray<FInventoryItem*>* GET_InventoryItemsArray()             const { return m_InventoryItems; }
	FORCEINLINE FTile*                  GET_Tile()                   const { return m_Tile                                           ; }
	FORCEINLINE bool                    IS_SingleItemDragOperation() const { return m_bIsSingleItemDrag                              ; }
	FORCEINLINE int32 GET_InventoryItemId() const { return m_InventoryItemid; }
	FORCEINLINE FInventoryItem* GET_InventoryItemClass() const { return m_InventoryItemClass; }

private:

	FSlateBrush             m_IconBrush              ;
	float                   m_ScreenToViewportRatio  ;
	FVector2D               m_DecoratorSize          ;
	SInventoryItemWidget*   m_DraggedItem  = nullptr ;
	UPickupEntity*          m_ItemEntity   = nullptr ;
	FTile*                  m_Tile         = nullptr ;
	TArray<UPickupEntity*>* m_ItemEntities = nullptr ; // old
	bool                    m_bIsSingleItemDrag      ;
	TArray<FInventoryItem*>* m_InventoryItems = nullptr; // new
	int32 m_InventoryItemid; // new (TMap)
	FInventoryItem* m_InventoryItemClass = nullptr; // new new 

	static int32 m_InstanceCount;
};



/**********************************
	Item tooltip class
***********************************/

class UEquipableEntity;
class UNonEquipableEntity;

class SILTARN_API FItemTooltip : public TSharedFromThis<FItemTooltip>
{
public:

	FItemTooltip();
	~FItemTooltip();

	static TSharedRef<FItemTooltip> CREATE_Tooltip(UPickupEntity* p_Item);

	void DESTROY_Tooltip();

	TSharedPtr<SWindow> GET_TooltipWindow() const;
	void MOVE_Window(const FVector2D& p_Position);

private:

	void CREATE_TooltipWindow();
	TSharedPtr<SWidget> BUILD_TooltipWidget() const;
	TSharedPtr<SWidget> BUILD_Tooltip_Pistol() const;
	TSharedPtr<SWidget> BUILD_Tooltip_Ammunition() const;

private:

	TSharedPtr<SWindow> m_TooltipWindow = nullptr;
	UPickupEntity*      m_ItemEntity    = nullptr;

	const FSiltarnGeneralStyleContainerStruct m_GeneralStyle = FSiltarnStyleController::GET_SiltarnGeneralStyleContainerStruct();

	FSlateFontInfo m_FontInfo;


	static int32 m_InstanceCount;
};