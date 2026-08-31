#include "BigCoinitem.h"

ABigCoinitem::ABigCoinitem()
{
	PointValue = 50;
	ItemType = "BigCoin";
}

void ABigCoinitem::ActivateItem(AActor* Activator) 
{
	Super::ActivateItem(Activator);
}