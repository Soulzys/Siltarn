#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameplayHUD.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogClass_GameplayHUD, Log, All);

class UTexture2D;
class SInventoryWidget;
class SInventoryItemWidget;
class SProfileMenu;
class SGameMessageWidget;
class SInGameMenu;
class SEchapMenu;
class UInteractableEntity;
class UPickupEntity;


UENUM() 
enum class EWidgetVisibilityState : uint8
{
	VISIBLE   = 0, 
	COLLAPSED = 1, 
	BUG       = 2
};

UENUM()
enum class ECrosshairTypes : uint8
{
	POINT = 0, 
	CROSS = 1
};

/*
	This class should eventually renamed as PyralinthHUD
*/

UCLASS()
class SILTARN_API AGameplayHUD : public AHUD
{
	GENERATED_BODY()

public:

	AGameplayHUD();
	virtual void PostInitializeComponents() override;
	virtual void DrawHUD() override;

	// Returns the visibility state of the widget
	EWidgetVisibilityState TOGGLE_InventoryWidget();
	EWidgetVisibilityState TOGGLE_EchapMenu();

	void SET_GameMessageWidgetText(const FText& p_Message);

	void SHUTDOWN_PickupItemWidget();

	// New inventory stuff
	//
	bool DOES_InventoryHasRoomForItem(const FIntPoint& p_ItemSize);
	bool ADD_ItemToInventory(UPickupEntity* p_Item);
	void DISPLAY_InteractableEntityTag(const FString& p_ItemName);

private:

	void COLLAPSE_GameMessageWidget();

private:

	UPROPERTY(EditAnywhere, Category = "Crosshair", meta = (DisplayName = "Crosshair Texture"))
	UTexture2D* m_CrosshairTexture = nullptr;

	UPROPERTY(EditAnywhere, Category = "Crosshair", meta = (DisplayName = "Crosshair Size")) // Acts like scale
	int32 m_CrosshairSize;
	
	UPROPERTY(EditAnywhere, Category = "Crosshair", meta = (DisplayName = "Crosshair Type"))
	ECrosshairTypes m_CrosshairType;

	// Inventory
	//
	UPROPERTY(EditAnywhere, Category = "Inventory", meta = (DisplayName = "Number of columns"))
	int32 m_NumberOfColumns;
	
	UPROPERTY(EditAnywhere, Category = "Inventory", meta = (DisplayName = "Number of rows"))
	int32 m_NumberOfRows;

	UPROPERTY(EditAnywhere, Category = "Inventory", meta = (DisplayName = "Tile size"))
	int32 m_TileSize;


	// Timers
	//
	UPROPERTY(EditAnywhere, Category = "Timers", meta = (DisplayName = "GameMessage collapse delay"))
	float m_GameMessageCollapseDelay;


	// *** Slate widgets *** //
	//
	TSharedPtr<SProfileMenu      > m_ProfileMenu         = nullptr;
	TSharedPtr<SGameMessageWidget> m_GameMessageWidget   = nullptr;
	TSharedPtr<SInGameMenu       > m_InGameMenu          = nullptr;
	TSharedPtr<SEchapMenu        > m_EchapMenu           = nullptr;

	bool m_bIsInventoryWidgetOpen;
	bool m_bIsEchapMenuOpen;

	FTimerHandle m_GameMessageCollapseTimer;

	FVector2D m_CenterOfScreen;

	int32 m_Toto;
};
