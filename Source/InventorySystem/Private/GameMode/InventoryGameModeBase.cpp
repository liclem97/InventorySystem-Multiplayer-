// Copyright by liclem97.


#include "GameMode/InventoryGameModeBase.h"

#include "Actor/Container.h"
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
			Setup_ContainerActors(InventorySaveGame->GetAllContainerActorsInfo(), InventorySaveGame->GetIsFirstTimeLoadingMap());
			if (InventorySaveGame->GetIsFirstTimeLoadingMap())
			{
				InventoryGameInstance->GetSaveGameWorld()->SetIsFirstTimeLoadingMap(false);
				UGameplayStatics::SaveGameToSlot(InventoryGameInstance->GetSaveGameWorld(), InventoryGameInstance->GetGameWorldSaveSlot(), 0);
			}
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString("GameMode: InventoryGameInstance is not valid."));
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
	}
}

void AInventoryGameModeBase::Setup_ContainerActors(const TArray<FWorldInfo_Containers>& All_ContainerActorsInfo, bool bIsFirstTimeLoadingMap)
{
	if (bIsFirstTimeLoadingMap)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString("No Saved CONTAINER data, saving world info to SaveGame"));
		TArray<AActor*> OutActors;
		UGameplayStatics::GetAllActorsOfClass(this, AContainer::StaticClass(), OutActors);
		for (AActor* InActor : OutActors)
		{
			All_SavedContainerActors.Add(Cast<AContainer>(InActor));
		}

		All_SavedContainerActorsInfo.SetNum(All_SavedContainerActors.Num());
		for (int32 ArrayIndex = 0; ArrayIndex < All_SavedContainerActors.Num(); ArrayIndex++)
		{
			if (All_SavedContainerActors[ArrayIndex])
			{
				All_SavedContainerActorsInfo[ArrayIndex].ContainerRowName = All_SavedContainerActors[ArrayIndex]->GetContainerRowName();
				All_SavedContainerActorsInfo[ArrayIndex].ContainerContents = All_SavedContainerActors[ArrayIndex]->GetContainerContents();
				All_SavedContainerActorsInfo[ArrayIndex].WorldTransform = All_SavedContainerActors[ArrayIndex]->GetActorTransform();
			}
		}
		InventoryGameInstance->Update_SavedContainerActors(All_SavedContainerActorsInfo);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString("Saved CONTAINER data found, loading world contents"));
		TArray<AActor*> OutActors;
		UGameplayStatics::GetAllActorsOfClass(this, AContainer::StaticClass(), OutActors);
		for (AActor* InActor : OutActors)
		{
			InActor->Destroy();
		}

		FTransform SpawnTransform;
		AContainer* SpawnedContainer;

		for (FWorldInfo_Containers InContainer : All_ContainerActorsInfo)
		{
			SpawnTransform = InContainer.WorldTransform;
			SpawnedContainer = Cast<AContainer>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, AContainer::StaticClass(), SpawnTransform));
			if (SpawnedContainer)
			{
				//SpawnedPickup->SetItemDataTable(ItemDataTable);
				SpawnedContainer->SetContainerRowName(InContainer.ContainerRowName);
				SpawnedContainer->SetContainerContents(InContainer.ContainerContents);

				UGameplayStatics::FinishSpawningActor(SpawnedContainer, SpawnTransform);

				All_SavedContainerActors.Add(SpawnedContainer);
				All_SavedContainerActorsInfo.Add(InContainer);
			}
		}
	}
}

void AInventoryGameModeBase::Add_ItemToContainer(const TArray<FInventoryContents>& InContents, AContainer* InContainer)
{	
	int32 Local_ContainerWorldIndex = All_SavedContainerActors.Find(InContainer);
	if (Local_ContainerWorldIndex == INDEX_NONE)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString("ERROR: Container not found in world! - GameMode->Add_ItemToContainer."));
		return;
	}

	if (Local_ContainerWorldIndex >= 0)
	{
		for (const FInventoryContents& ArrayContents : InContents)
		{
			FName Local_ItemToAdd = ArrayContents.ItemRowName;
			int32 Local_AmountToAdd = ArrayContents.ItemAmount;
			bool Local_bIsNewItem = true;
			int32 Local_CurrentAmountInContainer = 0;

			const TArray<FInventoryContents>& ContainerContents = InContainer->GetContainerContents();
			int32 Index = 0;

			for (Index = 0; Index < ContainerContents.Num(); ++Index)
			{
				if (ContainerContents[Index].ItemRowName == Local_ItemToAdd)
				{
					Local_bIsNewItem = false;
					Local_CurrentAmountInContainer = ContainerContents[Index].ItemAmount;
					break;
				}
			}

			FInventoryContents NewItem;
			NewItem.ItemRowName = Local_ItemToAdd;

			// TODO: ContainerContents를 직접 참조하고있기에 나중에 바꿔야함.
			if (Local_bIsNewItem)
			{
				NewItem.ItemAmount = Local_AmountToAdd;
				InContainer->ContainerContents.Add(NewItem);
			}
			else
			{
				NewItem.ItemAmount = Local_AmountToAdd + Local_CurrentAmountInContainer;
				InContainer->ContainerContents[Index] = NewItem;
			}
		}
		All_SavedContainerActors[Local_ContainerWorldIndex] = InContainer;
		All_SavedContainerActorsInfo[Local_ContainerWorldIndex].ContainerContents = InContainer->ContainerContents;
		All_SavedContainerActorsInfo[Local_ContainerWorldIndex].WorldTransform = InContainer->GetActorTransform();
		All_SavedContainerActorsInfo[Local_ContainerWorldIndex].ContainerRowName = InContainer->GetContainerRowName();
		InventoryGameInstance->Update_SavedContainerActors(All_SavedContainerActorsInfo);
		InContainer->ContainerContentsChanged();
	}	
}

