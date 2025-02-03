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
		for (AActor* InActor : OutActors)
		{
			All_SavedPickupActors.Add(Cast<APickup>(InActor));
		}

		All_SavedPickupActorsInfo.SetNum(All_SavedPickupActors.Num());
		for (int32 ArrayIndex = 0; ArrayIndex < All_SavedPickupActors.Num(); ArrayIndex++)
		{	
			if (All_SavedPickupActors[ArrayIndex])
			{
				All_SavedPickupActorsInfo[ArrayIndex].ItemRowName = All_SavedPickupActors[ArrayIndex]->GetItemRowName();
				All_SavedPickupActorsInfo[ArrayIndex].ItemContents = All_SavedPickupActors[ArrayIndex]->GetItemContents();
				All_SavedPickupActorsInfo[ArrayIndex].WorldTransform = All_SavedPickupActors[ArrayIndex]->GetActorTransform();
			}			
		}
		InventoryGameInstance->Update_SavedPickupActors(All_SavedPickupActorsInfo);
		return;
	}
	else
	{	
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString("Saved PICKUP data found, loading world contents"));
		TArray<AActor*> OutActors;
		UGameplayStatics::GetAllActorsOfClass(this, APickup::StaticClass(), OutActors);
		for (AActor* InActor : OutActors)
		{
			InActor->Destroy();
		}		

		FTransform SpawnTransform;
		APickup* SpawnedPickup;

		for (FWorldInfo_PickupItem PickupItem : All_PickupActorsInfo)
		{
			SpawnTransform = PickupItem.WorldTransform;
			SpawnedPickup = Cast<APickup>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, APickup::StaticClass(), SpawnTransform));
			if (SpawnedPickup)
			{
				SpawnedPickup->SetItemDataTable(ItemDataTable);
				SpawnedPickup->SetItemRowName(PickupItem.ItemRowName);
				SpawnedPickup->SetItemContents(PickupItem.ItemContents);

				UGameplayStatics::FinishSpawningActor(SpawnedPickup, SpawnTransform);
				
				All_SavedPickupActors.Add(SpawnedPickup);
				All_SavedPickupActorsInfo.Add(PickupItem);		
			}
		}
		return;
	}
}

void AInventoryGameModeBase::Add_SavedPickupActor(APickup* InPickup)
{	
	int32 NewIndex = All_SavedPickupActors.AddUnique(InPickup);
	if (NewIndex >= 0 && InventoryGameInstance)
	{
		GEngine->AddOnScreenDebugMessage(-1, 6.f, FColor::Green, FString("PICKUP ADDED TO SAVEGAME."));

		All_SavedPickupActorsInfo[NewIndex].ItemRowName = InPickup->GetItemRowName();
		All_SavedPickupActorsInfo[NewIndex].ItemContents = InPickup->GetItemContents();
		All_SavedPickupActorsInfo[NewIndex].WorldTransform = InPickup->GetActorTransform();

		InventoryGameInstance->Update_SavedPickupActors(All_SavedPickupActorsInfo);
		return;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 6.f, FColor::Red, FString("ADDING PICKUP TO SAVEGAME FAILED."));
		return;
	}
}

void AInventoryGameModeBase::Remove_SavedPickupActor(APickup* InPickup)
{
	int32 IndexToRemove = All_SavedPickupActors.Find(InPickup);
	if (IndexToRemove >= 0 && InventoryGameInstance)
	{
		GEngine->AddOnScreenDebugMessage(-1, 6.f, FColor::Green, FString("PICKUP REMOVED FROM SAVEGAME."));

		All_SavedPickupActorsInfo.RemoveAt(IndexToRemove);
		All_SavedPickupActors.RemoveAt(IndexToRemove);

		InventoryGameInstance->Update_SavedPickupActors(All_SavedPickupActorsInfo);
		return;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 6.f, FColor::Red, FString("PICKUP REMOVE FROM SAVEGAME FAILED."));
		return;
	}
}
