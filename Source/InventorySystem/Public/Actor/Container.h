// Copyright by liclem97.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractableInterface.h"
#include "InventoryStructure.h"
#include "Container.generated.h"

class AInventoryCharacter;
class UBoxComponent;

UCLASS()
class INVENTORYSYSTEM_API AContainer : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	AContainer();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnConstruction(const FTransform& Transform) override;
	void ContainerContentsChanged();

	int32 FindEmptySlot() const;
	int32 FindItemIndex(const FName& ItemName) const;

	/** Getter */
	TArray<FInventoryContents> GetContainerContents() const { return ContainerContents; }
	TArray<AInventoryCharacter*> GetInteractionPlayers() const { return InteractingPlayers; }
	FName GetContainerRowName() const { return ContainerRowName; }
	/** End Getter*/

	/** Setter */
	void SetContainerRowName(FName InRowName) { ContainerRowName = InRowName; }
	void SetContainerContents(const TArray<FInventoryContents>& InContents) { ContainerContents = InContents; }
	void SetContainerMesh(UStaticMesh* InMesh) { ContainerMesh->SetStaticMesh(InMesh); }
	/** End Setter */

	/** Interactable Interface */
	virtual void InteractWithActor_Implementation(AInventoryCharacter* PlayerCharacter) override;
	/** end Interactable Interface*/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	TArray<FInventoryContents> ContainerContents;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TArray<AInventoryCharacter*> InteractingPlayers;

	UPROPERTY()
	USceneComponent* Root;

	UPROPERTY(EditAnywhere, Replicated)
	UStaticMeshComponent* ContainerMesh;

	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxTrigger;

	UPROPERTY()
	FName ContainerRowName;

	int32 InventorySize;
};
