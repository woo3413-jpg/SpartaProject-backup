#include "Coinitem.h"
#include "Engine/World.h"
#include "SpartaGameState.h"
ACoinitem::ACoinitem()
{
	PointValue = 0;
	ItemType = "DefailtCoin";

}

void ACoinitem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (UWorld* World = GetWorld())
		{
			if (ASpartaGameState* GameState = World->GetGameState<ASpartaGameState>())
			{
				GameState->AddScore(PointValue);
				GameState->OnCoinCollected();
			}
		}
		DestroyItem();
	}
}