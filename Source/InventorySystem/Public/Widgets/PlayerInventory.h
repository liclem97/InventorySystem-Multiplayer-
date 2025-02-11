// Copyright by liclem97.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryStructure.h"
#include "PlayerInventory.generated.h"

class UInventoryGrid;
class UInventorySlot;
class UBorder;
class UButton;

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UPlayerInventory : public UUserWidget
{
	GENERATED_BODY()

public:
	void Setup_InventoryGrid(const TArray<FInventoryContents>& InContents, bool bIsPlayerInventory, bool bIsWorldInventory);
	void Set_ContainerWidgetVisibility(bool NewVisibility);

protected:
	virtual void NativeDestruct() override;
	virtual void NativeConstruct() override;

	void OnClicked_Button_CloseContainer();

private:
	UPROPERTY(meta = (BindWidget))
	UInventoryGrid* Grid_Inventory_Player;

	UPROPERTY(meta = (BindWidget))
	UInventoryGrid* Grid_Inventory_World;

	UPROPERTY(meta = (BindWidget))
	UBorder* Border_InventoryWorld;

	UPROPERTY(meta = (BindWidget))
	UBorder* Border_ContainerButtons;

	UPROPERTY(meta = (BindWidget))
	UButton* Button_CloseContainer;

	UPROPERTY()
	TArray<UInventorySlot*> All_InventorySlot_Player;

	UPROPERTY()
	TArray<UInventorySlot*> All_InventorySlot_World;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UInventorySlot> InventorySlotClass;
};
