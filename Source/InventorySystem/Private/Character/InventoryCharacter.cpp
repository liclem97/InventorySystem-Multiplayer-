// Copyright by liclem97.


#include "Character/InventoryCharacter.h"

#include "Actor/Container.h"
#include "Actor/Pickup.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "DragDrop/DragDrop.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameMode/InventoryGameModeBase.h"
#include "InputActionValue.h"
#include "Interface/InteractableInterface.h"
#include "InventorySystem/InventorySystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "PlayerController/InventoryPlayerController.h"
#include "utility"
#include "Widgets/PlayerInventory.h"

AInventoryCharacter::AInventoryCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	InteractableDetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
	InteractableDetectionSphere->SetupAttachment(RootComponent);
	InteractableDetectionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractableDetectionSphere->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	InteractableDetectionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	InteractableDetectionSphere->SetCollisionResponseToChannel(ECC_Pickup, ECR_Overlap);
	InteractableDetectionSphere->SetGenerateOverlapEvents(true);
	InteractableDetectionSphere->SetSphereRadius(600.f);

	GetMesh()->SetGenerateOverlapEvents(true);


	MouseSensitivity = 0.6f;
}

void AInventoryCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocallyControlled())
	{
		SetupInputMapping();
	}

	if (!IsLocallyControlled())
	{	
		InteractableDetectionSphere->DestroyComponent();
	}

	InteractableDetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &AInventoryCharacter::OnInteractableDetectionOverlapBegin);
	InteractableDetectionSphere->OnComponentEndOverlap.AddDynamic(this, &AInventoryCharacter::OnInteractableDetectionOverlapEnd);
	GetMesh()->OnComponentBeginOverlap.AddDynamic(this, &AInventoryCharacter::OnMeshOverlapBegin);
	GetMesh()->OnComponentEndOverlap.AddDynamic(this, &AInventoryCharacter::OnMeshOverlapEnd);
}

void AInventoryCharacter::SetupInputMapping()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	else
	{
		// 컨트롤러가 아직 설정되지 않았다면 잠시 후에 다시 시도
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AInventoryCharacter::SetupInputMapping, 0.1f, false);
	}
}

void AInventoryCharacter::OnInteractableDetectionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	if (OtherActor->Implements<UInteractableInterface>())
	{
		IInteractableInterface::Execute_SetItemNameVisibility(OtherActor, true);
	}
}

void AInventoryCharacter::OnInteractableDetectionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{	
	if (OtherActor->Implements<UInteractableInterface>())
	{
		IInteractableInterface::Execute_SetItemNameVisibility(OtherActor, false);
	}
}

void AInventoryCharacter::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority()) return;

	if (OtherActor->Implements<UInteractableInterface>())
	{
		InteractableActor = OtherActor;
	}
}

void AInventoryCharacter::OnMeshOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{	
	if (!HasAuthority()) return;

	if (OtherActor == InteractableActor)
	{
		InteractableActor = nullptr;
		if (IsValid(OpenedContainer))
		{
			LeaveContainerTrigger();
		}
	}
}

void AInventoryCharacter::Server_ItemSwap_Implementation(UDragDrop* InDragDrop, int32 InInventoryIndex)
{
	InventoryItemSwap(InDragDrop, InInventoryIndex);
}

void AInventoryCharacter::InventoryItemSwap(UDragDrop* InDragDrop, int32 InIndex)
{	
	if (PlayerInventory.IsValidIndex(InDragDrop->GetCurrentIndex()) && PlayerInventory.IsValidIndex(InIndex))
	{	
		PlayerInventory[InDragDrop->GetCurrentIndex()] = PlayerInventory[InIndex];
		PlayerInventory[InIndex].ItemRowName = InDragDrop->GetItemRowName();
		PlayerInventory[InIndex].ItemAmount = InDragDrop->GetItemAmount();

		SaveItemAndUpdateHUD(nullptr);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("Slot Item Swapped from %d to %d"), InDragDrop->GetCurrentIndex(), InIndex));
	}
}

void AInventoryCharacter::Server_RemoveItemFromContainer_Implementation(const TArray<FInventoryContents>& InContents)
{
	if (InventoryGameMode && OpenedContainer)
	{
		InventoryGameMode->Remove_ItemFromContainer(InContents, OpenedContainer);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Character: GameMode or OpenedContainer is nullptr. -Server_RemoveItemFromContainer -"));
	}
}


