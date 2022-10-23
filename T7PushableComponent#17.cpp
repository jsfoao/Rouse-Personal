#include "T7PushableComponent.h"
#include "T7PushComponent.h"
#include "Components/BoxComponent.h"
#include "GP3/Library/T7Debug.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

UT7PushableComponent::UT7PushableComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	GroundTraceLength = 100.f;
	bCanPush = true;

	IKOffset = FVector::ZeroVector;
	IKHandSpacing = 1.f;
	BoxTraceSize = FVector(32.f, 32.f, 32.f);
	OverlapBoxX = FVector2D(32.f, 32.f);
	OverlapBoxY = FVector2D(32.f, 32.f);

	bDebugInvertAxis = false;
}

void UT7PushableComponent::BeginPlay()
{
	Super::BeginPlay();
	LocationPrevFrame = GetOwner()->GetActorLocation();
	ActorsToIgnore.Add(GetOwner());

	if (GetOwner()->GetActorRotation().Yaw != 0.f)
	{
		bDebugInvertAxis = true;
	}
	if (InvertCollisionAxis)
	{
		OverlapBoxX = FVector2D(OverlapBoxX.Y, OverlapBoxX.X);
		OverlapBoxY = FVector2D(OverlapBoxY.Y, OverlapBoxY.X);
	}
}

void UT7PushableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// Calculating velocity of pushable
	Velocity = GetOwner()->GetActorLocation() - LocationPrevFrame;
	Speed = Velocity.Length();
	LocationPrevFrame = GetOwner()->GetActorLocation();
	
	// Grounding
	FHitResult TraceHit;
	const FVector Location = GetOwner()->GetActorLocation();
	UKismetSystemLibrary::BoxTraceSingle(
		GetWorld(),
		Location,
		Location + (FVector::DownVector * GroundTraceLength),
		BoxTraceSize,
		FRotator::ZeroRotator,
		GroundQuery,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		TraceHit,
		true);
	//GetWorld()->LineTraceSingleByChannel(TraceHit, Location, Location + (FVector::DownVector * GroundTraceLength), GroundChannel);
	bIsGrounded = TraceHit.bBlockingHit;

	if (bDebugInvertAxis == false)
		return;
	
	DrawDebugSphere(GetWorld(), GetOwner()->GetActorLocation(), 60.f, 10.f, FColor::Yellow, false, 0, 1, 2.f);
}

bool UT7PushableComponent::IsOverlappingInner(AActor* OtherActor)
{
	return IsOverlappingAxis(OtherActor, OverlapBoxX, OverlapBoxY, OverlapBoxHeight);
}

bool UT7PushableComponent::IsOverlappingOuter(AActor* OtherActor)
{
	if (InvertCollisionAxis)
	{
		return IsOverlappingAxis(OtherActor, FVector2D(OverlapBoxX.X, OverlapBoxX.Y + OuterOffset), FVector2D(OverlapBoxY.X + OuterOffset, OverlapBoxY.Y), OverlapBoxHeight);
	}
	return IsOverlappingAxis(OtherActor, FVector2D(OverlapBoxX.X + OuterOffset, OverlapBoxX.Y), FVector2D(OverlapBoxY.X, OverlapBoxY.Y + OuterOffset), OverlapBoxHeight);
}

bool UT7PushableComponent::IsOverlappingAxis(AActor* OtherActor, FVector2D XBox, FVector2D YBox, float Height)
{
	const UT7PushComponent* PushComp = OtherActor->FindComponentByClass<UT7PushComponent>();
	if (PushComp == nullptr)
		return false;
	
	const FVector PlayerLocation = OtherActor->GetActorLocation();
	const FVector Location = GetOwner()->GetActorLocation();
	const FVector RawDirection = PlayerLocation - Location;

	// Not overlapping Z
	if (RawDirection.Z > Height || RawDirection.Z < -Height)
	{
		return false;
	}
	if (RawDirection.X < YBox.X && RawDirection.X > -YBox.X && abs(RawDirection.Y) < YBox.Y)
	{
		return true;
	}
	if (RawDirection.Y < XBox.Y && RawDirection.Y > -XBox.Y && abs(RawDirection.X) < XBox.X)
	{
		return true;
	}
	return false;
}

void UT7PushableComponent::EvaluateBoxSide(AActor* OtherActor)
{
	FVector PlayerLocation = OtherActor->GetActorLocation();
	FVector Location = GetOwner()->GetActorLocation();
	FVector RawDirection = PlayerLocation - Location;
	RawDirection.Z = 0.f;
	
	// Sides
	if (RawDirection.X > 0 && abs(RawDirection.X) > abs(RawDirection.Y))
	{
		BoxSide = FVector::ForwardVector;
		LOG("Forward");
	}
	else if (RawDirection.X < 0 && abs(RawDirection.X) > abs(RawDirection.Y))
	{
		BoxSide = FVector::BackwardVector;
		LOG("Backward");
	}
	else if (RawDirection.Y < 0 && abs(RawDirection.Y) > abs(RawDirection.X))
	{
		BoxSide = FVector::LeftVector;
		LOG("Left");
	}
	else if (RawDirection.Y > 0 && abs(RawDirection.Y) > abs(RawDirection.X))
	{
		BoxSide = FVector::RightVector;
		LOG("Right");
	}
	else
	{
		BoxSide = FVector::ZeroVector;
	}
}

