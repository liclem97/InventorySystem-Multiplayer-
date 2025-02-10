// Copyright by liclem97.


#include "Widgets/PlayerInventory.h"

#include "Components/UniformGridPanel.h"
#include "Widgets/InventoryGrid.h"
#include "Widgets/InventorySlot.h"

void UPlayerInventory::Setup_InventoryGrid(TArray<FInventoryContents>& InContents, bool bPlayerInventory, bool bWorldInventory)
{
	if (bPlayerInventory)
	{
		TArray<UInventorySlot*> Local_InventorySlotWidgets = All_InventorySlot_Player;
		UInventoryGrid* Local_InventoryGrid = Grid_Inventory_Player;

		for (UInventorySlot* InInventorySlotWidgets : Local_InventorySlotWidgets)
		{
			InInventorySlotWidgets->RemoveFromParent();
		}
		Local_InventorySlotWidgets.Empty();
		
		for (FInventoryContents ArrayContents : InContents)
		{
			UInventorySlot* NewInventorySlot = CreateWidget<UInventorySlot>(this, InventorySlotClass);
			if (NewInventorySlot)
			{
				NewInventorySlot->SetItemRowName(ArrayContents.ItemRowName);
				NewInventorySlot->SetItemAmount(ArrayContents.ItemAmount);
				NewInventorySlot->SetIsWorldItem(false);
				NewInventorySlot->SetPlayerInventory(this);
				NewInventorySlot->SetOwningPlayer(GetOwningPlayer());

				int32 Value = Local_InventorySlotWidgets.Add(NewInventorySlot);
				int32 InRow = Value % 7;
				int32 InColumn = Value % 7;
				Local_InventoryGrid->GetInventoryGrid()->AddChildToUniformGrid(NewInventorySlot, InRow, InColumn);
			}
		}
		All_InventorySlot_Player = Local_InventorySlotWidgets;
	}
}