void AInventoryCharacter::Server_RemoveDraggedItemFromContainer_Implementation(const TArray<FInventoryContents>& InContents, bool bDropIntoWorld, int32 InventoryIndex)
{	
	if (InventoryGameMode && OpenedContainer)
	{
		InventoryGameMode->Remove_DraggedItemFromContainer(InContents, OpenedContainer, bDropIntoWorld, InventoryIndex);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Character: GameMode or OpenedContainer is nullptr. -Server_RemoveDraggedItemFromContainer -"));
	}
}

void AInventoryCharacter::Server_AddItemToContainer_Implementation(const TArray<FInventoryContents>& InContents)
{
	if (InventoryGameMode && OpenedContainer)
	{	
		InventoryGameMode->Add_ItemToContainer(InContents, OpenedContainer);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Character: GameMode or OpenedContainer is nullptr. - Server_AddItemToContainer -"));
	}
}

void AInventoryCharacter::LeaveContainerTrigger_Implementation()
{
	if (InventoryPlayerController)
	{
		InventoryPlayerController->GetPlayerInventoryWidget()->RemoveFromParent();
		InventoryPlayerController->LeaveInventoryMenu();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Character: InventoryPlayerController is nullptr."));
		return;
	}
}

void AInventoryCharacter::Server_CloseContainer_Implementation()
{
	if (IsValid(OpenedContainer))
	{
		// 먼저 배열에 해당 플레이어가 있는지 확인
		if (OpenedContainer->GetInteractionPlayers().Contains(this))
		{
			// 플레이어가 존재하면 제거
			OpenedContainer->GetInteractionPlayers().Remove(this);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString("Player Removed from InteractingPlayers"));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("Player NOT Found in InteractingPlayers"));
		}
	}
	OpenedContainer = nullptr;
}

void AInventoryCharacter::OpenContainer_Implementation(AContainer* InContainer)
{
	if (InventoryPlayerController && InContainer)
	{
		InventoryPlayerController->ContainerOpened(InContainer);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Character: InventoryPlayerController or InContainer is nullptr."));
		return;
	}
}

void AInventoryCharacter::Server_LoadInventoryFromSaveGame_Implementation(const TArray<FInventoryContents>& InPlayerInventory)
{
	PlayerInventory = InPlayerInventory;
	InventorySize = 21;

	// 슬롯을 "Empty"로 채움.
	for (int32 i = PlayerInventory.Num(); i < InventorySize; i++)
	{
		FInventoryContents EmptySlot;
		EmptySlot.ItemRowName = FName("Empty");
		EmptySlot.ItemAmount = 0;
		
		PlayerInventory.Add(EmptySlot);
	}
}

void AInventoryCharacter::Server_RemoveItemFromInventory_Implementation(const TArray<FInventoryContents>& ItemInfo, bool bDropIntoWorld, int32 InventoryIndex)
{
	RemoveItemFromInventory(ItemInfo, bDropIntoWorld, InventoryIndex);
}

void AInventoryCharacter::RemoveItemFromInventory(const TArray<FInventoryContents>& ItemInfo, bool bDropIntoWorld, int32 InventoryIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Remove from Inventory Index: %d"), InventoryIndex);

	FVector DropLocation = GetActorLocation();

	if (bDropIntoWorld)
	{
		FVector Start = GetActorLocation();
		FVector End = Start + (GetActorUpVector() * -500.f);
		FHitResult HitResult;

		bool bHit = UKismetSystemLibrary::LineTraceSingle(
			this,
			Start,
			End,
			UEngineTypes::ConvertToTraceType(ECC_Visibility),
			false,
			TArray<AActor*>(),
			EDrawDebugTrace::ForDuration,
			HitResult,
			true
		);

		if (bHit)
		{
			DropLocation = HitResult.Location;
		}

		FTransform SpawnTransform(FRotator(0.f, GetActorRotation().Yaw, 0.f), DropLocation);

		APickup* SpawnedPickup = Cast<APickup>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, APickup::StaticClass(), SpawnTransform));
		if (SpawnedPickup)
		{
			SpawnedPickup->SetItemDataTable(InventoryGameMode->GetItemDataTable());
			SpawnedPickup->SetItemRowName(ItemInfo[0].ItemRowName);
			SpawnedPickup->SetItemContents(ItemInfo);

			UGameplayStatics::FinishSpawningActor(SpawnedPickup, SpawnTransform);

			if (InventoryGameMode)
			{
				InventoryGameMode->Add_SavedPickupActor(SpawnedPickup);
			}
		}
	}

	if (PlayerInventory[InventoryIndex].ItemAmount > 0)
	{
		PlayerInventory[InventoryIndex].ItemAmount -= 1;
		if (PlayerInventory[InventoryIndex].ItemAmount == 0)
		{
			PlayerInventory[InventoryIndex].ItemRowName = FName("Empty");
		}
	}	

	SaveCurrentInventory();
	InventoryPlayerController->HUD_UpdateInventoryGrid(PlayerInventory, true, false);
}

