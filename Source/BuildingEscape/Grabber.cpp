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
    if (PhysicsHandle == nullptr) {
        UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component!"), *GetOwner()->GetName());
    }
}

/// Setup attached Input Component
void UGrabber::SetupInputComponent() {
    InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
    if (InputComponent) {
        InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
        InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
    }
    else {
        UE_LOG(LogTemp, Error, TEXT("%s missing input component!"), *GetOwner()->GetName());
    }
}

void UGrabber::Grab() {
    // LINE TRACE and see if we reach any actors with physics body collision channel set
    auto HitResult = GetFirstPhysicsBodyInReach();
    auto ComponentToGrab = HitResult.GetComponent(); // gets the mesh in our case
    auto ActorHit = HitResult.GetActor();
    
    /// If we hit something then attach a physics handle
    if (ActorHit) {
        PhysicsHandle->GrabComponent(
            ComponentToGrab,
            NAME_None, // no bones needed
            ComponentToGrab->GetOwner()->GetActorLocation(),
            true // allows rotation
        );
    }
}

void UGrabber::Release() {
    PhysicsHandle->ReleaseComponent();
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() {
    /// Line-trace (AKA ray-cast) out to reach distance
    FCollisionQueryParams TraceParameters(FName(""), false, GetOwner());
    FHitResult HitResult;
    
    GetWorld()->LineTraceSingleByObjectType(
        OUT HitResult,
        GetReachLineStart(),
        GetReachLineEnd(),
        FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
        TraceParameters
    );
    
    /// See what we hit
    AActor* ActorHit = HitResult.GetActor();
    if (ActorHit) {
        UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *(ActorHit->GetName()));
    }
    
    return HitResult;
}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
    
    // If the physics handle is attached
    if (PhysicsHandle->GrabbedComponent) {
        // Move object we are holding
        PhysicsHandle->SetTargetLocation(GetReachLineEnd());
    }

}

FVector UGrabber::GetReachLineStart() {
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
        OUT PlayerViewPointLocation,
        OUT PlayerViewPointRotation
    );
    
    return PlayerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd() {
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
        OUT PlayerViewPointLocation,
        OUT PlayerViewPointRotation
    );
    
    return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}

