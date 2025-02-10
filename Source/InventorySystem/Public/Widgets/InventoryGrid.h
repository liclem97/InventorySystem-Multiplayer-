// Copyright by liclem97.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryGrid.generated.h"

class UUniformGridPanel;
/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UInventoryGrid : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/** Getter*/
	FORCEINLINE UUniformGridPanel* GetInventoryGrid() const { return InventoryGrid; }
	/** End Getter*/

private:
	UPROPERTY(meta=(BindWidget))
	UUniformGridPanel* InventoryGrid;

};
