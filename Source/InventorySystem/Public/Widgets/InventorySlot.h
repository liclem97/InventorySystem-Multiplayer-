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
	/** End Getter*/

	/** Setter */
	void SetItemRowName(FName InItemRowName);
	void SetPlayerInventory(UPlayerInventory* InPlayerInventory);
	void SetItemAmount(int32 InItemAmount);
	void SetIsWorldItem(bool bInIsWorldItem);
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
private:
	UPROPERTY(meta = (BindWidget))
	UBorder* Border_InventoryIcon;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock_ItemAmount;

	UPROPERTY(EditDefaultsOnly, Category = "DataTable")
	UDataTable* ItemDataTable;
	
};
