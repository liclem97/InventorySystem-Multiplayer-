// Copyright by liclem97.


#include "Actor/Pickup.h"

#include "Character/InventoryCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"

APickup::APickup()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	PickupMesh->SetupAttachment(RootComponent);
	PickupMesh->SetRelativeLocation(FVector(0.f, 0.f, 30.f));
	PickupMesh->SetCollisionProfileName(FName("NoCollision"));
	PickupMesh->SetGenerateOverlapEvents(false);
	PickupMesh->SetIsReplicated(true);

	BoxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTrigger"));
	BoxTrigger->SetupAttachment(RootComponent);
	BoxTrigger->SetBoxExtent(FVector(50.f, 50.f, 30.f));
	BoxTrigger->SetCollisionProfileName(FName("Pickup_Profile"));
	BoxTrigger->SetGenerateOverlapEvents(true);
	BoxTrigger->SetRelativeLocation(FVector(0.f, 0.f, 30.f));

	Widget_PickupName = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget_PickupName"));
	Widget_PickupName->SetupAttachment(RootComponent);
	Widget_PickupName->SetWidgetSpace(EWidgetSpace::Screen);
	Widget_PickupName->SetDrawAtDesiredSize(true);
	Widget_PickupName->SetVisibility(false);
	Widget_PickupName->SetRelativeLocation(FVector(0.f, 0.f, 70.f));
}

void APickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APickup, PickupMesh);
	DOREPLIFETIME(APickup, PickupName);
	DOREPLIFETIME(APickup, bShowPickiupName);
}

void APickup::OnConstruction(const FTransform& Transform)
{
	if (!HasAuthority()) return;

	if (!IsValid(ItemDataTable))
	{
		UE_LOG(LogTemp, Warning, TEXT("Pickup: Item DataTable is not valid."));
		return;
	}

	FString ContextString;
	FPickupItemInfo* RowData = ItemDataTable->FindRow<FPickupItemInfo>(ItemRowName, ContextString);

	if (RowData)
	{	
		PickupName = RowData->PickupName;
		PickupMesh->SetStaticMesh(RowData->PickupMesh);
		PickupMesh->SetRelativeScale3D(FVector(RowData->PickupMesh_Scale));
		PickupMesh->SetRelativeLocation(RowData->PickupMeshOffset);
		bShowPickiupName = RowData->bShowPickupName;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Pickup: Can't find RowData."));
	}
}

void APickup::InteractWithActor_Implementation(AInventoryCharacter* PlayerCharacter)
{	
	PlayerCharacter->AddItemToInventory(ItemContents, this);
	Destroy();
}

void APickup::SetItemNameVisibility_Implementation(bool InVisibility)
{
	if (bShowPickiupName)
	{
		Widget_PickupName->SetVisibility(InVisibility);
	}
}

void APickup::SetItemRowName(FName InItemRowName)
{
	ItemRowName = InItemRowName;
}

void APickup::SetItemContents(const TArray<FInventoryContents>& InItemContents)
{
	ItemContents = InItemContents;
}

void APickup::SetItemDataTable(UDataTable* InItemDataTable)
{
	ItemDataTable = InItemDataTable;
}

void APickup::BeginPlay()
{
	Super::BeginPlay();
	
}

