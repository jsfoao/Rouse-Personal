#include "T7InteractableComponent.h"

#include "T7InteractableProcessor.h"

UT7InteractableComponent::UT7InteractableComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bIsActivated = false;
}

void UT7InteractableComponent::TriggerActivate()
{
	bIsActivated = true;
	OnActivate.Broadcast();
	
	if (ProcessorActor == nullptr) return;
	UT7InteractableProcessor* Processor = ProcessorActor->FindComponentByClass<UT7InteractableProcessor>();
	if (Processor == nullptr) return;
	Processor->EvaluateCallResult();
}

void UT7InteractableComponent::TriggerDeactivate()
{
	bIsActivated = false;
	OnDeactivate.Broadcast();
	
	if (ProcessorActor == nullptr) return;
	UT7InteractableProcessor* Processor = ProcessorActor->FindComponentByClass<UT7InteractableProcessor>();
	if (Processor == nullptr) return;
	Processor->EvaluateCallResult();
}



