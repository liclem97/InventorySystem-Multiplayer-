// Copyright by liclem97.


#include "Widgets/InventorySlot.h"

#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "InventoryStructure.h"
#include "Kismet/KismetTextLibrary.h"

void UInventorySlot::SetItemRowName(FName InItemRowName)
{
	ItemRowName = InItemRowName;
}

void UInventorySlot::SetPlayerInventory(UPlayerInventory* InPlayerInventory)
{
	PlayerInventory = InPlayerInventory;
}

void UInventorySlot::SetItemAmount(int32 InItemAmount)
{
	ItemAmount = InItemAmount;
}

void UInventorySlot::SetIsWorldItem(bool bInIsWorldItem)
{
	bIsWorldItem = bInIsWorldItem;
}

void UInventorySlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (!IsValid(ItemDataTable))
	{
		UE_LOG(LogTemp, Warning, TEXT("InventorySlot: Item DataTable is not valid."));
		return;
	}

	FString ContextString;
	FPickupItemInfo* RowData = ItemDataTable->FindRow<FPickupItemInfo>(ItemRowName, ContextString);
	if (RowData)
	{
		Border_InventoryIcon->SetBrushFromTexture(RowData->HUD_Icon);
		if (ItemAmount > 1)
		{
			TextBlock_ItemAmount->SetVisibility(ESlateVisibility::Visible);
			TextBlock_ItemAmount->SetText(UKismetTextLibrary::Conv_IntToText(ItemAmount, false, false));
		}
		else
		{
			TextBlock_ItemAmount->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UInventorySlot::Update_ItemAmount(int32 InItemAmount)
{	
	ItemAmount = InItemAmount;

	if (ItemAmount > 1)
	{
		TextBlock_ItemAmount->SetVisibility(ESlateVisibility::Visible);
		TextBlock_ItemAmount->SetText(UKismetTextLibrary::Conv_IntToText(ItemAmount, false, false));
	}
	else
	{
		TextBlock_ItemAmount->SetVisibility(ESlateVisibility::Hidden);
	}
}
