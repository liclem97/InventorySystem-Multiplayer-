// Copyright by liclem97.


#include "Widgets/InventorySlot.h"

#include "Character/InventoryCharacter.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "InventoryStructure.h"
#include "Kismet/KismetTextLibrary.h"
#include "Widgets/PlayerInventory.h"

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

	if (ItemRowName == FName("Empty"))
	{
		Border_InventoryIcon->SetBrushFromTexture(nullptr);
		TextBlock_ItemAmount->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
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

void UInventorySlot::RemoveDraggedItem()
{	
	if (!PlayerInventory && PlayerInventory->GetInventoryCharacter())
	{
		UE_LOG(LogTemp, Warning, TEXT("InventorySlot: PlayerInventory or InventoryCharacter is not valid."));
		return;
	}

	FInventoryContents NewContents;
	NewContents.ItemRowName = ItemRowName;
	NewContents.ItemAmount = ItemAmount;

	TArray<FInventoryContents> ArrayContents;
	ArrayContents.Empty();
	ArrayContents.Add(NewContents);

	if (bIsWorldItem)
	{
		PlayerInventory->GetInventoryCharacter()->Server_RemoveItemFromContainer(ArrayContents);
	}
	else
	{
		PlayerInventory->GetInventoryCharacter()->Server_RemoveItemFromInventory(ArrayContents, false);
	}
}
