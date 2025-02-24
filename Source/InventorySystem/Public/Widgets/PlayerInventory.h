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
class AInventoryCharacter;
class UDragDrop;

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

	UFUNCTION(BlueprintCallable)
	void ItemDropped(bool bDroppedInWorldInventory, bool bDroppedInPlayerInventory, FName InItemRowName, int32 InItemAmount, bool bWasWorldItem, int32 InventoryIndex);

	/** Getter */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE AInventoryCharacter* GetInventoryCharacter() const { return InventoryCharacter; }
	/** End Getter */

protected:
	virtual void NativeDestruct() override;
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnClicked_Button_CloseContainer();

	UFUNCTION()
	void OnClicked_Button_PlaceItem();

	UFUNCTION()
	void OnClicked_Button_TakeItem();

	UFUNCTION(BlueprintCallable)
	void SlotItemSwap(UDragDrop* DragDrop, int32 InInventoryIndex);

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

	UPROPERTY(meta = (BindWidget))
	UButton* Button_TakeItem;

	UPROPERTY(meta = (BindWidget))
	UButton* Button_PlaceItem;

	UPROPERTY()
	TArray<UInventorySlot*> All_InventorySlot_Player;

	UPROPERTY()
	TArray<UInventorySlot*> All_InventorySlot_World;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UInventorySlot> InventorySlotClass;

	UPROPERTY()
	AInventoryCharacter* InventoryCharacter;

	UPROPERTY()
	FName Temp_ItemName;
};
