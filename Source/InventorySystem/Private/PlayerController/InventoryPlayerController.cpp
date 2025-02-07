// Copyright by liclem97.


#include "PlayerController/InventoryPlayerController.h"

#include "Character/InventoryCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/PlayerInventorySaveGame.h"
#include "Widgets/IngameWidget.h"

void AInventoryPlayerController::OnPossess(APawn* aPawn)
{	
	Super::OnPossess(aPawn);

	InventoryCharacter = Cast<AInventoryCharacter>(aPawn);
}

void AInventoryPlayerController::BeginPlay()
{
	Super::BeginPlay();

	InventoryCharacter = InventoryCharacter == nullptr ? Cast<AInventoryCharacter>(GetPawn()) : InventoryCharacter;

	if (IsLocalPlayerController())
	{	
		UIShowIngameHUD();
		if (HasAuthority())
		{
			Inventory_SlotName = "InventorySlot_00";
			LoadInventorySaveGame();
			InventoryCharacter->Server_LoadInventoryFromSaveGame(PlayerInventorySaveGame->GetPlayerInventory());
		}		
	}
}

void AInventoryPlayerController::UIShowIngameHUD()
{	
	if (IsValid(IngameWidget))
	{
		IngameWidget->AddToViewport();
	}
	else
	{	
		if (IngameWidgetClass)
		{
			IngameWidget = CreateWidget<UIngameWidget>(this, IngameWidgetClass);
			IngameWidget->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerController: IngameWidgetClass is nullptr."));
			return;
		}
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
		if (PlayerInventorySaveGameClass)
		{
			PlayerInventorySaveGame = Cast<UPlayerInventorySaveGame>(UGameplayStatics::CreateSaveGameObject(PlayerInventorySaveGameClass));
			UGameplayStatics::SaveGameToSlot(PlayerInventorySaveGame, Inventory_SlotName, 0);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("PlayerController: Please Set PlayerInventory Savegame Class."));
		}		
	}
}

void AInventoryPlayerController::SaveInventoryToSaveGame_Implementation(const TArray<FInventoryContents>& InPlayerInventory)
{
	if (PlayerInventorySaveGame)
	{	
		PlayerInventorySaveGame->SetPlayerInventory(InPlayerInventory);
		UGameplayStatics::SaveGameToSlot(PlayerInventorySaveGame, Inventory_SlotName, 0);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("InventoryPlayerController: (SaveInventoryToSaveGame) PlayerInventorySaveGame is not valid."));
		return;
	}
}
