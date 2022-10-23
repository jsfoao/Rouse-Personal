#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "T7PushableComponent.generated.h"

class UBoxComponent;
class UT7PushComponent;
UCLASS(ClassGroup="Puzzle", meta=(BlueprintSpawnableComponent))
class GP3_API UT7PushableComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UT7PushableComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	bool IsOverlappingInner(AActor* OtherActor);

	UFUNCTION(BlueprintCallable)
	bool IsOverlappingOuter(AActor* OtherActor);

	UFUNCTION()
	bool IsOverlappingAxis(AActor* OtherActor, FVector2D XBox, FVector2D YBox, float Height);
	
	UFUNCTION()
	void EvaluateBoxSide(AActor* OtherActor);

	UFUNCTION()
	bool EvaluateOverlap(AActor* OtherActor);

	// To call on begin play in BP
	UFUNCTION()
	void SetPrimitiveComponent(UBoxComponent* SetBoxComponent);

	UFUNCTION(BlueprintCallable)
	void SetDragMultiplier(float Value);

	UFUNCTION(BlueprintCallable)
	void DebugBoxLines();

	UFUNCTION(BlueprintCallable)
	void SetHandIKLocation(UT7PushComponent* PushComponent);
	
public:
	UPROPERTY(BlueprintReadWrite, Category="Pushable")
	UPrimitiveComponent* PrimitiveComponent;
	
	UPROPERTY(BlueprintReadOnly, Category="Pushable")
	FVector BoxSide;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pushable")
	bool bCanPush;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin=0.f, ClampMax=1.f), Category="Pushable")
	float DragMultiplier;

	UPROPERTY(BlueprintReadOnly, Category="Pushable")
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category="Pushable")
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pushable")
	float OuterOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pushable")
	bool InvertCollisionAxis;

	UPROPERTY()
	FVector LocationPrevFrame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	FVector2D OverlapBoxX;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	FVector2D OverlapBoxY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	float OverlapBoxHeight;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grounding")
	float GroundTraceLength;

	UPROPERTY(EditAnywhere, Category="Grounding")
	FVector BoxTraceSize;

	UPROPERTY(EditAnywhere, Category="Grounding")
	TEnumAsByte<ETraceTypeQuery> GroundQuery;

	UPROPERTY(EditAnywhere, Category="Grounding")
	TArray<AActor*> ActorsToIgnore;
	
	UPROPERTY(EditAnywhere, Category="Grounding")
	TEnumAsByte<ECollisionChannel> GroundChannel;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Grounding")
	bool bIsGrounded;

	UPROPERTY(BlueprintReadOnly, Category="HandIK")
	FVector IKHandLocationL;
	
	UPROPERTY(BlueprintReadOnly, Category="HandIK")
	FVector IKHandLocationR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HandIK")
	float IKHandSpacing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HandIK")
	FVector IKOffset;

	UPROPERTY(EditAnywhere, Category="Pushable")
	bool bDebugInvertAxis = false;
};
