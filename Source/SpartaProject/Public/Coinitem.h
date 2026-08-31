#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "Coinitem.generated.h"


UCLASS()
class SPARTAPROJECT_API ACoinitem : public ABaseItem
{
	GENERATED_BODY()
	
public:
	ACoinitem();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 PointValue;

	virtual void ActivateItem(AActor* Activator) override;
};