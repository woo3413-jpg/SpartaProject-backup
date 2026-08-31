#pragma once

#include "CoreMinimal.h"
#include "Coinitem.h"
#include "SmallCoinitem.generated.h"


UCLASS()
class SPARTAPROJECT_API ASmallCoinitem : public ACoinitem
{
	GENERATED_BODY()
	

public:
	ASmallCoinitem();

	virtual void ActivateItem(AActor* Activator) override;
};