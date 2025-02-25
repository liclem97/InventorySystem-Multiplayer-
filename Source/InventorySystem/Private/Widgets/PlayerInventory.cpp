// Copyright by liclem97.


#include "Widgets/PlayerInventory.h"

#include "Actor/Container.h"
#include "Character/InventoryCharacter.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/UniformGridPanel.h"
#include "DragDrop/DragDrop.h"
#include "PlayerController/InventoryPlayerController.h"
#include "Widgets/InventoryGrid.h"
#include "Widgets/InventorySlot.h"

void UPlayerInventory::NativeConstruct()
{
	Super::NativeConstruct();

	InventoryCharacter = InventoryCharacter == nullptr ? Cast<AInventoryCharacter>(GetOwningPlayerPawn()) : InventoryCharacter;

	Grid_Inventory_World->SetIsWorldInventory(true);
	Grid_Inventory_World->SetPlayerInventory(this);

	Grid_Inventory_Player->SetIsWorldInventory(false);
	Grid_Inventory_Player->SetPlayerInventory(this);

	Button_CloseContainer->OnClicked.AddDynamic(this, &UPlayerInventory::OnClicked_Button_CloseContainer);
	Button_PlaceItem->OnClicked.AddDynamic(this, &UPlayerInventory::OnClicked_Button_PlaceItem);
	Button_TakeItem->OnClicked.AddDynamic(this, &UPlayerInventory::OnClicked_Button_TakeItem);
}

void UPlayerInventory::ItemDropped(bool bDroppedInWorldInventory, bool bDroppedInPlayerInventory, FName InItemRowName, int32 InItemAmount, bool bWasWorldItem, int32 InventoryIndex)
{	
	TArray<FInventoryContents> Local_DroppedItems;
	FInventoryContents NewItem;
	NewItem.ItemRowName = InItemRowName;
	NewItem.ItemAmount = InItemAmount;
	Local_DroppedItems.Add(NewItem);

	if (bDroppedInWorldInventory)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString("Dropped on World Inventory."));
		if (bWasWorldItem) // World -> World.
		{
			InventoryCharacter->Server_AddItemToContainer(Local_DroppedItems);
		}
		else // Player -> World.
		{

		}

	}
	else if (bDroppedInPlayerInventory)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString("Dropped on Player Inventory."));
		if (bWasWorldItem) // World -> Player.
		{

		}
		else // Player -> Player.
		{
			InventoryCharacter->Server_AddItemToInventory(Local_DroppedItems, nullptr, InventoryIndex);
		}
	}
	else if (!bDroppedInWorldInventory && !bDroppedInPlayerInventory)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString("Dropped into World."));
		if (bWasWorldItem)
		{
			InventoryCharacter->Server_AddItemToContainer(Local_DroppedItems);
		}
		else
		{
			InventoryCharacter->Server_AddItemToInventory(Local_DroppedItems, nullptr, InventoryIndex);
		}
	}
}

void UPlayerInventory::OnClicked_Button_CloseContainer()
{
	AInventoryPlayerController* InventoryPlayerController = Cast<AInventoryPlayerController>(GetOwningPlayer());
	if (InventoryPlayerController)
	{
		InventoryPlayerController->LeaveInventoryMenu();
		this->RemoveFromParent();
	}
}

// Player Inventory -> World Container.
void UPlayerInventory::OnClicked_Button_PlaceItem()
{	
	if (!All_InventorySlot_Player.IsEmpty() && InventoryCharacter)
	{	
		int32 PlaceItemIndex = InventoryCharacter->FindFirstItemIndex();
		if (PlaceItemIndex == -1)
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerInventory: No item found in Player Inventory."));
			return;
		}
		Temp_ItemName = All_InventorySlot_Player[PlaceItemIndex]->GetItemRowName();
		TArray<FInventoryContents> PlaceArray;
		FInventoryContents PlaceItem;
		PlaceItem.ItemRowName = Temp_ItemName;
		PlaceItem.ItemAmount = 1;
		PlaceArray.Add(PlaceItem);

		InventoryCharacter->Server_RemoveItemFromInventory(PlaceArray, false, PlaceItemIndex);
		InventoryCharacter->Server_AddItemToContainer(PlaceArray);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerInventory: All_InventorySlot_Player is empty or InventoryCharacter is nullptr."));
	}
}

