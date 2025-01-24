// Copyright by liclem97.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "InventoryStructure.h"
#include "InventoryGameInstance.generated.h"

class USaveGame;
class UInventorySaveGame;

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UInventoryGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
protected:
	UInventorySaveGame* Initialize_GameWorldSave(FString SaveSlotName);
	void Update_SavedPickupActors(TArray<FWorldInfo_PickupItem> All_PickupActorsInfo);

private:
	FString GameWorldSaveSlot;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USaveGame> SaveGameClass;

	UPROPERTY()
	UInventorySaveGame* SaveGameWorld;
};
