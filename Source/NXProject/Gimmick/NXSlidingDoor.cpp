#include "Gimmick/NXSlidingDoor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/Character.h"

ANXSlidingDoor::ANXSlidingDoor()
{
   
    PrimaryActorTick.bCanEverTick = true;

    
    RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
    SetRootComponent(RootSceneComponent);

    InteractionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionBox"));
    InteractionBox->SetupAttachment(RootSceneComponent);

    InteractionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    InteractionBox->SetCollisionObjectType(ECC_WorldDynamic);
    InteractionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    InteractionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    InteractionBox->SetBoxExtent(FVector(100.f, 100.f, 100.f));

    InteractionBox->SetHiddenInGame(false);

    DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
    DoorMesh->SetupAttachment(RootSceneComponent);

    SlideDirection = FVector(1.0f, 0.0f, 0.0f);

    InteractionBox->OnComponentBeginOverlap.AddDynamic(this, &ANXSlidingDoor::OnDoorOverlapBegin);
    InteractionBox->OnComponentEndOverlap.AddDynamic(this, &ANXSlidingDoor::OnDoorOverlapEnd);
}

void ANXSlidingDoor::BeginPlay()
{
    Super::BeginPlay();

    InitialLocation = DoorMesh->GetRelativeLocation();

    InteractionBox->SetBoxExtent(InteractionBoxExtent);

    UE_LOG(LogTemp, Warning, TEXT("Door BeginPlay - Initial Location: %s"), *InitialLocation.ToString());
    UE_LOG(LogTemp, Warning, TEXT("Door BeginPlay - Interaction Box Extent: %s"), *InteractionBoxExtent.ToString());
}

void ANXSlidingDoor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UpdateDoorMovement(DeltaTime);
}

void ANXSlidingDoor::OnDoorOverlapBegin(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult
)
{
    ACharacter* Character = Cast<ACharacter>(OtherActor);
    if (Character && (OtherActor->ActorHasTag(FName("Player")) || Character->IsPlayerControlled()))
    {
        bPlayerInRange = true;
        bCanOpenDoor = true;

        OpenDoor();

        UE_LOG(LogTemp, Warning, TEXT("Player Entered Door Range"));

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Door Opened by Player"));
        }
    }
}

void ANXSlidingDoor::OnDoorOverlapEnd(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex
)
{
    ACharacter* Character = Cast<ACharacter>(OtherActor);
    if (Character && (OtherActor->ActorHasTag(FName("Player")) || Character->IsPlayerControlled()))
    {
        bPlayerInRange = false;

        CloseDoor();

        UE_LOG(LogTemp, Warning, TEXT("Player Left Door Range"));

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Player Left Door Range"));
        }
    }
}

void ANXSlidingDoor::OpenDoor()
{
    if (!bIsDoorOpened && !bIsOpening)
    {
        bIsOpening = true;
        bIsClosing = false;
        CurrentTime = 0.0f;

        if (OpenAnimation)
        {
            UE_LOG(LogTemp, Warning, TEXT("Door Open Animation"));
        }

        if (OpenSound)
        {
            UGameplayStatics::PlaySoundAtLocation(this, OpenSound, GetActorLocation());
        }

        if (OpenEffect)
        {
            UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, OpenEffect, GetActorLocation(), GetActorRotation());
        }


        UE_LOG(LogTemp, Warning, TEXT("Door Opening"));
    }
}

void ANXSlidingDoor::CloseDoor()
{
    if (bIsDoorOpened && !bIsClosing)
    {
        bIsClosing = true;
        bIsOpening = false;
        CurrentTime = 0.0f;

        if (CloseSound)
        {
            UGameplayStatics::PlaySoundAtLocation(this, CloseSound, GetActorLocation());
        }

        if (CloseEffect)
        {
            UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, CloseEffect, GetActorLocation(), GetActorRotation());
        }

        UE_LOG(LogTemp, Warning, TEXT("Door Closing"));
    }
}

void ANXSlidingDoor::UpdateDoorMovement(float DeltaTime)
{
    if (bIsDoorOpened && !bIsClosing)
    {
        OpenedTime += DeltaTime;

        if (OpenedTime >= DoorStayOpenDuration)
        {
            if (!bPlayerInRange)
            {
                CloseDoor();
                OpenedTime = 0.0f;
            }
            else
            {
                OpenedTime = 0.0f;
            }
        }
    }

    if (bIsOpening)
    {
        CurrentTime += DeltaTime;
        float Alpha = FMath::Clamp(CurrentTime / SlideDuration, 0.0f, 1.0f);

        FVector TargetLocation = InitialLocation + SlideDirection * SlideDistance;
        FVector NewLocation = FMath::Lerp(InitialLocation, TargetLocation, Alpha);
        DoorMesh->SetRelativeLocation(NewLocation);

        if (Alpha >= 1.0f)
        {
            bIsOpening = false;
            bIsDoorOpened = true;
            CurrentTime = 0.0f;
            UE_LOG(LogTemp, Warning, TEXT("Door Fully Opened"));
        }
    }
    else if (bIsClosing)
    {
        CurrentTime += DeltaTime;
        float Alpha = FMath::Clamp(CurrentTime / CloseDuration, 0.0f, 1.0f);

        FVector NewLocation = FMath::Lerp(DoorMesh->GetRelativeLocation(), InitialLocation, Alpha);
        DoorMesh->SetRelativeLocation(NewLocation);

        if (Alpha >= 1.0f)
        {
            bIsClosing = false;
            bIsDoorOpened = false;
            CurrentTime = 0.0f;
            UE_LOG(LogTemp, Warning, TEXT("Door Fully Closed"));
        }
    }
}




