#include "SmallCoinitem.h"

ASmallCoinitem::ASmallCoinitem()
{
	PointValue = 10;
	ItemType = "SmallCoin";
}

void ASmallCoinitem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
}