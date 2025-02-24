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
class AContainer;

UCLASS()
class INVENTORYSYSTEM_API AInventoryCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AInventoryCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PossessedBy(AController* NewController) override;

	void AddItemToInventory(const TArray<FInventoryContents>& PickupContents, APickup* InPickup, int32 InventoryIndex);
	void SaveItemAndUpdateHUD(APickup* InPickup);

	int32 FindEmptySlot() const; 
	int32 FindFirstItemIndex() const;

	/** Server */
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_LoadInventoryFromSaveGame(const TArray<FInventoryContents>& InPlayerInventory);

	UFUNCTION(Server, Reliable)
	void Server_CloseContainer();

	UFUNCTION(Server, Reliable)
	void Server_AddItemToContainer(const TArray<FInventoryContents>& InContents);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_RemoveItemFromContainer(const TArray<FInventoryContents>& InContents);

	UFUNCTION(Server, Reliable)
	void Server_AddItemToInventory(const TArray<FInventoryContents>& PickupContents, APickup* InPickup, int32 InventoryIndex);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_RemoveItemFromInventory(const TArray<FInventoryContents>& ItemInfo, bool bDropIntoWorld, int32 InventoryIndex);

	UFUNCTION(Server, Reliable)
	void Server_RemoveDraggedItemFromInventory(const TArray<FInventoryContents>& ItemInfo, bool bDropIntoWorld, int32 InventoryIndex);

	/** End Server */

	/** Client */
	UFUNCTION(Client, Reliable)
	void OpenContainer(AContainer* InContainer);
	/** End Client*/

	void HandlePickupAndSave(APickup* InPickup);

	/** Setter */
	void SetOpenedContainer(AContainer* NewContainer);
	/** End Setter*/

	/** Getter */
	FORCEINLINE TArray<FInventoryContents> GetPlayerInventory() const { return PlayerInventory; }
	FORCEINLINE AInventoryPlayerController* GetInventoryPlayerController() const { return InventoryPlayerController; }
	FORCEINLINE AContainer* GetOpenedContainer() const { return OpenedContainer; }
	/** End Getter*/

protected:
	virtual void BeginPlay() override;

	void SetupInputMapping();
	void RemoveItemFromInventory(const TArray<FInventoryContents>& ItemInfo, bool bDropIntoWorld, int32 InventoryIndex);
	void RemoveDraggedItemFromInventory(const TArray<FInventoryContents>& ItemInfo, bool bDropIntoWorld, int32 InventoryIndex);
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

	/** Client */
	UFUNCTION(Client, Reliable)
	void LeaveContainerTrigger();
	/** End Client */

	UPROPERTY(Replicated, BlueprintReadOnly)
	TArray<FInventoryContents> PlayerInventory;

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

	float MouseSensitivity;

	UPROPERTY()
	AActor* InteractableActor;

	UPROPERTY()
	AInventoryGameModeBase* InventoryGameMode;

	UPROPERTY(Replicated)
	AInventoryPlayerController* InventoryPlayerController;

	UPROPERTY(Replicated)
	AContainer* OpenedContainer;

	int32 InventorySize;

};
