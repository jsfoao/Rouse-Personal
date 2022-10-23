#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "T7InteractableComponent.generated.h"

UCLASS(ClassGroup="Puzzle", meta=(BlueprintSpawnableComponent))
class GP3_API UT7InteractableComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UT7InteractableComponent();

public:
	UFUNCTION(BlueprintCallable)
	void TriggerActivate();

	UFUNCTION(BlueprintCallable)
	void TriggerDeactivate();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActivate);
	UPROPERTY(BlueprintAssignable)
	FOnActivate OnActivate;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeactivate);
	UPROPERTY(BlueprintAssignable)
	FOnActivate OnDeactivate;

	UPROPERTY(BlueprintReadWrite)
	bool bIsActivated;

	UPROPERTY(EditInstanceOnly)
	AActor* ProcessorActor;
};
