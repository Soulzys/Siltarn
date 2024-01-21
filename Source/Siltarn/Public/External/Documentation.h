#pragma once

/*

	*** How to deal with SBorder ***
	 
	@ To remove the border effect when using an "empty" SBorder with no image 
			.BorderImage(FCoreStyle::Get().GetBrush("NoBorder")


	@ To change the background color of an "empty" SBorder
		1) Set a plain white border image as the brush. FSiltarnGeneralStyleContainerStruct already contains such an image : m_WhiteBackgroundImage_SlateBrush
				.BorderImage(&m_GeneralStyle.m_WhiteBackgroundImage_SlateBrush)

		2) Set the border background color using either an FLinearColor or an FSlateColor. 
		   The latter can be set with an FLinearColor as below : 
				.BorderBackgroundColor(FSlateColor(FLinearColor(r, g, b, a)))





	*** How to deal with Slate events ***

	@ To write custom FReply
		In .h   --> FReply CustomDetectDrag(SCompoundWidget* Widget, const FPointerEvent& MouseEvent, FKey DragKey);
		In .cpp --> FReply CustomDetectDrag(SCompoundWidget* Widget, const FPointerEvent& MouseEvent, FKey DragKey)
					{
						blabla 
						blabla

						if (blabla)
						{
							return FReply::Unhandled();
						}

						return FReply::Handled();
					}

		For a real example, check the CustomDetectDrag of MyWindow class in the DragPro project. 
		That example was written from https://github.com/DavidCRicardo/DragAndDrop/tree/main/Source/DragAndDrop
			
		




	*** How to load an asset at runtime ***
	
	UMaterialInstance* _Material = (UMaterialInstance*)StaticLoadObject(UMaterialInstance::StaticClass(), nullptr, *asset_path);

	Note : the path of an asset can be obtained through GetPathName(). 
	 
	
*/