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

	/** Getter */
	TArray<FInventoryContents> GetContainerContents() const { return ContainerContents; }
	TArray<AInventoryCharacter*> GetInteractionPlayers() const { return InteractingPlayers; }
	/** End Getter*/

	/** Interactable Interface */
	virtual void InteractWithActor_Implementation(AInventoryCharacter* PlayerCharacter) override;
	/** end Interactable Interface*/

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	TArray<FInventoryContents> ContainerContents;

private:
	UPROPERTY()
	TArray<AInventoryCharacter*> InteractingPlayers;

	UPROPERTY()
	USceneComponent* Root;

	UPROPERTY(EditAnywhere, Replicated)
	UStaticMeshComponent* ContainerMesh;

	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxTrigger;
};
