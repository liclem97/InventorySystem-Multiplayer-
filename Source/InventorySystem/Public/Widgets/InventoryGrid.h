// Copyright by liclem97.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryGrid.generated.h"

class UUniformGridPanel;
class UPlayerInventory;

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UInventoryGrid : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/** Setter */
	void SetIsWorldInventory(bool bNewWorldInventory) { bIsWorldInventory = bNewWorldInventory; }
	void SetPlayerInventory(UPlayerInventory* InPlayerInventory) { PlayerInventory = InPlayerInventory; }
	/** End Setter */

	/** Getter*/
	FORCEINLINE UUniformGridPanel* GetInventoryGrid() const { return InventoryGrid; }
	/** End Getter*/

protected:
	UPROPERTY(BlueprintReadOnly)
	UPlayerInventory* PlayerInventory;

	UPROPERTY(BlueprintReadOnly)
	bool bIsWorldInventory;

private:
	UPROPERTY(meta=(BindWidget))
	UUniformGridPanel* InventoryGrid;

};
