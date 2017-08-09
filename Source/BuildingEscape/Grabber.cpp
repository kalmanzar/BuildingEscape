// Copyright Kevin Almanzar 2017

#include "BuildingEscape.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
    FindPhysicsHandleComponent();
    SetupInputComponent();
    
}

/// Look for attached Physics Handle
void UGrabber::FindPhysicsHandleComponent() {
    PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
    if (PhysicsHandle) {
        // Physics Handle is found
    }
    else {
        UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component!"), *GetOwner()->GetName());
    }
}

/// Setup attached Input Component
void UGrabber::SetupInputComponent() {
    InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
    if (InputComponent) {
        UE_LOG(LogTemp, Warning, TEXT("Input component found!"));
        /// Bind the input axis
        InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
        InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
        
    }
    else {
        UE_LOG(LogTemp, Error, TEXT("%s missing input component!"), *GetOwner()->GetName());
    }
}

void UGrabber::Grab() {
    UE_LOG(LogTemp, Warning, TEXT("Grab pressed"));
    
    // LINE TRACE and see if we reach any actors with physics body collision channel set
    GetFirstPhysicsBodyInReach();
    
    /// If we hit something then attach a physics handle
    // TODO attach a physics handle
}

void UGrabber::Release() {
    UE_LOG(LogTemp, Warning, TEXT("Grab released"));
    // TODO release physics handle
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const{
    // Get the player viewpoint this tick
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
        OUT PlayerViewPointLocation,
        OUT PlayerViewPointRotation
    );
    
    FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
    
    /// Setup query parameters
    FCollisionQueryParams TraceParameters(FName(""), false, GetOwner());
    
    /// Line-trace (AKA ray-cast) out to reach distance
    FHitResult Hit;
    GetWorld()->LineTraceSingleByObjectType(
        OUT Hit,
        PlayerViewPointLocation,
        LineTraceEnd,
        FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
        TraceParameters
    );
    
    /// See what we hit
    AActor* ActorHit = Hit.GetActor();
    if (ActorHit) {
        UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *(ActorHit->GetName()));
    }
    
    return Hit;
}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

    // If the physics handle is attached
        // Move object we are holding

}

