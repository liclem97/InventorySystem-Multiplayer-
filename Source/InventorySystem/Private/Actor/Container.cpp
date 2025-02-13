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

void AContainer::InteractWithActor_Implementation(AInventoryCharacter* PlayerCharacter)
{
	InteractingPlayers.AddUnique(PlayerCharacter);
	PlayerCharacter->SetOpenedContainer(this);
	PlayerCharacter->OpenContainer(this);
}

void AContainer::BeginPlay()
{
	Super::BeginPlay();
	
}

void AContainer::ContainerContentsChanged()
{
	for (AInventoryCharacter* InPlayers : InteractingPlayers)
	{
		InPlayers->GetInventoryPlayerController()->HUD_UpdateInventoryGrid(ContainerContents, false, true);
	}
}
