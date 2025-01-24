// Copyright by liclem97.


#include "GameInstance/InventoryGameInstance.h"

#include "Kismet/GameplayStatics.h"
#include "SaveGame/InventorySaveGame.h"

UInventorySaveGame* UInventoryGameInstance::Initialize_GameWorldSave(FString SaveSlotName)
{
	GameWorldSaveSlot = SaveSlotName;

	if (UGameplayStatics::DoesSaveGameExist(GameWorldSaveSlot, 0))
	{	
		SaveGameWorld = Cast<UInventorySaveGame>(UGameplayStatics::LoadGameFromSlot(GameWorldSaveSlot, 0));		
		return SaveGameWorld;
	}
	else
	{	
		if (SaveGameClass)
		{
			SaveGameWorld = Cast<UInventorySaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameClass));
			UGameplayStatics::SaveGameToSlot(SaveGameWorld, GameWorldSaveSlot, 0);
			return SaveGameWorld;
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString("InventoryGameInstance: SaveGameClass is not valid."));
			return nullptr;
		}
	}
}

void UInventoryGameInstance::Update_SavedPickupActors(TArray<FWorldInfo_PickupItem> All_PickupActorsInfo)
{
	if (!IsValid(SaveGameWorld)) return;

	SaveGameWorld->SetPickupActorsInfo(All_PickupActorsInfo);
	UGameplayStatics::SaveGameToSlot(SaveGameWorld, GameWorldSaveSlot, 0);
	return;
}
