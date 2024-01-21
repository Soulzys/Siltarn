#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractInterface.generated.h"

class ASiltarnCharacter;
class ASiltarnPlayerController;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractInterface : public UInterface
{
	GENERATED_BODY()
};

class SILTARN_API IInteractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void INTERFACE_Interact    (ASiltarnPlayerController* p_SiltarnController) = 0;
	virtual void INTERFACE_OnStartFocus(ASiltarnPlayerController* p_SiltarnController) = 0;
	virtual void INTERFACE_OnEndFocus  (ASiltarnPlayerController* p_SiltarnController) = 0;
};
