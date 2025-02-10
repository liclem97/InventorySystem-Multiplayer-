// Copyright by liclem97.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryStructure.h"
#include "PlayerInventory.generated.h"

class UInventoryGrid;
class UInventorySlot;
/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UPlayerInventory : public UUserWidget
{
	GENERATED_BODY()

protected:
	void Setup_InventoryGrid(TArray<FInventoryContents>& InContents, bool bPlayerInventory, bool bWorldInventory);	

private:
	UPROPERTY(meta = (BindWidget))
	UInventoryGrid* Grid_Inventory_Player;

	UPROPERTY()
	TArray<UInventorySlot*> All_InventorySlot_Player;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UInventorySlot> InventorySlotClass;
};
