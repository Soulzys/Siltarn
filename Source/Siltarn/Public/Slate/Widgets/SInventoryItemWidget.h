#pragma once

#include "Siltarn/Public/Slate/Styles/SiltarnGeneralStyleContainer.h"
#include "Siltarn/Public/Slate/Styles/SiltarnStyleController.h"
#include "Siltarn/Public/Slate/Widgets/SInventoryWidget.h" // It would be ideal if we coud get it in the .cpp
#include "Input/DragAndDrop.h"
#include "SlateBasics.h"

#define DECORATOR_SIZE_PRECISION 10000
#define RESOLUTION_X 1920
#define RESOLUTION_Y 1080

DECLARE_LOG_CATEGORY_EXTERN(LogClass_SInventoryItemWidget  , Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogClass_FInventoryItemDragDrop, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogClass_FItemTooltip          , Log, All);

class UMaterialInstance;
class SInventoryWidget;
class FItemTooltip;
class UPickupEntity;

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

	SLATE_BEGIN_ARGS(SInventoryItemWidget) : _a_ItemData    (nullptr)         , 
											 _a_Tile        (nullptr)         , 
											 _a_ItemSize    (FVector2D(0.0f)) , 
											 _a_ParentWidget(nullptr)
											 {}

	SLATE_ARGUMENT(UPickupEntity*   , a_ItemData     )
	SLATE_ARGUMENT(FTile*           , a_Tile         )
	SLATE_ARGUMENT(FVector2D        , a_ItemSize     )
	SLATE_ARGUMENT(SInventoryWidget*, a_ParentWidget )

	SLATE_END_ARGS()

public:

	SInventoryItemWidget();
	~SInventoryItemWidget();

	virtual void   OnMouseEnter         (const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void   OnMouseLeave         (const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonDown    (const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnDragDetected       (const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual bool   SupportsKeyboardFocus() const override { return true; }

	void Construct(const FArguments& p_InArgs);
	void UPDATE_Tile(FTile& p_NewTile);

	void UPDATE_Widget(SInventoryItemWidget* p_ItemWidget);

	// Custom FReply
	FReply DeleteItemWidget();

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
	SInventoryWidget* m_ParentWidget;

	bool m_bIsInInventory;

	
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

	// New way
	static TSharedRef<FInventoryItemDragDrop> New
	(
		SInventoryItemWidget* p_DraggedItem           , 
		const float           p_ScreenToViewportRatio , 
		const FVector2D&      p_WidgetSize            , 
		UPickupEntity*        p_ItemEntity            ,
		FTile*                p_Tile
	);

	FORCEINLINE const FVector2D GET_WidgetMiddlePosition() const { return m_DecoratorSize * 0.5f * m_ScreenToViewportRatio; }
	FORCEINLINE UPickupEntity*  GET_ItemEntity()           const { return m_ItemEntity;                                     }
	FORCEINLINE FTile*          GET_Tile()                 const { return m_Tile;                                           }

private:

	FSlateBrush           m_IconBrush             ;
	SInventoryItemWidget* m_DraggedItem = nullptr ;
	float                 m_ScreenToViewportRatio ;
	FVector2D             m_DecoratorSize         ;
	UPickupEntity*        m_ItemEntity = nullptr  ;
	FTile*                m_Tile       = nullptr  ;

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