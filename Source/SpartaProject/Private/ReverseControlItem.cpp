#include "ReverseControlItem.h"
#include "SpartaCharacter.h"
#include "TimerManager.h"

AReverseControlItem::AReverseControlItem()
{
	ItemType = "ReverseControlItem";
	
}

void AReverseControlItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	ASpartaCharacter* Character = Cast<ASpartaCharacter>(Activator);

	if (!Character)
	{
		return;
	}
	Character->ReverseStack++;
	Character->bReverseControl = true;

	FTimerHandle ReverseTimerHandle;

	GetWorld()->GetTimerManager().SetTimer(
		ReverseTimerHandle,
		[Character]()
		{
			if (IsValid(Character))
			{
				Character->ReverseStack--;
				Character->ReverseStack = FMath::Max(0, Character->ReverseStack);
				Character->bReverseControl = Character->ReverseStack > 0;

			}
		},
		ReverseDuration,
		false
	);
	DestroyItem();
}