void AInventoryCharacter::RemoveDraggedItemFromInventory(const TArray<FInventoryContents>& ItemInfo, bool bDropIntoWorld, int32 InventoryIndex)
{
	if (PlayerInventory[InventoryIndex].ItemAmount > 0 && PlayerInventory[InventoryIndex].ItemRowName != FName("Empty"))
	{
		PlayerInventory[InventoryIndex].ItemAmount = 0;
		PlayerInventory[InventoryIndex].ItemRowName = FName("Empty");

		SaveCurrentInventory();
		InventoryPlayerController->HUD_UpdateInventoryGrid(PlayerInventory, true, false);
	}	
}

void AInventoryCharacter::Server_RemoveDraggedItemFromInventory_Implementation(const TArray<FInventoryContents>& ItemInfo, bool bDropIntoWorld, int32 InventoryIndex)
{
	RemoveDraggedItemFromInventory(ItemInfo, bDropIntoWorld, InventoryIndex);
}

void AInventoryCharacter::SaveCurrentInventory()
{
	if (!IsValid(InventoryPlayerController))
	{
		UE_LOG(LogTemp, Error, TEXT("Character: (SaveCurrentInventory) PlayerController is not valid."));
		return;
	}
	InventoryPlayerController->SaveInventoryToSaveGame(PlayerInventory);
}

void AInventoryCharacter::Server_AddItemToInventory_Implementation(const TArray<FInventoryContents>& PickupContents, APickup* InPickup, int32 InventoryIndex)
{
	AddItemToInventory(PickupContents, InPickup, InventoryIndex);
}

void AInventoryCharacter::AddItemToInventory(const TArray<FInventoryContents>& PickupContents, APickup* InPickup, int32 InventoryIndex)
{		
	if (PickupContents.Num() > 1)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("Warning: Pickup items can only contain 1 item."));
		return;
	}
	
	bool bAdded = false;
	if (InventoryIndex < 0)
	{
		for (int32 i = 0; i < PlayerInventory.Num(); i++)
		{
			if (PlayerInventory[i].ItemRowName == FName("Empty"))
			{
				PlayerInventory[i] = PickupContents[0];
				bAdded = true;
				break;
			}
			else if (PlayerInventory[i].ItemRowName == PickupContents[0].ItemRowName)
			{
				PlayerInventory[i].ItemAmount += PickupContents[0].ItemAmount;
				bAdded = true;
				break;
			}
		}
		if (!bAdded)
		{
			UE_LOG(LogTemp, Warning, TEXT("Inventory is full, Can't add item to inventory."));
			return;
		}
	}
	SaveItemAndUpdateHUD(InPickup);
}

void AInventoryCharacter::Server_AddDraggedItemToInventory_Implementation(const TArray<FInventoryContents>& PickupContents, int32 InventoryIndex)
{
	AddDraggedItemToInventory(PickupContents, InventoryIndex);
}

void AInventoryCharacter::AddDraggedItemToInventory(const TArray<FInventoryContents>& PickupContents, int32 InventoryIndex)
{	
	// 1. 아이템 이름이 같은 경우 수량 추가.
	if (PlayerInventory[InventoryIndex].ItemRowName == PickupContents[0].ItemRowName)
	{
		PlayerInventory[InventoryIndex].ItemAmount += PickupContents[0].ItemAmount;
	}
	// 2. 아이템 이름이 다른 경우 아이템 스왑.
	else if (PlayerInventory[InventoryIndex].ItemRowName != PickupContents[0].ItemRowName)
	{
		// Item Swap.
	}
	// 3. 아이템 인벤토리가 비어있는 경우 그냥 추가.
	else if (PlayerInventory[InventoryIndex].ItemRowName == FName("Empty"))
	{
		PlayerInventory[InventoryIndex] = PickupContents[0];
	}
	SaveItemAndUpdateHUD(nullptr);
}

