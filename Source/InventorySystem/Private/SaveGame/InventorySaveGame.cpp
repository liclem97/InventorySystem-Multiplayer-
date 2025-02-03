// Copyright by liclem97.

#include "SaveGame/InventorySaveGame.h"

void UInventorySaveGame::SetPickupActorsInfo(TArray<FWorldInfo_PickupItem>& InActorsInfo)
{	
	All_PickupActorsInfo = InActorsInfo;
}

void UInventorySaveGame::SetIsFirstTimeLoadingMap(bool InFirstTimeLoadingMap)
{
	bIsFirstTimeLoadingMap = InFirstTimeLoadingMap;
}
