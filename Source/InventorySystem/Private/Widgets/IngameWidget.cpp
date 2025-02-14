// Copyright by liclem97.


#include "Widgets/IngameWidget.h"

#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Kismet/KismetSystemLibrary.h"

void UIngameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!UKismetSystemLibrary::IsServer(this)) //client
	{
		Set_SetSlotVisibility(true);
	}
	else //server
	{
		Border_DEBUG_ItemCounter->SetVisibility(ESlateVisibility::Visible);
		//DEBUG_ItemCounter();
		
		//GetWorld()->GetTimerManager().SetTimer(TimerHandle_DebugItemCounter, this, &UIngameWidget::DEBUG_ItemCounter, 1.f, true);
	}
}

void UIngameWidget::Set_SetSlotVisibility(bool NewVisibility)
{
	if (NewVisibility)
	{
		Border_SaveSlotText->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Border_SaveSlotText->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UIngameWidget::Set_SaveSlotText(FText SlotNumber)
{	
	FText InText = FText::FromString(FString::Printf(TEXT("SaveSlot: %s"), *SlotNumber.ToString()));
	TextBlock_SaveSlotHint->SetText(InText);
}
