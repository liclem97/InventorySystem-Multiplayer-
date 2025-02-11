// Copyright by liclem97.


#include "PlayerController/InventoryPlayerController.h"

#include "Actor/Container.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Character/InventoryCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/PlayerInventorySaveGame.h"
#include "Widgets/IngameWidget.h"
#include "Widgets/PlayerInventory.h"

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
		UI_ShowIngameHUD();
		if (HasAuthority())
		{
			Inventory_SlotName = "InventorySlot_00";
			LoadInventorySaveGame();
			InventoryCharacter->Server_LoadInventoryFromSaveGame(PlayerInventorySaveGame->GetPlayerInventory());
		}		
	}
}

void AInventoryPlayerController::UI_ShowIngameHUD()
{	
	if (!IsValid(IngameWidget))
	{
		if (!IngameWidgetClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerController: IngameWidgetClass is nullptr."));
			return;
		}
		IngameWidget = CreateWidget<UIngameWidget>(this, IngameWidgetClass);
	}

	if (!IsValid(IngameWidget))
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController: Failed to create IngameWidget."));
		return;
	}

	IngameWidget->AddToViewport();
}

void AInventoryPlayerController::HUD_UpdateInventoryGrid_Implementation(const TArray<FInventoryContents>& InContents, bool bIsPlayerInventory, bool bIsWorldInventory)
{
	if (IsValid(PlayerInventoryWidget) && PlayerInventoryWidget->IsInViewport())
	{
		PlayerInventoryWidget->Setup_InventoryGrid(InContents, bIsPlayerInventory, bIsWorldInventory);
	}
}

void AInventoryPlayerController::UI_ShowInventoryMenu()
{
	if (!IsValid(PlayerInventoryWidget))
	{
		if (!PlayerInventoryClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerController: PlayerInventoryClass is nullptr."));
			return;
		}
		PlayerInventoryWidget = CreateWidget<UPlayerInventory>(this, PlayerInventoryClass);
	}

	if (!IsValid(PlayerInventoryWidget))
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController: Failed to create PlayerInventoryWidget."));
		return;
	}

	PlayerInventoryWidget->AddToViewport();
	SetShowMouseCursor(true);
	UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this, PlayerInventoryWidget, EMouseLockMode::DoNotLock, false);
	PlayerInventoryWidget->Setup_InventoryGrid(InventoryCharacter->GetPlayerInventory(), true, false);
	if (IsValid(IngameWidget) && IngameWidget->IsInViewport())
	{
		IngameWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AInventoryPlayerController::LeaveInventoryMenu()
{	
	PlayerInventoryWidget = nullptr;
	SetShowMouseCursor(false);
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
	if (IsValid(IngameWidget))
	{
		IngameWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		UI_ShowIngameHUD();
	}
	InventoryCharacter->Server_CloseContainer();
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

void AInventoryPlayerController::ContainerOpened(AContainer* InContainer)
{
	if (!IsValid(PlayerInventoryWidget) || !PlayerInventoryWidget->IsInViewport())
	{
		UI_ShowInventoryMenu();
	}

	if (IsValid(PlayerInventoryWidget))
	{
		PlayerInventoryWidget->Set_ContainerWidgetVisibility(true);
		PlayerInventoryWidget->Setup_InventoryGrid(InContainer->GetContainerContents(), false, true);
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
