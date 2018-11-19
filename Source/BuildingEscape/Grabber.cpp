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
		UE_LOG(LogTemp, Warning, TEXT("Input Component Found"));
		InputComponent->BindAction("Grab", EInputEvent::IE_Released, this, &UGrabber::Release);
		InputComponent->BindAction("Grab", EInputEvent::IE_Pressed, this, &UGrabber::Grab);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s missing the Input component"), *GetOwner()->GetName());
	}
}

void UGrabber::GetPhysicsHandle()
{

	UE_LOG(LogTemp, Warning, TEXT("Grabber Reporting for Duty!"));

	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle) {
		UE_LOG(LogTemp, Warning, TEXT("PhysicsHandle Component Found"));
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s missing the physics handle component"), *GetOwner()->GetName());
	}
}

void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("%s Release"), *GetOwner()->GetName());
	PhysicsHandle->ReleaseComponent();
}

void UGrabber::Grab() {

	UE_LOG(LogTemp, Warning, TEXT("%s Grab"), *GetOwner()->GetName());

	auto Hit = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = Hit.GetComponent();
	auto ActorHit = Hit.GetActor();

	if (ActorHit) {
		UE_LOG(LogTemp, Warning, TEXT(" Held"));
		PhysicsHandle->GrabComponent(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), true);
	}
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;

	//Get player view point this tick
	APlayerController *PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->GetPlayerViewPoint(
		OUT PlayerViewpointLocation, OUT PlayerViewpointRotation);

	FVector LineTracedEnd = PlayerViewpointLocation + PlayerViewpointRotation.Vector() *  Reach;

	if (PhysicsHandle->GrabbedComponent) {
		PhysicsHandle->SetTargetLocation(LineTracedEnd);
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{

	UE_LOG(LogTemp, Warning, TEXT("%s Line Trace"), *GetOwner()->GetName());
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;

	//Get player view point this tick
	APlayerController *PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->GetPlayerViewPoint(
		OUT PlayerViewpointLocation, OUT PlayerViewpointRotation);

	FVector LineTracedEnd = PlayerViewpointLocation + PlayerViewpointRotation.Vector() *  Reach;

	DrawDebugLine(GetWorld(),
		PlayerViewpointLocation,
		LineTracedEnd,
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
		PlayerViewpointLocation,
		LineTracedEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParam
	);
	return Hit;
}

