#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "T7InteractableProcessor.generated.h"

UCLASS(ClassGroup="Puzzle", meta=(BlueprintSpawnableComponent))
class GP3_API UT7InteractableProcessor : public UActorComponent
{
	GENERATED_BODY()

public:
	UT7InteractableProcessor();
	
public:
	UFUNCTION()
	void EvaluateCallResult();

	UFUNCTION(BlueprintCallable)
	void TriggerActivate();

	UFUNCTION(BlueprintCallable)
	void TriggerDeactivate();
	
public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TArray<AActor*> InputInteractables;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TArray<AActor*> OutputResponses;
};
