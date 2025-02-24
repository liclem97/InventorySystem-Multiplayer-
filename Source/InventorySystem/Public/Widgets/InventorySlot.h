// Copyright by liclem97.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlot.generated.h"

class UBorder;
class UTextBlock;
class UPlayerInventory;
/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Getter*/
	FORCEINLINE FName GetItemRowName() const { return ItemRowName; }
	FORCEINLINE int32 GetItemAmount() const { return ItemAmount; }
	FORCEINLINE int32 GetItemIndex() const { return InventoryIndex; }
	FORCEINLINE bool GetIsWorldItem() const { return bIsWorldItem; }
	/** End Getter*/

	/** Setter */
	void SetItemRowName(FName InItemRowName);
	void SetPlayerInventory(UPlayerInventory* InPlayerInventory);
	void SetItemAmount(int32 InItemAmount);
	void SetIsWorldItem(bool bInIsWorldItem);
	void SetInventoryIndex(int32 InIndex) { InventoryIndex = InIndex; }
	/** End Setter*/

protected:
	virtual void NativeConstruct() override;
	void Update_ItemAmount(int32 InItemAmount);

	UFUNCTION(BlueprintCallable)
	void RemoveDraggedItem();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	FName ItemRowName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	UPlayerInventory* PlayerInventory;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	int32 ItemAmount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	bool bIsWorldItem;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UBorder* Border_Background;

	UPROPERTY(BlueprintReadOnly)
	int32 InventoryIndex;

private:
	UPROPERTY(meta = (BindWidget))
	UBorder* Border_InventoryIcon;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock_ItemAmount;

	UPROPERTY(EditDefaultsOnly, Category = "DataTable")
	UDataTable* ItemDataTable;

	
};
