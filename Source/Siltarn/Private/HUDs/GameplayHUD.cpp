#include "HUDs/GameplayHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "Siltarn/Public/Slate/Widgets/SGameMessageWidget.h"
#include "Siltarn/Public/Slate/Widgets/SInventoryBagWidget.h"
#include "Siltarn/Public/Slate/Widgets/SInGameBagInventory.h"
#include "Siltarn/Public/Slate/Menus/SProfileMenu.h"
#include "Siltarn/Public/Slate/Menus/SInGameMenu.h"
#include "Siltarn/Public/Slate/Menus/SEchapMenu.h"
#include "Siltarn/Public/Interactables/InteractableEntity.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY(LogClass_AGameplayHUD);



AGameplayHUD::AGameplayHUD()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> _OF_CrosshairTexture
		(TEXT("/Game/Textures/Crosshairs_Plain_Square_White_5x5.Crosshairs_Plain_Square_White_5x5"));

	m_CrosshairTexture = _OF_CrosshairTexture.Object;

	m_CrosshairSize = 10;

	m_NumberOfColumns = 10;
	m_NumberOfRows = 10;
	m_TileSize = 50.0f;

	m_GameMessageCollapseDelay = 2.0f;

	m_bIsInventoryWidgetOpen = false;
	m_bIsEchapMenuOpen = false;
}



void AGameplayHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (GEngine && GEngine->GameViewport)
	{

		// Luciole 12/02/2024 | Need to initialize these variables via GETTERS directly from SProfileMenu instead of doing it this way as 
		// we will also need them for the SInventoryBagWidget
		SAssignNew(m_ProfileMenu         , SProfileMenu  )
			.a_HUDOwner(this)
			.a_NumberOfColumns(m_NumberOfColumns)
			.a_NumberOfRows(m_NumberOfRows)
			.a_TileSize(m_TileSize);
		SAssignNew(m_GameMessageWidget, SGameMessageWidget).a_HUDOwner(this);
		SAssignNew(m_InGameMenu, SInGameMenu).a_HUDOwner(this);
		SAssignNew(m_EscapeMenu, SEchapMenu).a_HUDOwner(this);
		SAssignNew(m_InventoryBagWidget, SInventoryBagWidget);


		bool _bWidgets = m_ProfileMenu       &&
						 m_GameMessageWidget &&
					     m_InGameMenu        &&
						 m_EscapeMenu         ;

		if (_bWidgets)
		{
			GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(m_ProfileMenu         .ToSharedRef())) ;
			GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(m_GameMessageWidget   .ToSharedRef())) ;
			GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(m_InGameMenu          .ToSharedRef())) ;
			GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(m_EscapeMenu          .ToSharedRef())) ;
			GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(m_InventoryBagWidget  .ToSharedRef())) ;

			m_ProfileMenu        ->SetVisibility(EVisibility::Collapsed) ;
			m_GameMessageWidget  ->SetVisibility(EVisibility::Collapsed) ;
			m_InGameMenu         ->SetVisibility(EVisibility::Collapsed) ;
			m_EscapeMenu         ->SetVisibility(EVisibility::Collapsed) ;
			m_InventoryBagWidget ->SetVisibility(EVisibility::Collapsed) ;
		}
		else
		{
			UE_LOG(LogClass_AGameplayHUD, Error, TEXT("PostInitializeComponents() : At least one of the Slate widget is NULL."))
		}
	}
}



