// Copyright by liclem97.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InventoryStructure.h"
#include "InventoryCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class USphereComponent;
class APickup;
class AInventoryGameModeBase;
class AInventoryPlayerController;

UCLASS()
class INVENTORYSYSTEM_API AInventoryCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AInventoryCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PossessedBy(AController* NewController) override;

	void AddItemToInventory(TArray<FInventoryContents>& PickupContents, APickup* InPickup);

	/** Server */
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_LoadInventoryFromSaveGame(const TArray<FInventoryContents>& InPlayerInventory);
	/** End Server */

	/** Getter */
	FORCEINLINE TArray<FInventoryContents> GetPlayerInventory() const { return PlayerInventory; }
	/** End Getter*/

protected:
	virtual void BeginPlay() override;

	void SetupInputMapping();
	void RemoveItemFromInventory(TArray<FInventoryContents>& ItemInfo, bool bDropIntoWorld);
	void SaveCurrentInventory();

	/** Player Input*/
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Interact();
	void Inventory();
	/** end Player Input*/

	UFUNCTION()
	void OnInteractableDetectionOverlapBegin(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnInteractableDetectionOverlapEnd(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	UFUNCTION()
	void OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnMeshOverlapEnd(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	/** Server */
	UFUNCTION(Server, Reliable)
	void Server_InteractWithInInteractable();
	/** End Server */

private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere)
	USphereComponent* InteractableDetectionSphere;

	/** MappingContext */
	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* DefaultMappingContext;

	/** Input Actions */
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* InventoryAction;
	/** end Input Actions */

	UPROPERTY(Replicated)
	TArray<FInventoryContents> PlayerInventory;

	float MouseSensitivity;

	UPROPERTY()
	AActor* InteractableActor;

	UPROPERTY()
	AInventoryGameModeBase* InventoryGameMode;

	UPROPERTY(Replicated)
	AInventoryPlayerController* InventoryPlayerController;
};
