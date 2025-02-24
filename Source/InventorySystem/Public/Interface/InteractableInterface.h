// Copyright by liclem97.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableInterface.generated.h"

class AInventoryCharacter;

UINTERFACE(MinimalAPI)
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INVENTORYSYSTEM_API IInteractableInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void InteractWithActor(AInventoryCharacter* PlayerCharacter);

	UFUNCTION(BlueprintNativeEvent)
	void SetItemNameVisibility(bool InVisibility);

	UFUNCTION(BlueprintNativeEvent)
	FText GetInteractHintText() const;
};
