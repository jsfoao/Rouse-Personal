#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "T7InteractableTrigger.generated.h"

UCLASS( ClassGroup="Puzzle", meta=(BlueprintSpawnableComponent) )
class GP3_API UT7InteractableTrigger : public UActorComponent
{
	GENERATED_BODY()

public:	
	UT7InteractableTrigger();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin = 0.f, ClampMax = 100.f))
	float Weight;
};
