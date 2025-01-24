// Copyright by liclem97.


#include "GameMode/InventoryGameModeBase.h"

#include "Actor/Pickup.h"
#include "GameInstance/InventoryGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/InventorySaveGame.h"

void AInventoryGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	InventoryGameInstance = Cast<UInventoryGameInstance>(GetGameInstance());
	if (InventoryGameInstance)
	{		
		InventorySaveGame = InventoryGameInstance->Initialize_GameWorldSave(UGameplayStatics::GetCurrentLevelName(this));
		if (IsValid(InventorySaveGame))
		{
			Setup_PickupActors(InventorySaveGame->GetAllPickupActorsInfo(), InventorySaveGame->GetIsFirstTimeLoadingMap());
			if (InventorySaveGame->GetIsFirstTimeLoadingMap())
			{
				InventoryGameInstance->GetSaveGameWorld()->SetIsFirstTimeLoadingMap(false);
				UGameplayStatics::SaveGameToSlot(InventoryGameInstance->GetSaveGameWorld(), InventoryGameInstance->GetGameWorldSaveSlot(), 0);
			}
		}
	}
}

void AInventoryGameModeBase::Setup_PickupActors(const TArray<FWorldInfo_PickupItem>& All_PickupActorsInfo, bool bIsFirstTimeLoadingMap)
{
	if (bIsFirstTimeLoadingMap)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString("No Saved PICKUP data, saving world info to SaveGame"));
		TArray<AActor*> OutActors;
		UGameplayStatics::GetAllActorsOfClass(this, APickup::StaticClass(), OutActors);
		for (AActor* Actor : OutActors)
		{
			All_SavedPickupActors.Add(static_cast<APickup*>(Actor));
		}

		for (int32 ArrayIndex = 0; ArrayIndex < All_SavedPickupActors.Num(); ArrayIndex++)
		{	
			FWorldInfo_PickupItem Item;
			Item.ItemRowName = All_SavedPickupActors[ArrayIndex]->GetItemRowName();
			Item.ItemContents = All_SavedPickupActors[ArrayIndex]->GetItemContents();
			Item.WorldTransform = All_SavedPickupActors[ArrayIndex]->GetActorTransform();

			All_SavedPickupActorsInfo[ArrayIndex] = Item;
		}
		InventoryGameInstance->Update_SavedPickupActors(All_SavedPickupActorsInfo);
		return;
	}
	else
	{	
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString("Saved PICKUP data found, loading world contents"));
		TArray<AActor*> OutActors;
		UGameplayStatics::GetAllActorsOfClass(this, APickup::StaticClass(), OutActors);
		for (AActor* Actor : OutActors)
		{
			Actor->Destroy();
		}		

		FTransform SpawnTransform;
		APickup* SpawnedPickup;

		for (FWorldInfo_PickupItem PickupItem : All_PickupActorsInfo)
		{
			SpawnTransform = PickupItem.WorldTransform;
			SpawnedPickup = Cast<APickup>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, APickup::StaticClass(), SpawnTransform));

			if (SpawnedPickup)
			{
				SpawnedPickup->SetItemRowName(PickupItem.ItemRowName);
				SpawnedPickup->SetItemContents(PickupItem.ItemContents);

				UGameplayStatics::FinishSpawningActor(SpawnedPickup, SpawnTransform);

				int32 Index = All_SavedPickupActors.Add(SpawnedPickup);
				All_SavedPickupActorsInfo[Index] = PickupItem;
			}
		}
		return;
	}
}
