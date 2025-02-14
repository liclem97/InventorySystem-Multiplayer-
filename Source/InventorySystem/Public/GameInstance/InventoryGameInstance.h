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
	
public:
	UInventorySaveGame* Initialize_GameWorldSave(FString SaveSlotName);
	void Update_SavedPickupActors(const TArray<FWorldInfo_PickupItem>& All_PickupActorsInfo);
	void Update_SavedContainerActors(const TArray<FWorldInfo_Containers>& All_ContainerActorsInfo);

	/** Getter */
	FORCEINLINE UInventorySaveGame* GetSaveGameWorld() const { return SaveGameWorld; }
	FORCEINLINE FString GetGameWorldSaveSlot() const { return GameWorldSaveSlot; }
	/** End Getter*/

protected:

private:
	FString GameWorldSaveSlot;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USaveGame> SaveGameClass;

	UPROPERTY()
	UInventorySaveGame* SaveGameWorld;
};
