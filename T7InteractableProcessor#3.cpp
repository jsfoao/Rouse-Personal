#include "T7InteractableProcessor.h"
#include "T7InteractableComponent.h"
#include "T7InteractableResponse.h"

UT7InteractableProcessor::UT7InteractableProcessor()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UT7InteractableProcessor::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!bDrawGizmos)
		return;

	DrawDebugSphere(GetWorld(), GetOwner()->GetActorLocation(), 30.f, 10.f, FColor::Blue, false, 0, 1, 1);
	for (auto Input : InputInteractables)
	{
		DrawDebugSphere(GetWorld(), Input->GetActorLocation(), 20.f, 10.f, FColor::Red, false, 0, 1, 1);
		DrawDebugLine(GetWorld(), GetOwner()->GetActorLocation(), Input->GetActorLocation(), FColor::Red, false, 0, 1, 2);
	}

	for (auto Output : OutputResponses)
	{
		DrawDebugSphere(GetWorld(), Output->GetActorLocation(), 20.f, 10.f, FColor::Yellow, false, 0, 1, 1);
		DrawDebugLine(GetWorld(), GetOwner()->GetActorLocation(), Output->GetActorLocation(), FColor::Red, false, 0, 1, 2);
	}
}

void UT7InteractableProcessor::BeginPlay()
{
	Super::BeginPlay();
	SetupInteractables();
}

void UT7InteractableProcessor::SetupInteractables()
{
	for (const auto Input : InputInteractables)
	{
		UT7InteractableComponent* InteractableComp = Input->FindComponentByClass<UT7InteractableComponent>();
		InteractableComp->ProcessorActor = GetOwner();
	}
}


void UT7InteractableProcessor::EvaluateCallResult()
{
	// AND processor
	bool bAndFlag = false;
	for (const auto Input : InputInteractables)
	{
		const UT7InteractableComponent* InteractableComp = Input->FindComponentByClass<UT7InteractableComponent>();
		check(InteractableComp);

		bAndFlag = InteractableComp->bIsActivated;

		if (bAndFlag == false)
		{
			TriggerDeactivate();
			return;
		};
	}
	TriggerActivate();
}

void UT7InteractableProcessor::TriggerActivate()
{
	if (OutputResponses.Num() == 0) return;
	
	for (const auto Output : OutputResponses)
	{
		if (Output == nullptr) continue;
		
		const UT7InteractableResponse* InteractableResp = Output->FindComponentByClass<UT7InteractableResponse>();
		if (InteractableResp == nullptr) continue;

		InteractableResp->OnActivate.Broadcast();
	}
}

void UT7InteractableProcessor::TriggerDeactivate()
{
	if (OutputResponses.Num() == 0) return;
	
	for (const auto Output : OutputResponses)
	{
		if (Output == nullptr) continue;
		
		const UT7InteractableResponse* InteractableResp = Output->FindComponentByClass<UT7InteractableResponse>();
		if (InteractableResp == nullptr) continue;

		InteractableResp->OnDeactivate.Broadcast();
	}
}

