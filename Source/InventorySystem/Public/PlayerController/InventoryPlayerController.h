// Copyright by liclem97.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InventoryStructure.h"
#include "InventoryPlayerController.generated.h"

class AInventoryCharacter;
class UPlayerInventorySaveGame;
class USaveGame;

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API AInventoryPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(Client, Reliable)
	void SaveInventoryToSaveGame(const TArray<FInventoryContents>& InPlayerInventory);
	
protected:
	virtual void OnPossess(APawn* aPawn) override;
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void LoadInventorySaveGame();	

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	AInventoryCharacter* InventoryCharacter;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FString Inventory_SlotName;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USaveGame> PlayerInventorySaveGameClass;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UPlayerInventorySaveGame* PlayerInventorySaveGame;
};
