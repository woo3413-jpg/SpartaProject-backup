#include "SlowingItem.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"

ASlowingItem::ASlowingItem()
{
	ItemType = "SlowingItem";
}

void ASlowingItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	ACharacter* Character = Cast<ACharacter>(Activator);

	if (!Character)
	{
		return;
	}

	UCharacterMovementComponent* Movement = Character->GetCharacterMovement();

	if (!Movement)
	{
		return;
	}

	const float OriginalSpeed = Movement->MaxWalkSpeed;

	Movement->MaxWalkSpeed = OriginalSpeed * SlowRate;

	FTimerHandle SlowTimerHandle;

	GetWorld()->GetTimerManager().SetTimer(
		SlowTimerHandle,
		[Movement, OriginalSpeed]()
		{
			if (IsValid(Movement))
			{
				Movement->MaxWalkSpeed = OriginalSpeed;
			}
		},
		SlowDuration,
		false
	);

	DestroyItem();
}