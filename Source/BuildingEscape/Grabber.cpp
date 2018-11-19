// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include"GameFramework/Actor.h"
#include"Engine/World.h"
#include"GameFramework/PlayerController.h"
#include"DrawDebugHelpers.h"
#include"Components/InputComponent.h"
#include"Engine/EngineBaseTypes.h"
#include"Engine/EngineTypes.h"


#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	GetPhysicsHandle();

	SetupInputComponent();

}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent) {
		InputComponent->BindAction("Grab", EInputEvent::IE_Released, this, &UGrabber::Release);
		InputComponent->BindAction("Grab", EInputEvent::IE_Pressed, this, &UGrabber::Grab);
	}
	else 
		UE_LOG(LogTemp, Error, TEXT("%s missing the Input component"), *GetOwner()->GetName());
}

void UGrabber::GetPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (!PhysicsHandle) 
		UE_LOG(LogTemp, Error, TEXT("%s missing the physics handle component"), *GetOwner()->GetName());
}

void UGrabber::Release() {
	PhysicsHandle->ReleaseComponent();
}

void UGrabber::Grab() {
	auto Hit = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = Hit.GetComponent();
	auto ActorHit = Hit.GetActor();

	if (ActorHit) {
		PhysicsHandle->GrabComponent(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), true);
	}
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	auto TraceLine = GetLineTrace();

	if (PhysicsHandle->GrabbedComponent) {
		PhysicsHandle->SetTargetLocation(TraceLine.End);
	}
}

UGrabber::LineTrace UGrabber::GetLineTrace()
{
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;

	//Get player view point this tick
	APlayerController *PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->GetPlayerViewPoint(
		OUT PlayerViewpointLocation, OUT PlayerViewpointRotation);

	FVector LineTracedEnd = PlayerViewpointLocation + PlayerViewpointRotation.Vector() *  Reach;

	return LineTrace{ PlayerViewpointLocation, LineTracedEnd };
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	auto TraceLine = GetLineTrace();

	DrawDebugLine(GetWorld(),
		TraceLine.Start,
		TraceLine.End,
		FColor(255, 0, 0),
		false,
		0.0f,
		0.0f,
		10.0f);

	FHitResult Hit;

	///Setup Query Params
	FCollisionQueryParams TraceParam(FName(TEXT("")), false, GetOwner());
	///Ray-cast out to reach distance
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		TraceLine.Start,
		TraceLine.End, 
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParam
	);
	return Hit;
}

