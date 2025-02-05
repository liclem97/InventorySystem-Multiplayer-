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

public:
	/** Setter */
	void SetPlayerInventory(const TArray<FInventoryContents>& InPlayerInventory);
	/** End Setter */

	/** Getter */
	FORCEINLINE TArray<FInventoryContents> GetPlayerInventory() const { return PlayerInventory; }
	/** End Getter */

private:
	TArray<FInventoryContents> PlayerInventory;

};
