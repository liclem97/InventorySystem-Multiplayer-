// Copyright by liclem97.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractableInterface.h"
#include "InventoryStructure.h"
#include "Pickup.generated.h"

class UBoxComponent;
class UWidgetComponent;

UCLASS()
class INVENTORYSYSTEM_API APickup : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	APickup();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnConstruction(const FTransform& Transform) override;	

	/** Interactable Interface */
	virtual void InteractWithActor_Implementation(AInventoryCharacter* PlayerCharacter) override;
	virtual void SetItemNameVisibility_Implementation(bool InVisibility) override;
	/** end Interactable Interface*/

	/** Getter */
	FORCEINLINE FName GetItemRowName() const { return ItemRowName; }
	FORCEINLINE TArray<FInventoryContents> GetItemContents() const { return ItemContents; }
	/** End Getter */

	/** Setter */
	void SetItemRowName(FName InItemRowName);
	void SetItemContents(const TArray<FInventoryContents>& InItemContents);
	void SetItemDataTable(UDataTable* InItemDataTable);
	/** End Setter */

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ItemRowName;

	UPROPERTY(BlueprintReadOnly, Replicated)
	FText PickupName;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UWidgetComponent* Widget_PickupName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FInventoryContents> ItemContents;

private:
	UPROPERTY()
	USceneComponent* Root;

	UPROPERTY(EditAnywhere, Replicated)
	UStaticMeshComponent* PickupMesh;

	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxTrigger;

	UPROPERTY(EditDefaultsOnly, Category = "DataTable")
	UDataTable* ItemDataTable;

	UPROPERTY(Replicated)
	bool bShowPickiupName;
};
