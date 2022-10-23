#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "T7TraceHitResponse.generated.h"

class UT7TraceShootComponent;
class UT7FreezeResponse;
class UT7LevitateResponse;
class UT7StickyResponse;
class UT7AmmoComponent;
UCLASS(ClassGroup="Puzzle", meta=(BlueprintSpawnableComponent))
class GP3_API UT7TraceHitResponse : public UActorComponent
{
	GENERATED_BODY()

public:
	UT7TraceHitResponse();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHitEnter, UT7TraceShootComponent*, TraceShootComp);
	UPROPERTY(BlueprintAssignable)
	FOnHitEnter OnHitEnter;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHitExit, UT7TraceShootComponent*, TraceShootComp);
	UPROPERTY(BlueprintAssignable)
	FOnHitExit OnHitExit;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHitStay, UT7TraceShootComponent*, TraceShootComp);
	UPROPERTY(BlueprintAssignable)
	FOnHitStay OnHitStay;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteract, UT7TraceShootComponent*, TraceShootComp);
	UPROPERTY(BlueprintAssignable)
	FOnInteract OnInteract;

	UFUNCTION(BlueprintCallable, DisplayName="TriggerEnter")
	void TriggerEnter(UT7TraceShootComponent* TraceShootComponent);

	UFUNCTION(BlueprintCallable, DisplayName="TriggerExit")
	void TriggerExit(UT7TraceShootComponent* TraceShootComponent);

	UFUNCTION(BlueprintCallable, DisplayName="TriggerStay")
	void TriggerStay(UT7TraceShootComponent* TraceShootComponent);

	UFUNCTION(BlueprintCallable, DisplayName="TriggerInteract")
	void TriggerInteract(UT7TraceShootComponent* TraceShootComponent);
};