void AGameplayHUD::DrawHUD()
{
	Super::DrawHUD();

	if (m_CrosshairTexture)
	{
		m_CenterOfScreen = FVector2D(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

		const FVector2D _CrosshairSize(m_CrosshairTexture->GetSurfaceWidth()  * (float)m_CrosshairSize  ,
									   m_CrosshairTexture->GetSurfaceHeight() * (float)m_CrosshairSize) ;

		const FVector2D _CrosshairDrawPos(m_CenterOfScreen.X - _CrosshairSize.X * 0.5f  , 
										  m_CenterOfScreen.Y - _CrosshairSize.Y * 0.5f) ;

		// Drawing the crosshair
		// Compiler warning : warning C4996: 'UTexture::Resource': Use GetResource() / SetResource() accessors instead. [...]
		// Please update your code to the new API before upgrading to the next release, otherwise your project will no longer compile.
		FCanvasTileItem _TileItem(_CrosshairDrawPos, m_CrosshairTexture->Resource, _CrosshairSize, FLinearColor::White);
		_TileItem.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(_TileItem);
	}
}








void AGameplayHUD::SET_GameMessageWidgetText(const FText& p_Message)
{
	if (m_GameMessageWidget == nullptr)
	{
		UE_LOG(LogClass_AGameplayHUD, Warning, TEXT("DISPLAY_GameMessage() : m_GameMessageWidget is NULL. || Unable to proceed further."));
		return;
	}

	GetWorldTimerManager().SetTimer
	(
		m_GameMessageCollapseTimer, 
		this, 
		&AGameplayHUD::COLLAPSE_GameMessageWidget, 
		m_GameMessageCollapseDelay
	);

	m_GameMessageWidget->SET_TextMessage(p_Message);
	m_GameMessageWidget->SetVisibility(EVisibility::Visible); // Set a timer for the widget to collapsed after x seconds
}



void AGameplayHUD::COLLAPSE_GameMessageWidget()
{
	if (m_GameMessageWidget == nullptr)
	{
		UE_LOG(LogClass_AGameplayHUD, Warning, TEXT("COLLAPSE_GameMessageWidget() : m_GameMessageWidget is NULL. || Unable to proceed further."));
		return;
	}

	m_GameMessageWidget->SetVisibility(EVisibility::Collapsed);
}



void AGameplayHUD::SHUTDOWN_PickupItemWidget()
{
	m_InGameMenu->SetVisibility(EVisibility::Collapsed);
}



void AGameplayHUD::DISPLAY_InteractableEntityTag(const FString& p_ItemName)
{
	if (m_ProfileMenu->GetVisibility() == EVisibility::Visible)
	{
		return;
	}

	m_InGameMenu->SetVisibility(EVisibility::Visible);
	m_InGameMenu->DISPLAY_ItemTag(p_ItemName);
}



void AGameplayHUD::OPEN_ItemBag(AItemBagActor* p_BagActor, const FIntPoint& p_InventorySize, TArray<UPickupEntity*>& p_Items)
{
	if (m_ProfileMenu.IsValid())
	{
		OpenCharacterProfileWidget();
		m_ProfileMenu->OpenExternalInventoryWidget(p_BagActor, p_InventorySize, m_TileSize, p_Items);
	}
}



TSharedPtr<SInGameBagInventory> AGameplayHUD::GET_InGameBagInventory() const
{
	if (!m_InventoryBagWidget.IsValid())
	{
		UE_LOG(LogClass_AGameplayHUD, Error, TEXT("GET_InGameBagInventory() : m_InventoryBagWidget is NULL !"));
		return nullptr;
	}

	return m_InventoryBagWidget->GET_InventoryWidget();
}



TSharedPtr<SInGamePlayerInventoryWidget> AGameplayHUD::GET_InGamePlayerInventoryWidget() const
{
	return m_ProfileMenu->GET_InventoryWidget();
}



void AGameplayHUD::SET_InventoryManager(class UInventoryManager* p_InventoryManager) const
{
	if (m_ProfileMenu)
	{
		m_ProfileMenu->SET_InventoryManager(p_InventoryManager);
	}
}



void AGameplayHUD::OpenEscapeMenu()
{
	if (!m_EscapeMenu.IsValid())
	{
		UE_LOG(LogClass_AGameplayHUD, Error, TEXT("OpenEscapeMenu() : m_EscapeMenu is not valid !"));
		return;
	}

	if (!m_InGameMenu.IsValid())
	{
		UE_LOG(LogClass_AGameplayHUD, Error, TEXT("OpenEscapeMenu() : m_InGameMenu is not valid !"));
		return;
	}


	// If we don't shutdown the m_InGameMenu (which is basically the APickupActor's tag widget, then it will be "above" our SProfileMenu widget
	m_InGameMenu->SetVisibility(EVisibility::Collapsed);

	APlayerController* _PC = GetOwningPlayerController();

	if (_PC)
	{
		FInputModeUIOnly _IM;
		_IM.SetWidgetToFocus(m_EscapeMenu);
		_PC->SetInputMode(_IM);
		_PC->SetShowMouseCursor(true);

		m_EscapeMenu->SetVisibility(EVisibility::Visible);
	}
	else
	{
		UE_LOG(LogClass_AGameplayHUD, Error, TEXT("OpenEscapeMenu() : Could not retrieve the owning player controller !"));
	}
}



void AGameplayHUD::CloseEscapeMenu()
{
	if (!m_EscapeMenu.IsValid())
	{
		UE_LOG(LogClass_AGameplayHUD, Error, TEXT("CloseEscapeMenu() : m_EscapeMenu is not valid !"));
		return;
	}


	APlayerController* _PC = GetOwningPlayerController();

	if (_PC)
	{
		_PC->SetInputMode(FInputModeGameOnly());
		_PC->SetShowMouseCursor(false);

		m_EscapeMenu->SetVisibility(EVisibility::Collapsed);
	}
	else
	{
		UE_LOG(LogClass_AGameplayHUD, Error, TEXT("CloseEscapeMenu() : Could not retrieve the owning player controller !"));
	}
}



void AGameplayHUD::OpenCharacterProfileWidget()
{
	if (!m_ProfileMenu.IsValid())
	{
		UE_LOG(LogClass_AGameplayHUD, Error, TEXT("OpenCharacterProfileWidget() : m_ProfileMenu is not valid !"));
		return;
	}

	if (!m_InGameMenu.IsValid())
	{
		UE_LOG(LogClass_AGameplayHUD, Error, TEXT("OpenCharacterProfileWidget() : m_InGameMenu is not valid !"));
		return;
	}


	// If we don't shutdown the m_InGameMenu (which is basically the APickupActor's tag widget, then it will be "above" our SProfileMenu widget
	m_InGameMenu->SetVisibility(EVisibility::Collapsed);

	APlayerController* _PC = GetOwningPlayerController();

	if (_PC)
	{
		FInputModeUIOnly _IM;
		_IM.SetWidgetToFocus(m_ProfileMenu);
		_PC->SetInputMode(_IM);
		_PC->SetShowMouseCursor(true);

		m_ProfileMenu->SetVisibility(EVisibility::Visible);
	}
	else
	{
		UE_LOG(LogClass_AGameplayHUD, Error, TEXT("OpenCharacterProfileWidget() : Could not retrieve the owning player controller !"));
	}
}



void AGameplayHUD::CloseCharacterProfileWidget()
{
	if (!m_ProfileMenu.IsValid())
	{
		UE_LOG(LogClass_AGameplayHUD, Error, TEXT("CloseCharacterProfileWidget() : m_ProfileMenu is not valid !"));
		return;
	}


	APlayerController* _PC = GetOwningPlayerController();

	if (_PC)
	{
		m_ProfileMenu->CloseExternalInventoryWidget(); // The external inventory widget can only be opened when interacting with a bag, a chest... etc.
		m_ProfileMenu->SetVisibility(EVisibility::Collapsed);

		_PC->SetInputMode(FInputModeGameOnly());
		_PC->SetShowMouseCursor(false);		
	}
	else
	{
		UE_LOG(LogClass_AGameplayHUD, Error, TEXT("CloseCharacterProfileWidget() : Could not retrieve the owning player controller !"));
	}
}