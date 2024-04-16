#pragma once

#include "SlateBasics.h"

DECLARE_LOG_CATEGORY_EXTERN(LogClass_SCharacterProfileWidget, Log, All);

class SItemWidget;

class SILTARN_API SCharacterProfileWidget : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SCharacterProfileWidget) {}

	SLATE_ATTRIBUTE(FVector2D, a_CanvasSize)
	SLATE_ATTRIBUTE(FVector2D, a_ItemSlotSize)
	SLATE_ATTRIBUTE(float    , a_TileSize)

	SLATE_END_ARGS()

public:

	virtual bool SupportsKeyboardFocus() const override { return true; }

	void Construct(const FArguments& p_InArgs);

	bool PLACE_ItemInSlot(SItemWidget* p_ItemWidget);

private:

	void COMPUTE_SlotsSizes();

	void UPDATE_HeadItemWidget(SItemWidget* p_ItemWidget);
	void UPDATE_RightHandWidget(SItemWidget* p_ItemWidget);

private:

	SCanvas::FSlot* m_HeadSlot  = nullptr;
	SCanvas::FSlot* m_ChestSlot = nullptr;
	SCanvas::FSlot* m_PantsSlot = nullptr;
	SCanvas::FSlot* m_NeckSlot  = nullptr;

	TSharedPtr<SItemWidget> m_HeadItemWidget = nullptr;
	TSharedPtr<SItemWidget> m_RightHandItemWidget = nullptr;

	FVector2D m_HeadSlotSize      ; 
	FVector2D m_ChestSlotSize     ;
	FVector2D m_PantsSlotSize     ;
	FVector2D m_HandSlotSize      ;
	FVector2D m_NeckSlotSize      ;
	FVector2D m_ShouldersSlotSize ;
	FVector2D m_GlovesSlotSize    ;
	FVector2D m_BeltSlotSize      ;
	FVector2D m_ShoesSlotSize     ;
	FVector2D m_BagSlotSize       ;


	float m_IntersticeSlotsDistance;


	TAttribute<FVector2D> m_CanvasSize;
	TAttribute<FVector2D> m_ItemSlotSize;
	TAttribute<float> m_TileSize;
};
