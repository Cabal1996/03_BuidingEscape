// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "CollisionQueryParams.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "GameFramework/Actor.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	//How far player can reach in cm
	const float Rach = 100.0f;

	UPhysicsHandleComponent* PhysicsHadle = nullptr;
	
	UInputComponent* PawnInput = nullptr;

	//Find (assumed) attached physics handle
	void FindPhysicsHandleComponent();

	//Setup (assumed) attached Input Component
	void SetupInputComponent();

	//Rey-cast and grab what's in reach
	void Grab();

	//Release grabbed object
	void Release();

	//Return hit for first physic body in reach
	FHitResult GetFirstPhysicBodyInReach();

	//Get player reach start location
	FVector GetReachLineStart();

	//Get player reach end location
	FVector GetReachLineEnd();
};