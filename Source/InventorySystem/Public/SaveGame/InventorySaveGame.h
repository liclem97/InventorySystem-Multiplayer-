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
	/** Setter */
	void SetPickupActorsInfo(TArray<FWorldInfo_PickupItem>& InActorsInfo);
	void SetIsFirstTimeLoadingMap(bool InFirstTimeLoadingMap);
	/** End Setter */

	/** Getter */
	FORCEINLINE bool GetIsFirstTimeLoadingMap() const { return bIsFirstTimeLoadingMap; }
	FORCEINLINE TArray<FWorldInfo_PickupItem> GetAllPickupActorsInfo() const { return All_PickupActorsInfo; }
	/** End Getter */

private:
	UPROPERTY(VisibleAnywhere)
	TArray<FWorldInfo_PickupItem> All_PickupActorsInfo;

	UPROPERTY(VisibleAnywhere)
	bool bIsFirstTimeLoadingMap = true;
};
