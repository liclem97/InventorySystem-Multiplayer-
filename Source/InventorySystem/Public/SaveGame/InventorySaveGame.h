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
	void SetPickupActorsInfo(const TArray<FWorldInfo_PickupItem>& InActorsInfo);
	void SetContainerActorsInfo(const TArray<FWorldInfo_Containers>& InActorsInfo);
	void SetIsFirstTimeLoadingMap(bool InFirstTimeLoadingMap);
	/** End Setter */

	/** Getter */
	FORCEINLINE bool GetIsFirstTimeLoadingMap() const { return bIsFirstTimeLoadingMap; }
	FORCEINLINE TArray<FWorldInfo_PickupItem> GetAllPickupActorsInfo() const { return All_PickupActorsInfo; }
	FORCEINLINE TArray<FWorldInfo_Containers> GetAllContainerActorsInfo() const { return All_ContainerActorsInfo; }
	/** End Getter */

private:
	UPROPERTY(VisibleAnywhere)
	TArray<FWorldInfo_PickupItem> All_PickupActorsInfo;

	UPROPERTY(VisibleAnywhere)
	TArray<FWorldInfo_Containers> All_ContainerActorsInfo;

	UPROPERTY(VisibleAnywhere)
	bool bIsFirstTimeLoadingMap = true;	
};
