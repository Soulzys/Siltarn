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


	*** How to load an asset at compile time *** 
	
	const ConstructorHelpers::FObjectFinder<ObjectType>LoadingObject(TEXT("ReferencePath"));

	m_ClassMember = LoadingObject.Object;

	/!\ Note 1 : A check can be used as follow : check(LoadingObject.Succeeded());
	/!\ Note 2 : The "ReferencePath" has to be modified to fit UE's standards. Example of a valid BP's reference path : /Game/Blueprints/Interactables/BP_ItemBagActor
	 
	




	*** LOGGING *** 
	
	int64 -> %lld
	TCHAR -> %c
*/






/*
	*** About UPickupEntity ID and how it is computed ***
	 
	- Id for items are stored in a int64 (8 bits variable on a 64 processor)
	- They are supposed to be unique
	- They are supposed to be 18 digits long

	Computing process : 
		a) Get the local time (not UTC) as an FString --> year.month.day-hour.min.sec
		b) Append the milliseconds to it (only the ISO 8601 version has milliseconds natively)
		c) Since milliseconds range from 0 to 999 and we want to end up with an int64 with a constant number of digits : 
			1/ If 10 < milliseconds < 100, then append a RandRange(1, 9) to it
			2/ If milliseconds < 10, then append a RandRange(10, 99) to it
		d) Since we do not expect our game still be running in the 22nd century (or at least this current version of it), 
		   we assume we do not need the first two digits of the ID to always be "24". 
		   Consequently, we switch these first two digits with random numbers ranging from 1 to 9.
		e) We add a final random number ranging from 1 to 9 to the end of the ID.



		0 0 00 00 00 00 00 00 0 0 0 0			| Every UPickupEntity object id is supposed 
		^ ^  ^  ^  ^  ^  ^  ^ ^ ^ ^ ^			| to follow this structural pattern
		| |  |  |  |  |  |  | | | | |
		R R  Y  M  D  H  M  S M M M R
		N N  E  O  A  O  I  E S R R N


		RN -> Random number
		YE -> Last two digits of play current year
		MR -> Millisecond or random number
		   
*/



/*
	LANDMINE

	@
	@
	@

	Mine 1 : FSlateApplication::SetUserFocus()
	------------------------------------------

	When we use FSlateApplication::SetUserFocus(), such as in SInventoryItemWidget::OnMouseEnter() or SInventoryWidget::OnDrop(), 
	we pass in 0 as the User index. This may come back to bite us later when we deal with multiplayer.




	Mine 2 : SInventoryItemWidget::GET_ScreenToViewportRatio()
	----------------------------------------------------------
	
	I forgot how we got to deal with it this way, but right now we are working with a static resolution of 1920x1080. 
	We would ideally need to make this dynamic and let everything adapt to the viewport size. 
*/

/*
	MYSTERIES LAND

	@
	@
	@

	Mystery 1 : Look for the Luciole in SInventoryItemWidget::OnMouseLeave()
	------------------------------------------------------------------------
*/