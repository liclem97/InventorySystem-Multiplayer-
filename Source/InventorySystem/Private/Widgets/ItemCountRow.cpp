// Copyright by liclem97.


#include "Widgets/ItemCountRow.h"
#include "Components/TextBlock.h"

void UItemCountRow::NativeConstruct()
{
	Super::NativeConstruct();

	TextBlock_ItemName->SetText(ItemName);
	TextBlock_ItemAmount->SetText(FText::AsNumber(ItemAmount));
}