// World Container -> Player Inventory.
void UPlayerInventory::OnClicked_Button_TakeItem()
{
	if (!All_InventorySlot_World.IsEmpty() && InventoryCharacter && InventoryCharacter->GetOpenedContainer())
	{	
		int32 ItemIndex = InventoryCharacter->GetOpenedContainer()->FindFirstItemIndex();
		if (ItemIndex == -1)
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerInventory: No item found in World Inventory."));
			return;
		}
		Temp_ItemName = All_InventorySlot_World[ItemIndex]->GetItemRowName();
		TArray<FInventoryContents> TakeArray;
		FInventoryContents TakeItem;
		TakeItem.ItemRowName = Temp_ItemName;
		TakeItem.ItemAmount = 1;
		TakeArray.Add(TakeItem);

		int32 TakeItemIndex = InventoryCharacter->FindEmptySlot();

		InventoryCharacter->Server_RemoveItemFromContainer(TakeArray);
		InventoryCharacter->Server_AddItemToInventory(TakeArray, nullptr, TakeItemIndex);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerInventory: All_InventorySlot_World is empty or InventoryCharacter or OpenedContainer is nullptr."));
	}
}

void UPlayerInventory::Setup_InventoryGrid(const TArray<FInventoryContents>& InContents, bool bIsPlayerInventory, bool bIsWorldInventory)
{	
	TArray<UInventorySlot*> Local_InventorySlotWidgets = TArray<UInventorySlot*>();;
	UInventoryGrid* Local_InventoryGrid = nullptr;

	if (bIsPlayerInventory)
	{
		Local_InventorySlotWidgets = All_InventorySlot_Player;
		Local_InventoryGrid = Grid_Inventory_Player;		
	}
	else if (bIsWorldInventory)
	{
		Local_InventorySlotWidgets = All_InventorySlot_World;
		Local_InventoryGrid = Grid_Inventory_World;		
	}

	for (UInventorySlot* InInventorySlotWidgets : Local_InventorySlotWidgets)
	{
		InInventorySlotWidgets->RemoveFromParent();
	}
	Local_InventorySlotWidgets.Empty();

	for (int32 i = 0; i < InContents.Num(); i++)
	{
		const FInventoryContents& ArrayContents = InContents[i];

		UInventorySlot* NewInventorySlot = CreateWidget<UInventorySlot>(this, InventorySlotClass);
		if (NewInventorySlot)
		{
			NewInventorySlot->SetItemRowName(ArrayContents.ItemRowName);
			NewInventorySlot->SetItemAmount(ArrayContents.ItemAmount);
			NewInventorySlot->SetIsWorldItem(bIsWorldInventory);
			NewInventorySlot->SetInventoryIndex(i);
			NewInventorySlot->SetPlayerInventory(this);
			NewInventorySlot->SetOwningPlayer(GetOwningPlayer());
			int32 Value = Local_InventorySlotWidgets.Add(NewInventorySlot);
			int32 InRow = Value / 7;
			int32 InColumn = Value % 7;

			if (Local_InventoryGrid)
			{
				Local_InventoryGrid->GetInventoryGrid()->AddChildToUniformGrid(NewInventorySlot, InRow, InColumn);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Setup_InventoryGrid: Local_InventoryGrid is nullptr. Please Set World or Player Inventory."));
			}
		}
	}

	if (bIsPlayerInventory)
	{
		All_InventorySlot_Player = Local_InventorySlotWidgets;
	}
	else if (bIsWorldInventory)
	{
		All_InventorySlot_World = Local_InventorySlotWidgets;
	}
}

void UPlayerInventory::NativeDestruct()
{
	Super::NativeDestruct();

}


void UPlayerInventory::Set_ContainerWidgetVisibility(bool NewVisibility)
{
	if (NewVisibility)
	{
		Border_InventoryWorld->SetVisibility(ESlateVisibility::Visible);
		Border_ContainerButtons->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Border_InventoryWorld->SetVisibility(ESlateVisibility::Hidden);
		Border_ContainerButtons->SetVisibility(ESlateVisibility::Hidden);
	}
}
