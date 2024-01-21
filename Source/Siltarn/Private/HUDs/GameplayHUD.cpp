#include "HUDs/GameplayHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "Siltarn/Public/Slate/Widgets/SGameMessageWidget.h"
#include "Siltarn/Public/Slate/Menus/SProfileMenu.h"
#include "Siltarn/Public/Slate/Menus/SInGameMenu.h"
#include "Siltarn/Public/Slate/Menus/SEchapMenu.h"
#include "Siltarn/Public/Interactables/InteractableEntity.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY(LogClass_GameplayHUD);

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
		SAssignNew(m_ProfileMenu         , SProfileMenu  )
			.a_HUDOwner(this)
			.a_NumberOfColumns(m_NumberOfColumns)
			.a_NumberOfRows(m_NumberOfRows)
			.a_TileSize(m_TileSize);
		SAssignNew(m_GameMessageWidget, SGameMessageWidget).a_HUDOwner(this);
		SAssignNew(m_InGameMenu, SInGameMenu).a_HUDOwner(this);
		SAssignNew(m_EchapMenu, SEchapMenu).a_HUDOwner(this);

		bool _bWidgets = m_ProfileMenu       &&
						 m_GameMessageWidget &&
					     m_InGameMenu        &&
						 m_EchapMenu          ;

		if (_bWidgets)
		{
			GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(m_ProfileMenu       .ToSharedRef()));
			GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(m_GameMessageWidget .ToSharedRef()));
			GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(m_InGameMenu        .ToSharedRef()));
			GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(m_EchapMenu         .ToSharedRef()));

			m_ProfileMenu        ->SetVisibility(EVisibility::Collapsed);
			m_GameMessageWidget  ->SetVisibility(EVisibility::Collapsed);
			m_InGameMenu         ->SetVisibility(EVisibility::Collapsed);
			m_EchapMenu          ->SetVisibility(EVisibility::Collapsed);
		}
		else
		{
			UE_LOG(LogClass_GameplayHUD, Warning, TEXT("PostInitializeComponents() : At least one of the Slate widget is NULL."))
		}
	}
}

void AGameplayHUD::DrawHUD()
{
	Super::DrawHUD();

	if (m_CrosshairTexture)
	{
		m_CenterOfScreen = FVector2D(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

		const FVector2D _CrosshairSize(m_CrosshairTexture->GetSurfaceWidth() * (float)m_CrosshairSize,
									   m_CrosshairTexture->GetSurfaceHeight() * (float)m_CrosshairSize);

		const FVector2D _CrosshairDrawPos(m_CenterOfScreen.X - _CrosshairSize.X * 0.5f, 
										  m_CenterOfScreen.Y - _CrosshairSize.Y * 0.5f);

		// Drawing the crosshair
		// Compiler warning : warning C4996: 'UTexture::Resource': Use GetResource() / SetResource() accessors instead. [...]
		// Please update your code to the new API before upgrading to the next release, otherwise your project will no longer compile.
		FCanvasTileItem _TileItem(_CrosshairDrawPos, m_CrosshairTexture->Resource, _CrosshairSize, FLinearColor::White);
		_TileItem.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(_TileItem);
	}
}

EWidgetVisibilityState AGameplayHUD::TOGGLE_InventoryWidget()
{
	if (m_ProfileMenu == nullptr)
	{
		UE_LOG(LogClass_GameplayHUD, Warning, TEXT("TOGGLE_InventoryWidget() : m_ProfileMenu is NULL. || Unable to proceed further."));
		return EWidgetVisibilityState::BUG;
	}

	if (m_bIsInventoryWidgetOpen)
	{
		m_ProfileMenu->SetVisibility(EVisibility::Collapsed);
		m_bIsInventoryWidgetOpen = false;
		
		return EWidgetVisibilityState::COLLAPSED;
	}
	else
	{
		m_ProfileMenu->SetVisibility(EVisibility::Visible);
		m_bIsInventoryWidgetOpen = true;
		m_InGameMenu->SetVisibility(EVisibility::Collapsed);
		
		return EWidgetVisibilityState::VISIBLE;
	}
}

EWidgetVisibilityState AGameplayHUD::TOGGLE_EchapMenu()
{
	if (m_EchapMenu == nullptr)
	{
		UE_LOG(LogClass_GameplayHUD, Warning, TEXT("TOGGLE_EchapMenu() : m_EchapMenu is NULL. || Unable to proceed further."));
		return EWidgetVisibilityState::BUG;
	}

	if (m_bIsEchapMenuOpen)
	{
		m_EchapMenu->SetVisibility(EVisibility::Collapsed);
		m_bIsEchapMenuOpen = false;

		return EWidgetVisibilityState::COLLAPSED;
	}
	else
	{
		m_EchapMenu->SetVisibility(EVisibility::Visible);
		m_bIsEchapMenuOpen = true;

		return EWidgetVisibilityState::VISIBLE;
	}
}

void AGameplayHUD::SET_GameMessageWidgetText(const FText& p_Message)
{
	if (m_GameMessageWidget == nullptr)
	{
		UE_LOG(LogClass_GameplayHUD, Warning, TEXT("DISPLAY_GameMessage() : m_GameMessageWidget is NULL. || Unable to proceed further."));
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
		UE_LOG(LogClass_GameplayHUD, Warning, TEXT("COLLAPSE_GameMessageWidget() : m_GameMessageWidget is NULL. || Unable to proceed further."));
		return;
	}

	m_GameMessageWidget->SetVisibility(EVisibility::Collapsed);
}

void AGameplayHUD::SHUTDOWN_PickupItemWidget()
{
	m_InGameMenu->SetVisibility(EVisibility::Collapsed);
}

bool AGameplayHUD::DOES_InventoryHasRoomForItem(const FIntPoint& p_ItemSize)
{
	bool _bEnoughRoomForItem = m_ProfileMenu->DOES_InventoryHasRoomForItem(p_ItemSize);
	
	if (!_bEnoughRoomForItem)
	{
		const FText _Text = FText::FromString("The inventory is full !");
		SET_GameMessageWidgetText(_Text);
	}

	return _bEnoughRoomForItem;
}

bool AGameplayHUD::ADD_ItemToInventory(UPickupEntity* p_Item)
{
	if (p_Item)
	{
		return m_ProfileMenu->ADD_ItemToInventory(p_Item);
	}

	return false;
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
