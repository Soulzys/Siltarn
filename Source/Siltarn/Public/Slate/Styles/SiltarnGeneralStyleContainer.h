#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateWidgetStyleContainerBase.h"
#include "SiltarnGeneralStyleContainer.generated.h"

struct FSlateWidgetStyle;
class UTexture2D;

USTRUCT()
struct FSiltarnGeneralStyleContainerStruct : public FSlateWidgetStyle
{
	GENERATED_BODY()

public:

	virtual void GetResources(TArray<const FSlateBrush*>& OutBrushes) const override;

	// *** GETTERS *** //
	//
	virtual const FName GetTypeName() const override { return TypeName; }
	static const FSiltarnGeneralStyleContainerStruct& GetDefault();

public:

	static const FName TypeName;

	// *** Slate properties *** //
	//
	// Slate brushes
	UPROPERTY(EditAnywhere, Category = "Slate Brush", meta = (DisplayName = "Inventory"             )) FSlateBrush m_Inventory_SlateBrush            ;
	UPROPERTY(EditAnywhere, Category = "Slate Brush", meta = (DisplayName = "Inventory Item Icon"   )) FSlateBrush m_InventoryItemIcon_SlateBrush    ;
	UPROPERTY(EditAnywhere, Category = "Slate Brush", meta = (DisplayName = "Debugging Blue"        )) FSlateBrush m_DebuggingBlue_SlateBrush        ;
	UPROPERTY(EditAnywhere, Category = "Slate Brush", meta = (DisplayName = "Debugging Red"         )) FSlateBrush m_DebuggingRed_SlateBrush         ;
	UPROPERTY(EditAnywhere, Category = "Slate Brush", meta = (DisplayName = "Debugging Green"       )) FSlateBrush m_DebuggingGreen_SlateBrush       ;
	UPROPERTY(EditAnywhere, Category = "Slate Brush", meta = (DisplayName = "Debugging Cyan"        )) FSlateBrush m_DebuggingCyan_SlateBrush        ;
	UPROPERTY(EditAnywhere, Category = "Slate Brush", meta = (DisplayName = "Debugging Yellow"      )) FSlateBrush m_DebuggingYellow_SlateBrush      ;
	UPROPERTY(EditAnywhere, Category = "Slate Brush", meta = (DisplayName = "Debugging Magenta"     )) FSlateBrush m_DebuggingMagenta_SlateBrush     ;
	UPROPERTY(EditAnywhere, Category = "Slate Brush", meta = (DisplayName = "White Background Image")) FSlateBrush m_WhiteBackgroundImage_SlateBrush ;
	UPROPERTY(EditAnywhere, Category = "Slate Brush", meta = (DisplayName = "Item Tooltip Border"   )) FSlateBrush m_ItemTooltipBorder_SlateBrush    ;
	UPROPERTY(EditAnywhere, Category = "Slate Brush", meta = (DisplayName = "ItemBag Icon"          )) FSlateBrush m_ItemBagIcon_SlateBrush          ;
	UPROPERTY(EditAnywhere, Category = "Slate Brush - Alphabet", meta = (DisplayName = "E"            )) FSlateBrush m_Alphabet_E_SlateBrush         ;
	UPROPERTY(EditAnywhere, Category = "Slate Brush - Box"     , meta = (DisplayName = "Alphabet Box" )) FSlateBrush m_Box_Alphabet_SlateBrush       ;


	// Button Style
	UPROPERTY(EditAnywhere, Category = "Button Style", meta = (DisplayName = "Echap Menu")) FButtonStyle m_EchapMenu_ButtonStyle;

	// SWindow Style
	UPROPERTY(EditAnywhere, Category = "SWindow Style", meta = (DisplayName = "Item Tooltip Window")) FWindowStyle m_ItemTooltip_WindowStyle;
	
	// Slate colors
	UPROPERTY(EditAnywhere, Category = "Slate Color", meta = (DisplayName = "Inventory Background Image")) FSlateColor m_InventoryBackgroundImage_SlateColor ;
	UPROPERTY(EditAnywhere, Category = "Slate Color", meta = (DisplayName = "Debugging Blue"            )) FSlateColor m_DebuggingBlue_SlateColor            ;
	UPROPERTY(EditAnywhere, Category = "Slate Color", meta = (DisplayName = "Debugging Red"             )) FSlateColor m_DebuggingRed_SlateColor             ;
	UPROPERTY(EditAnywhere, Category = "Slate Color", meta = (DisplayName = "Debugging Green"           )) FSlateColor m_DebuggingGreen_SlateColor           ;
	UPROPERTY(EditAnywhere, Category = "Slate Color", meta = (DisplayName = "Debugging Yellow"          )) FSlateColor m_DebuggingYellow_SlateColor          ;
	UPROPERTY(EditAnywhere, Category = "Slate Color", meta = (DisplayName = "Debugging Cyan"            )) FSlateColor m_DebuggingCyan_SlateColor            ;
	UPROPERTY(EditAnywhere, Category = "Slate Color", meta = (DisplayName = "Debugging Magenta"         )) FSlateColor m_DebuggingMagenta_SlateColor         ;
	UPROPERTY(EditAnywhere, Category = "Slate Color", meta = (DisplayName = "Error Text"                )) FSlateColor m_ErrorText_SlateColor                ;

	// Texture
	UPROPERTY(EditAnywhere, Category = "Texture 2D", meta = (DisplayName = "Testing Texture")) UTexture2D* m_TestingTexture = nullptr;

	// Fonts
	UPROPERTY(EditAnywhere, Category = "Fonts"         , meta = (DisplayName = "Monorita"      )) FSlateFontInfo m_Monorita_Font       ;
	UPROPERTY(EditAnywhere, Category = "Fonts - Roboto", meta = (DisplayName = "Roboto Regular")) FSlateFontInfo m_Roboto_Regular_Font ;
	UPROPERTY(EditAnywhere, Category = "Fonts - Roboto", meta = (DisplayName = "Roboto Bold"   )) FSlateFontInfo m_Roboto_Bold_Font    ;
	UPROPERTY(EditAnywhere, Category = "Fonts - Roboto", meta = (DisplayName = "Roboto VBold"  )) FSlateFontInfo m_Roboto_VBold_Font   ;
	UPROPERTY(EditAnywhere, Category = "Fonts - Roboto", meta = (DisplayName = "Roboto Italic" )) FSlateFontInfo m_Roboto_Italic_Font  ;

	// Linear Color
	UPROPERTY(EditAnywhere, Category = "Linear Color", meta = (DisplayName = "Opacity Light" )) FLinearColor m_Opacity_Light_LColor  ;
	UPROPERTY(EditAnywhere, Category = "Linear Color", meta = (DisplayName = "Opacity Medium")) FLinearColor m_Opacity_Medium_LColor ;


};

UCLASS()
class SILTARN_API USiltarnGeneralStyleContainer : public USlateWidgetStyleContainerBase
{
	GENERATED_BODY()

public:

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return static_cast<const FSlateWidgetStyle*>(&m_SiltarnGeneralStyleContainerStruct);
	}

public:

	UPROPERTY(EditAnywhere, Category = "Appearance", meta = (ShowOnlyInnerProperties))
	FSiltarnGeneralStyleContainerStruct m_SiltarnGeneralStyleContainerStruct;
	
};