void AInventoryGameModeBase::Remove_ItemFromContainer(const TArray<FInventoryContents>& InContents, AContainer* InContainer)
{	
	int32 Local_ContainerWorldIndex = All_SavedContainerActors.Find(InContainer);
	if (Local_ContainerWorldIndex == INDEX_NONE)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString("ERROR: Container not found in world! - GameMode->Add_ItemToContainer."));
		return;
	}

	if (Local_ContainerWorldIndex >= 0)
	{
		for (FInventoryContents ArrayContents : InContents)
		{
			FName Local_ItemToRemove = ArrayContents.ItemRowName;
			int32 Local_AmountToRemove = ArrayContents.ItemAmount;
			bool Local_bIsItemFound = false;
			int32 Local_CurrentAmountInContainer = 0;

			const TArray<FInventoryContents>& ContainerContents = InContainer->GetContainerContents();
			int32 Index = 0;

			for (Index = 0; Index < ContainerContents.Num(); ++Index)
			{
				if (ContainerContents[Index].ItemRowName == Local_ItemToRemove)
				{
					Local_bIsItemFound = true;
					Local_CurrentAmountInContainer = ContainerContents[Index].ItemAmount;
					break;
				}
			}

			// TODO: ContainerContents를 직접 참조하고있기에 나중에 바꿔야함.
			if (Local_bIsItemFound)
			{
				if (int32 Local_NewAmountInContainer = Local_CurrentAmountInContainer - Local_AmountToRemove > 0)
				{
					InContainer->ContainerContents[Index].ItemRowName = Local_ItemToRemove;
					InContainer->ContainerContents[Index].ItemAmount = Local_NewAmountInContainer;
				}
				else
				{
					InContainer->ContainerContents.RemoveAt(Index);
				}
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString("ERROR: Item to remove not found! - GameMode->Remove_ItemFromContainer."));
			}
		}
		All_SavedContainerActors[Local_ContainerWorldIndex] = InContainer;
		All_SavedContainerActorsInfo[Local_ContainerWorldIndex].ContainerContents = InContainer->ContainerContents;
		All_SavedContainerActorsInfo[Local_ContainerWorldIndex].WorldTransform = InContainer->GetActorTransform();
		All_SavedContainerActorsInfo[Local_ContainerWorldIndex].ContainerRowName = InContainer->GetContainerRowName();
		InventoryGameInstance->Update_SavedContainerActors(All_SavedContainerActorsInfo);
		InContainer->ContainerContentsChanged();
	}	
}

void AInventoryGameModeBase::Add_SavedPickupActor(APickup* InPickup)
{	
	if (!InPickup)
	{
		GEngine->AddOnScreenDebugMessage(-1, 6.f, FColor::Red, FString("Invalid Pickup"));
		return;
	}
	int32 NewIndex = All_SavedPickupActors.AddUnique(InPickup);
	if (NewIndex != INDEX_NONE && InventoryGameInstance)
	{
		GEngine->AddOnScreenDebugMessage(-1, 6.f, FColor::Green, FString("PICKUP ADDED TO SAVEGAME."));

		// Ensure All_SavedPickupActorsInfo has enough space
		if (All_SavedPickupActorsInfo.Num() <= NewIndex)
		{
			All_SavedPickupActorsInfo.SetNum(NewIndex + 1);
		}

		All_SavedPickupActorsInfo[NewIndex].ItemRowName = InPickup->GetItemRowName();
		All_SavedPickupActorsInfo[NewIndex].ItemContents = InPickup->GetItemContents();
		All_SavedPickupActorsInfo[NewIndex].WorldTransform = InPickup->GetActorTransform();

		InventoryGameInstance->Update_SavedPickupActors(All_SavedPickupActorsInfo);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 6.f, FColor::Red, FString("ADDING PICKUP TO SAVEGAME FAILED."));
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
