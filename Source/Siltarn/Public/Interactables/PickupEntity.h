#pragma once

#include "CoreMinimal.h"
#include "Interactables/InteractableEntity.h"
#include "Siltarn/Public/Interactables/InteractableEnums.h"
#include "PickupEntity.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogClass_UPickupEntity, Log, All);

class UMaterialInstance  ;
class AInteractableActor ;
class APickupActor       ;


UENUM()
enum class EItemAnchorType : uint8
{
	Tile  = 0 , 
	Cross = 1 , 
	Line  = 2 , 
	Error = 3
};

UENUM()
enum class EPickupEntityType : uint8
{
	Pistol     = 0, 
	Ammunition = 1, 
	NotSet     = 2
};

UCLASS()
class SILTARN_API UPickupEntity : public UInteractableEntity
{
	GENERATED_BODY()

public:

	UPickupEntity();
	~UPickupEntity();

	void Initialize_Entity(APickupActor* p_IA);

	virtual bool IS_EntityUsable(bool p_bDisplayLogs = false);

	EItemAnchorType           GET_AnchorType()  const ;
	virtual EPickupEntityType GET_ItemType()    const ;
	const   FLinearColor&     GET_RaretyColor() const ;

	FORCEINLINE const FIntPoint&          GET_InventorySpace() const { return m_InventorySpace ; }
	FORCEINLINE UMaterialInstance*        GET_Icon()           const { return m_Icon           ; }
	FORCEINLINE TSubclassOf<APickupActor> GET_ActorClass()     const { return m_ActorClass     ; }
	FORCEINLINE int32                     GET_ItemId()         const { return m_ItemId         ; }
	FORCEINLINE int32                     GET_ResellValue()    const { return m_ResellValue    ; }
	FORCEINLINE const FString&            GET_Description()    const { return m_Description    ; }
	FORCEINLINE bool                      IS_Equipable()       const { return m_bIsEquipable   ; }

	void SET_ItemId       (uint32 p_NewId); // This function should only be called once, in UInventory in CREATE_NewItemId()
	void SET_ControlTileId(uint32 p_NewId);

	friend bool operator<(const UPickupEntity& a, const UPickupEntity& b)
	{
		return a.GET_ItemId() < b.GET_ItemId();
	}

protected:

	uint8 GET_NumberType(uint32 p_N) const;

protected:

	UPROPERTY() UMaterialInstance*        m_Icon = nullptr       ; // UPROPERTY is added in order to avoid m_Icon to be removed by the garbage collector
	            FIntPoint                 m_InventorySpace       ;
	UPROPERTY()	TSubclassOf<APickupActor> m_ActorClass = nullptr ;
	            int32                     m_ResellValue          ;
	            FString                   m_Description          ;
	            EPickupActorRaretyType    m_Rarety               ;
				bool                      m_bIsEquipable         ;
				

	FLinearColor m_RaretyTypesColors[5] =
	{ 
		FLinearColor(1.0f      , 1.0f      , 1.0f      , 1.0f) , // White  --> Common
		FLinearColor(0.07188f  , 0.526042f , 0.0f      , 1.0f) , // Green  --> Uncommon
		FLinearColor(0.0f      , 0.268015f , 1.0f      , 1.0f) , // Blue   --> Rare
		FLinearColor(0.566864f , 0.0f      , 0.697917f , 1.0f) , // Purple --> Epic
		FLinearColor(0.947917f , 0.221753f , 0.0f      , 1.0f)   // Orange --> Legendary
	};


	

	// *** Item data that are endogenous to UPickupEntity
	//
	uint32 m_ItemId        ; // Item id that is used to retrieve the item from its TArray container. 	
	uint32 m_ControlTileId ; // Set and updated in SInventoryWidget
};
