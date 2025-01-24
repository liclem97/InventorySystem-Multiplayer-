#pragma once

#include "CoreMinimal.h"
#include "InventoryStructure.generated.h"

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

USTRUCT(BlueprintType)
struct FInventoryContents : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FName ItemRowName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 ItemAmount;
};

USTRUCT(BlueprintType)
struct FWorldInfo_PickupItem : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FName ItemRowName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TArray<FInventoryContents> ItemContents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FTransform WorldTransform;
};
