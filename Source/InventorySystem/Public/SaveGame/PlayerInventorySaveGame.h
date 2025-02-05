// Copyright by liclem97.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "InventoryStructure.h"
#include "PlayerInventorySaveGame.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UPlayerInventorySaveGame : public USaveGame
{
	GENERATED_BODY()

private:
	TArray<FInventoryContents> PlayerInventory;
	
};
