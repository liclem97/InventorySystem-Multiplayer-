// Copyright by liclem97.


#include "Widgets/PlayerInventory.h"

#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/UniformGridPanel.h"
#include "PlayerController/InventoryPlayerController.h"
#include "Widgets/InventoryGrid.h"
#include "Widgets/InventorySlot.h"

void UPlayerInventory::NativeConstruct()
{
	Super::NativeConstruct();

	Button_CloseContainer->OnClicked.AddDynamic(this, &UPlayerInventory::OnClicked_Button_CloseContainer);
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
			int32 InRow = Value / 7;
			int32 InColumn = Value % 7;
			Local_InventoryGrid->GetInventoryGrid()->AddChildToUniformGrid(NewInventorySlot, InRow, InColumn);
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
