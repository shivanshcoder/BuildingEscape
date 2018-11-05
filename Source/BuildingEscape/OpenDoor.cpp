// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include"GameFramework/Actor.h"
#include"Engine/World.h"
#include"GameFramework/PlayerController.h"
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
	ActorThatOpens = (GetWorld()->GetFirstPlayerController())->GetPawn();
	// ...
	
}

void UOpenDoor::OpenDoor()
{
	FRotator NewRotation = FRotator(0.0f, OpenAngle, 0.0f);

	GetOwner()->SetActorRotation(NewRotation);
}

void UOpenDoor::CloseDoor()
{
	FRotator NewRotation = FRotator(0.0f, -90.0f, 0.0f);

	GetOwner()->SetActorRotation(NewRotation);
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
	//Poll the Trigger Volume
	if (PressurePlate->IsOverlappingActor(ActorThatOpens)) {
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	
	if ( GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
		CloseDoor();
	//If the the Actor that opens is in the volume
	
	// ...
}

