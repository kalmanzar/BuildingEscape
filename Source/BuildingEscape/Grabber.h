// Copyright Kevin Almanzar 2017

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

private:
    // How far ahead of the player can we reach in cm
    float Reach = 100.f;
    
    UPhysicsHandleComponent* PhysicsHandle = nullptr;
    
    UInputComponent* InputComponent = nullptr;
    
    // Ray-cast and grab whats in reach
    void Grab();
    
    // Called when grab is released
    void Release();
    
    // Find attached physics handle
    void FindPhysicsHandleComponent();

    // Setup attached input component
    void SetupInputComponent();
    
    // Return hit for first physics body in reach
    FHitResult GetFirstPhysicsBodyInReach();
    
    // Returns start of reach line
    FVector GetReachLineStart();
    
    // Returns current end of reach line
    FVector GetReachLineEnd();
    
};
