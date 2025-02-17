// Copyright by liclem97.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "DragDrop.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UDragDrop : public UDragDropOperation
{
	GENERATED_BODY()

public:
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	FName ItemRowName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	int32 ItemAmount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	bool bIsWorldItem;
	
};
