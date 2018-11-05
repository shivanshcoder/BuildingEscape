// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include"GameFramework/Actor.h"
#include"Engine/World.h"
#include"GameFramework/PlayerController.h"
#include"DrawDebugHelpers.h"


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

	UE_LOG(LogTemp, Warning, TEXT("Grabber Reporting for Duty!"))
	// ...
	
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

	FVector LineTracedEnd = PlayerViewpointLocation + PlayerViewpointRotation.Vector() *  Reach ;

	DrawDebugLine(GetWorld(),
		PlayerViewpointLocation,
		LineTracedEnd,
		FColor(255, 0, 0),
		false,
		0.0f,
		0.0f,
		10.0f);

	//Logging the position of player every tick
	/*auto Location = PlayerViewpointLocation.ToString();
	auto Rotation = PlayerViewpointRotation.ToString();
	UE_LOG(LogTemp, Display, TEXT("The Player is at %s at Rotation %s"), *Location, *Rotation)
*/
	//Ray-cast out to reach distance
	// ...
}

