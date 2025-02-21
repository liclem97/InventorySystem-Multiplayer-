// Copyright by liclem97.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "InventoryStructure.h"
#include "InventoryGameModeBase.generated.h"

class UInventoryGameInstance;
class UInventorySaveGame;
class APickup;
class AContainer;

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API AInventoryGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	void Remove_SavedPickupActor(APickup* InPickup);
	void Add_SavedPickupActor(APickup* InPickup);
	void Add_ItemToContainer(const TArray<FInventoryContents>& InContents, AContainer* InContainer);
	void Remove_ItemFromContainer(const TArray<FInventoryContents>& InContents, AContainer* InContainer);

	/** Getter */
	FORCEINLINE UDataTable* GetItemDataTable() const { return ItemDataTable; }
	/** End Getter */

protected:
	virtual void BeginPlay() override;
	void Setup_PickupActors(const TArray<FWorldInfo_PickupItem>& All_PickupActorsInfo, bool bIsFirstTimeLoadingMap);
	void Setup_ContainerActors(const TArray<FWorldInfo_Containers>& All_ContainerActorsInfo, bool bIsFirstTimeLoadingMap);
	void UpdateContainerInfo(AContainer* InContainer, int32 ContainerIndex);

private:
	UPROPERTY()
	UInventoryGameInstance* InventoryGameInstance;

	UPROPERTY()
	UInventorySaveGame* InventorySaveGame;

	UPROPERTY()
	TArray<APickup*> All_SavedPickupActors;

	UPROPERTY()
	TArray<FWorldInfo_PickupItem> All_SavedPickupActorsInfo;

	UPROPERTY()
	TArray<AContainer*> All_SavedContainerActors;

	UPROPERTY()
	TArray<FWorldInfo_Containers> All_SavedContainerActorsInfo;

	UPROPERTY(EditDefaultsOnly, Category = "DataTable")
	UDataTable* ItemDataTable;

	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* ContainerMesh;
};
