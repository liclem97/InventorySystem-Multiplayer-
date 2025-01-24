// Copyright by liclem97.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "InventoryStructure.h"
#include "InventorySaveGame.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UInventorySaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	void SetPickupActorsInfo(TArray<FWorldInfo_PickupItem>& InActorsInfo);
	
private:
	TArray<FWorldInfo_PickupItem> All_PickupActorsInfo;

	bool bIsFirstTimeLoadingMap = true;
};
