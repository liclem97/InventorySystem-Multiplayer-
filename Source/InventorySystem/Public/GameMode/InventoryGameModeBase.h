// Copyright by liclem97.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "InventoryStructure.h"
#include "InventoryGameModeBase.generated.h"

class UInventoryGameInstance;
class UInventorySaveGame;
class APickup;

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API AInventoryGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	void Setup_PickupActors(const TArray<FWorldInfo_PickupItem>& All_PickupActorsInfo, bool bIsFirstTimeLoadingMap);

private:
	UPROPERTY()
	UInventoryGameInstance* InventoryGameInstance;

	UPROPERTY()
	UInventorySaveGame* InventorySaveGame;

	UPROPERTY()
	TArray<APickup*> All_SavedPickupActors;

	UPROPERTY()
	TArray<FWorldInfo_PickupItem> All_SavedPickupActorsInfo;
};
