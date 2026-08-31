#pragma once

#include "CoreMinimal.h"
#include "Coinitem.h"
#include "BigCoinitem.generated.h"

UCLASS()
class SPARTAPROJECT_API ABigCoinitem : public ACoinitem
{
	GENERATED_BODY()

public:
	ABigCoinitem();

	virtual void ActivateItem(AActor* Activator) override;
};