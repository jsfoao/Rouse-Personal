#pragma once
#include "CoreMinimal.h"
#include "T7TraceHitResponse.h"
#include "Components/ActorComponent.h"
#include "T7TraceShootComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GP3_API UT7TraceShootComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UT7TraceShootComponent();

public:
	UFUNCTION()
	void LineTraceShoot(FVector CameraLocation, FVector Direction, float LineRange, ECollisionChannel TraceChannel);

	UFUNCTION(BlueprintCallable)
	void LineTraceShootComponent(USceneComponent* SceneComponent, ECollisionChannel TraceChannel = ECC_WorldStatic);

	UFUNCTION(BlueprintCallable)
	UT7TraceHitResponse* Interact();
public:
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly)
	UT7TraceHitResponse* HoveredHitResponse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Range;

	UPROPERTY(BlueprintReadWrite)
	bool bIsHitting;
};
