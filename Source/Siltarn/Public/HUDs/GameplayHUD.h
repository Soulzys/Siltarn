#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameplayHUD.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogClass_AGameplayHUD, Log, All);

class UTexture2D;
class SInventoryWidget;
class SInventoryItemWidget;
class SProfileMenu;
class SGameMessageWidget;
class SInGameMenu;
class SEchapMenu;
class SInventoryBagWidget;
class SInGameBagInventory;
class SInGamePlayerInventoryWidget;
class UInteractableEntity;
class UPickupEntity;


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

	void SET_GameMessageWidgetText(const FText& p_Message);
	void SHUTDOWN_PickupItemWidget();

	// New inventory stuff
	//
	void DISPLAY_InteractableEntityTag(const FString& p_ItemName);

	void OPEN_ItemBag(const FIntPoint& p_InventorySize, TArray<UPickupEntity*>& p_Items);

	TSharedPtr<SInGameBagInventory> GET_InGameBagInventory() const;
	TSharedPtr<SInGamePlayerInventoryWidget> GET_InGamePlayerInventoryWidget() const;

	void SET_InventoryManager(class UInventoryManager* p_InventoryManager) const;

	// New ones
	void OpenEscapeMenu();
	void CloseEscapeMenu();
	void OpenCharacterProfileWidget();
	void CloseCharacterProfileWidget();

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
	TSharedPtr<SProfileMenu       > m_ProfileMenu         = nullptr ;
	TSharedPtr<SGameMessageWidget > m_GameMessageWidget   = nullptr ;
	TSharedPtr<SInGameMenu        > m_InGameMenu          = nullptr ;
	TSharedPtr<SEchapMenu         > m_EscapeMenu          = nullptr ;
	TSharedPtr<SInventoryBagWidget> m_InventoryBagWidget  = nullptr ;

	bool m_bIsInventoryWidgetOpen;
	bool m_bIsEchapMenuOpen;

	FTimerHandle m_GameMessageCollapseTimer;

	FVector2D m_CenterOfScreen;
};
