// Copyright by liclem97.


#include "PlayerController/InventoryPlayerController.h"

#include "Character/InventoryCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/PlayerInventorySaveGame.h"

void AInventoryPlayerController::OnPossess(APawn* aPawn)
{	
	Super::OnPossess(aPawn);

	InventoryCharacter = Cast<AInventoryCharacter>(aPawn);
}

void AInventoryPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController() && HasAuthority())
	{
		Inventory_SlotName = "InventorySlot_00";
		LoadInventorySaveGame();
		InventoryCharacter->Server_LoadInventoryFromSaveGame(PlayerInventorySaveGame->GetPlayerInventory());
	}
}

void AInventoryPlayerController::LoadInventorySaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(Inventory_SlotName, 0))
	{
		PlayerInventorySaveGame = Cast<UPlayerInventorySaveGame>(UGameplayStatics::LoadGameFromSlot(Inventory_SlotName, 0));
	}
	else
	{
		PlayerInventorySaveGame = Cast<UPlayerInventorySaveGame>(UGameplayStatics::CreateSaveGameObject(UPlayerInventorySaveGame::StaticClass()));
		UGameplayStatics::SaveGameToSlot(PlayerInventorySaveGame, Inventory_SlotName, 0);
	}
}

void AInventoryPlayerController::SaveInventoryToSaveGame_Implementation(const TArray<FInventoryContents>& InPlayerInventory)
{
	if (PlayerInventorySaveGame == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("InventoryPlayerController: (SaveInventoryToSaveGame) PlayerInventorySaveGame is nullptr."));
		return;
	}

	PlayerInventorySaveGame->SetPlayerInventory(InPlayerInventory);
	UGameplayStatics::SaveGameToSlot(PlayerInventorySaveGame, Inventory_SlotName, 0);
}
