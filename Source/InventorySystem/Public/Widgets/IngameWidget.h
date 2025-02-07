// Copyright by liclem97.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IngameWidget.generated.h"

class UBorder;
class UTextBlock;
/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UIngameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void Set_SetSlotVisibility(bool NewVisibility);

	UFUNCTION(BlueprintCallable)
	void Set_SaveSlotText(FText SlotNumber);
	
protected:
	virtual void NativeConstruct() override;	

private:
	UPROPERTY(meta = (BindWidget))
	UBorder* Border_SaveSlotText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock_SaveSlotHint;
};
