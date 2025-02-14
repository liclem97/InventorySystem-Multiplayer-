// Copyright by liclem97.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemCountRow.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UItemCountRow : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	FText ItemName;

	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	int32 ItemAmount;

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock_ItemName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock_ItemAmount;	
};
