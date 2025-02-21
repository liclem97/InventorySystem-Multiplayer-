// Copyright by liclem97.


#include "Actor/Container.h"

#include "Character/InventoryCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "PlayerController/InventoryPlayerController.h"

AContainer::AContainer()
{	
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	ContainerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ContainerMesh"));
	ContainerMesh->SetupAttachment(RootComponent);
	ContainerMesh->SetIsReplicated(true);

	BoxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTrigger"));
	BoxTrigger->SetupAttachment(RootComponent);
	BoxTrigger->SetBoxExtent(FVector(50.f, 50.f, 40.f));
}

void AContainer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AContainer, ContainerMesh);
	DOREPLIFETIME(AContainer, ContainerContents);
}

void AContainer::OnConstruction(const FTransform& Transform)
{
	if (!HasAuthority()) return;
}

void AContainer::InteractWithActor_Implementation(AInventoryCharacter* PlayerCharacter)
{
	InteractingPlayers.AddUnique(PlayerCharacter);
	PlayerCharacter->SetOpenedContainer(this);
	PlayerCharacter->OpenContainer(this);
}

void AContainer::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		InventorySize = 14;

		// 슬롯을 "Empty"로 채움.
		for (int32 i = ContainerContents.Num(); i < InventorySize; i++)
		{
			FInventoryContents EmptySlot;
			EmptySlot.ItemRowName = FName("Empty");
			EmptySlot.ItemAmount = 0;

			ContainerContents.Add(EmptySlot);
		}
	}
}

void AContainer::ContainerContentsChanged()
{
	for (AInventoryCharacter* InPlayers : InteractingPlayers)
	{
		InPlayers->GetInventoryPlayerController()->HUD_UpdateInventoryGrid(ContainerContents, false, true);
	}
}

int32 AContainer::FindEmptySlot() const
{	
	for (int32 i = 0; i < ContainerContents.Num(); i++)
	{
		if (ContainerContents[i].ItemRowName == FName("Empty"))
		{
			return i;
		}
	}
	return -1;
}

int32 AContainer::FindFirstItemIndex() const
{
	for (int32 i = 0; i < ContainerContents.Num(); i++)
	{
		if (ContainerContents[i].ItemRowName != FName("Empty"))
		{
			return i;
		}
	}
	return -1;
}

int32 AContainer::FindItemIndex(const FName& ItemName) const
{
	for (int32 i = 0; i < ContainerContents.Num(); i++)
	{
		if (ContainerContents[i].ItemRowName == ItemName)
		{
			return i;
		}
	}
	return -1;
}
