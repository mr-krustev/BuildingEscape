// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll TriggerVolume
	if (GetTotalMassOfActorOnPlate() > TriggerMass) {
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	else if (GetWorld()->GetTimeSeconds() >= (LastDoorOpenTime + DoorCloseDelay)){
		CloseDoor();
	}

}

void UOpenDoor::OpenDoor()
{
	Owner->SetActorRotation(FRotator{ 0.0f, OpenAngle, 0.0f });
}

void UOpenDoor::CloseDoor()
{
	Owner->SetActorRotation(FRotator{ 0.0f, 0.0f, 0.0f });
}

float UOpenDoor::GetTotalMassOfActorOnPlate()
{
	float TotalMass = 0.f;

	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// TODO: Add class filter.
	for (const auto& OverlappingActor : OverlappingActors) 
	{
		TotalMass += OverlappingActor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s is on pressure plate."), *(OverlappingActor->GetName()));
	}
	return TotalMass;
}