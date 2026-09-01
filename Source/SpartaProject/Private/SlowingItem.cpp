#include "SlowingItem.h"
#include "SpartaCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"

ASlowingItem::ASlowingItem()
{
	ItemType = "SlowingItem";
}

void ASlowingItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	ASpartaCharacter* Character = Cast<ASpartaCharacter>(Activator);

	if (!Character)
	{
		return;
	}
	Character->SlowStack++;
	Character->SlowMultiplier = FMath::Pow(0.5f, Character->SlowStack);

	if (Character->GetCharacterMovement())
	{
		Character->GetCharacterMovement()->MaxWalkSpeed *= 0.5f;
	}

	FTimerHandle SlowTimerHandle;

	GetWorld()->GetTimerManager().SetTimer(
		SlowTimerHandle,
		[Character]()
		{
			if (IsValid(Character))
			{
				Character->SlowStack--;
				Character->SlowStack = FMath::Max(0, Character->SlowStack);
				Character->SlowMultiplier = FMath::Pow(0.5f, Character->SlowStack);


				if (Character->GetCharacterMovement())
				{
					Character->GetCharacterMovement()->MaxWalkSpeed *= 2.0f;
				}
			}
		},
		5.0f,
		false
	);

	DestroyItem();
}