void AInventoryCharacter::SaveItemAndUpdateHUD(APickup* InPickup)
{
	if (IsValid(InPickup) && InventoryGameMode)
	{
		InventoryGameMode->Remove_SavedPickupActor(InPickup);
	}
	SaveCurrentInventory();
	if (InventoryPlayerController)
	{
		InventoryPlayerController->HUD_UpdateInventoryGrid(PlayerInventory, true, false);
	}
}

int32 AInventoryCharacter::FindEmptySlot() const
{	
	for (int32 i = 0; i < PlayerInventory.Num(); i++)
	{
		if (PlayerInventory[i].ItemRowName == FName("Empty"))
		{
			return i;
		}
	}
	return -1;
}

int32 AInventoryCharacter::FindFirstItemIndex() const
{	
	for (int32 i = 0; i < PlayerInventory.Num(); i++)
	{
		if (PlayerInventory[i].ItemRowName != FName("Empty"))
		{
			return i;
		}
	}
	return -1;
}

void AInventoryCharacter::SetOpenedContainer(AContainer* NewContainer)
{
	OpenedContainer = NewContainer;
}

void AInventoryCharacter::Server_InteractWithInInteractable_Implementation()
{	
	if (IsValid(InteractableActor) && InteractableActor->Implements<UInteractableInterface>())
	{
		// 아이템 획득 or 창고 열기
		if (OpenedContainer == nullptr)
		{
			IInteractableInterface::Execute_InteractWithActor(InteractableActor, this);
		}
		else
		{
			OpenContainer(OpenedContainer);
		}
	}
	else // 아이템 드롭
	{
		int32 DropItemIndex = FindFirstItemIndex();

		if (DropItemIndex >= 0)
		{
			TArray<FInventoryContents> DropItemArray;
			FInventoryContents DropItem;

			DropItem.ItemRowName = PlayerInventory[DropItemIndex].ItemRowName;
			DropItem.ItemAmount = 1;

			DropItemArray.Add(DropItem);

			RemoveItemFromInventory(DropItemArray, true, DropItemIndex);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Inventory is Empty. No Drop Items."));
		}
	}
}

void AInventoryCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AInventoryCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X * MouseSensitivity);
		AddControllerPitchInput(LookAxisVector.Y * MouseSensitivity);
	}
}

void AInventoryCharacter::Interact()
{
	Server_InteractWithInInteractable();
}

void AInventoryCharacter::Inventory()
{	
	if (!InventoryPlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("InventoryCharacter: PlayerController is nullptr."));
		return;
	}

	if (InventoryPlayerController->GetPlayerInventoryWidget())
	{
		InventoryPlayerController->GetPlayerInventoryWidget()->RemoveFromParent();
		InventoryPlayerController->LeaveInventoryMenu();
	}
	else
	{
		InventoryPlayerController->UI_ShowInventoryMenu();
	}	
}

void AInventoryCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AInventoryCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AInventoryCharacter::Look);		
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AInventoryCharacter::Interact);
		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this, &AInventoryCharacter::Inventory);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}

}

void AInventoryCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInventoryCharacter, PlayerInventory);
	DOREPLIFETIME(AInventoryCharacter, InventoryPlayerController);
	DOREPLIFETIME(AInventoryCharacter, OpenedContainer);
}

void AInventoryCharacter::PossessedBy(AController* NewController)
{	
	Super::PossessedBy(NewController);

	InventoryGameMode = InventoryGameMode == nullptr ? Cast<AInventoryGameModeBase>(UGameplayStatics::GetGameMode(this)) : InventoryGameMode;
	InventoryPlayerController = InventoryPlayerController == nullptr ? Cast<AInventoryPlayerController>(NewController) : InventoryPlayerController;
}

void AInventoryCharacter::Server_AddDraggedItemToContainer_Implementation(const TArray<FInventoryContents>& PickupContents, AContainer* InContainer)
{
}


