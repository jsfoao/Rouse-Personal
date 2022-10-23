#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "T7PushComponent.generated.h"

class UT7PushableComponent;
UCLASS(ClassGroup="Puzzle", meta=(BlueprintSpawnableComponent))
class GP3_API UT7PushComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UT7PushComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
	UFUNCTION(BlueprintCallable)
	void TryPush(AActor* ActorToPush);

	UFUNCTION(BlueprintCallable)
	void StopPush();
	
	UFUNCTION(BlueprintCallable)
	void MovePushable();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartPush, UT7PushableComponent*, PushableComponent);
	UPROPERTY(BlueprintAssignable)
	FOnStartPush OnStartPush;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStopPush, UT7PushableComponent*, PushableComponent);
	UPROPERTY(BlueprintAssignable)
	FOnStopPush OnStopPush;
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UT7PushableComponent* Pushable;

	UPROPERTY(EditAnywhere)
	float PushSpeed;
	
	UPROPERTY(BlueprintReadWrite)
	bool bIsPushing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LiftDistance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PushDistance;

	UPROPERTY(BlueprintReadOnly)
	FVector DirectionToPushable;

	UPROPERTY()
	FVector PushableLocationOnStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PushOffset;

	UPROPERTY(BlueprintReadWrite)
	bool bIsOverlapping;

	UPROPERTY()
	bool bLerpToLocation;

	UPROPERTY(EditAnywhere)
	float InterpSpeed;

	UPROPERTY(EditAnywhere)
	float RotationSpeed;
	
	UPROPERTY()
	FVector TargetLocation;

	UPROPERTY()
	FRotator TargetRotation;

	UPROPERTY()
	float TargetYaw;

	UPROPERTY()
	float Timer;

	UPROPERTY(EditAnywhere)
	float DefaultTime;
};
