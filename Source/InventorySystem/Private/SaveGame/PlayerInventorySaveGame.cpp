// Copyright by liclem97.


#include "SaveGame/PlayerInventorySaveGame.h"

void UPlayerInventorySaveGame::SetPlayerInventory(const TArray<FInventoryContents>& InPlayerInventory)
{
	PlayerInventory = InPlayerInventory;
}
