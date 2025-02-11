// Copyright by liclem97.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InventoryStructure.h"
#include "InventoryPlayerController.generated.h"

class AInventoryCharacter;
class UPlayerInventorySaveGame;
class USaveGame;
class UIngameWidget;
class UPlayerInventory;
class AContainer;

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

	/** Getter */
	FORCEINLINE UPlayerInventory* GetPlayerInventoryWidget() const { return PlayerInventoryWidget; }
	/** End Getter*/

	void UI_ShowInventoryMenu();
	void LeaveInventoryMenu();

	UFUNCTION(Client, Reliable)
	void HUD_UpdateInventoryGrid(const TArray<FInventoryContents>& InContents, bool bIsPlayerInventory, bool bIsWorldInventory);
	
	void ContainerOpened(AContainer* InContainer);

protected:
	virtual void OnPossess(APawn* aPawn) override;
	virtual void BeginPlay() override;

	void UI_ShowIngameHUD();
	
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

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UIngameWidget> IngameWidgetClass;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UIngameWidget* IngameWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPlayerInventory> PlayerInventoryClass;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UPlayerInventory* PlayerInventoryWidget;
};
