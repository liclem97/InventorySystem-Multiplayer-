// Copyright by liclem97.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractableInterface.h"
#include "Pickup.generated.h"

USTRUCT(BlueprintType)
struct FPickupItemInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FText PickupName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UStaticMesh* PickupMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float PickupMesh_Scale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	bool bShowPickupName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FVector PickupMeshOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UTexture2D* HUD_Icon;
};

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
	virtual void InteractWithActor_Implementation(ACharacter* PlayerCharacter) override;
	virtual void SetItemNameVisibility_Implementation(bool InVisibility) override;
	/** end Interactable Interface*/

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	FName ItemRowName;

	UPROPERTY(BlueprintReadOnly, Replicated)
	FText PickupName;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UWidgetComponent* Widget_PickupName;

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
