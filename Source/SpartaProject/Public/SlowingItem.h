#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "SlowingItem.generated.h"


UCLASS()
class SPARTAPROJECT_API ASlowingItem : public ABaseItem
{
	GENERATED_BODY()



public:

	ASlowingItem();

protected:
	virtual void ActivateItem(AActor* Activator) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debuff")
	float SlowDuration = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debuff")
	float SlowRate = 0.5f;
};
