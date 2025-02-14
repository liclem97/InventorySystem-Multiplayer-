// Copyright by liclem97.

#include "SaveGame/InventorySaveGame.h"

void UInventorySaveGame::SetPickupActorsInfo(const TArray<FWorldInfo_PickupItem>& InActorsInfo)
{	
	All_PickupActorsInfo = InActorsInfo;
}

void UInventorySaveGame::SetContainerActorsInfo(const TArray<FWorldInfo_Containers>& InActorsInfo)
{
	All_ContainerActorsInfo = InActorsInfo;
}

void UInventorySaveGame::SetIsFirstTimeLoadingMap(bool InFirstTimeLoadingMap)
{
	bIsFirstTimeLoadingMap = InFirstTimeLoadingMap;
}