bool UT7PushableComponent::EvaluateOverlap(AActor* OtherActor)
{
	const bool bIsOverlapping = IsOverlappingInner(OtherActor);
	if (!bIsOverlapping)
	{
		return false;
	}
	EvaluateBoxSide(OtherActor);
	return true;
}

void UT7PushableComponent::SetPrimitiveComponent(UBoxComponent* SetBoxComponent)
{
	PrimitiveComponent = SetBoxComponent;
}

void UT7PushableComponent::SetDragMultiplier(float Value)
{
	DragMultiplier = Value;
}

void UT7PushableComponent::SetHandIKLocation(UT7PushComponent* PushComponent)
{
	const FVector DirectionToPlayer = PushComponent->GetOwner()->GetActorLocation() - GetOwner()->GetActorLocation();
	const FVector NormalizedDirection = DirectionToPlayer.GetSafeNormal();
	const FVector ActorLocation = GetOwner()->GetActorLocation();
	const FVector PrimitiveComponentRelativeScale = PrimitiveComponent->GetComponentScale() / 2.f;
	FVector HandMidLocation;
	
	// Overlapping X
	if (BoxSide == FVector::BackwardVector || BoxSide == FVector::ForwardVector)
	{
		HandMidLocation = FVector(
		ActorLocation.X + NormalizedDirection.X * PrimitiveComponentRelativeScale.X * (100.f + IKOffset.X),
		ActorLocation.Y + DirectionToPlayer.Y,
		PushComponent->GetOwner()->GetActorLocation().Z + IKOffset.Z);

		if (InvertCollisionAxis)
		{
			HandMidLocation = FVector(
			ActorLocation.X + NormalizedDirection.X * PrimitiveComponentRelativeScale.Y * (100.f + IKOffset.X),
			ActorLocation.Y + DirectionToPlayer.Y,
			PushComponent->GetOwner()->GetActorLocation().Z + IKOffset.Z);
		}

		if (BoxSide == FVector::ForwardVector)
		{
			IKHandLocationL = HandMidLocation + FVector::LeftVector * (IKHandSpacing/2);
			IKHandLocationR = HandMidLocation + FVector::RightVector * (IKHandSpacing/2);
		}
		else if (BoxSide == FVector::BackwardVector)
		{
			IKHandLocationL = HandMidLocation + FVector::RightVector * (IKHandSpacing/2);
			IKHandLocationR = HandMidLocation + FVector::LeftVector * (IKHandSpacing/2);
		}
	}

	// Overlapping Y
	else if (BoxSide == FVector::LeftVector || BoxSide == FVector::RightVector)
	{
		HandMidLocation = FVector(
		ActorLocation.X + DirectionToPlayer.X,
		ActorLocation.Y + NormalizedDirection.Y * PrimitiveComponentRelativeScale.Y * (100.f + IKOffset.Y),
		PushComponent->GetOwner()->GetActorLocation().Z + IKOffset.Z);

		if (InvertCollisionAxis)
		{
			HandMidLocation = FVector(
			ActorLocation.X + DirectionToPlayer.X,
			ActorLocation.Y + NormalizedDirection.Y * PrimitiveComponentRelativeScale.X * (100.f + IKOffset.Y),
			PushComponent->GetOwner()->GetActorLocation().Z + IKOffset.Z);
		}

		if (BoxSide == FVector::LeftVector)
		{
			IKHandLocationL = HandMidLocation + FVector::BackwardVector * (IKHandSpacing/2);
			IKHandLocationR = HandMidLocation + FVector::ForwardVector * (IKHandSpacing/2);
		}
		else if (BoxSide == FVector::RightVector)
		{
			IKHandLocationL = HandMidLocation + FVector::ForwardVector * (IKHandSpacing/2);
			IKHandLocationR = HandMidLocation + FVector::BackwardVector * (IKHandSpacing/2);
		}
	}
	
	DrawDebugSphere(GetWorld(), IKHandLocationL, 5.f, 10.f, FColor::Red, false, 0.2f, 1, 2.f);
	DrawDebugSphere(GetWorld(), IKHandLocationR, 5.f, 10.f, FColor::Green, false, 0.2f, 1, 2.f);
}

void UT7PushableComponent::DebugBoxLines()
{
	DrawDebugBox(GetWorld(), GetOwner()->GetActorLocation(), FVector(OverlapBoxX.X, OverlapBoxX.Y, OverlapBoxHeight), FColor::Red, false, 0, 1, 1);
	DrawDebugBox(GetWorld(), GetOwner()->GetActorLocation(), FVector(OverlapBoxY.X, OverlapBoxY.Y, OverlapBoxHeight), FColor::Green, false, 0, 1, 1);
}



