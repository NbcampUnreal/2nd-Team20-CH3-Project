#include "Gimmick/NXItem.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/Engine.h"
#include "Player/NXPlayerCharacter.h"
#include "TimerManager.h"

ANXItem::ANXItem()
{
    PrimaryActorTick.bCanEverTick = false;

    Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
    SetRootComponent(Scene);

    Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    Collision->SetupAttachment(Scene);

    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    StaticMesh->SetupAttachment(Collision);

    Collision->OnComponentBeginOverlap.AddDynamic(this, &ANXItem::OnItemOverlap);
    Collision->OnComponentEndOverlap.AddDynamic(this, &ANXItem::OnItemEndOverlap);
}

void ANXItem::OnItemOverlap(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)

{
    ANXPlayerCharacter* PlayerCharacter = Cast<ANXPlayerCharacter>(OtherActor);
    if (PlayerCharacter)
    {
        ActivateItem(PlayerCharacter);
    }
}


void ANXItem::OnItemEndOverlap(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex)
{
}

void ANXItem::ActivateItem(AActor* Activator)
{
	UParticleSystemComponent* Particle = nullptr;

	if (PickupParticle)
	{
		Particle = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			PickupParticle,
			GetActorLocation(),
			GetActorRotation(),
			true
		);
	}

	if (PickupSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			PickupSound,
			GetActorLocation()
		);
	}
}

FName ANXItem::GetItemType() const
{
	return ItemType;
}

void ANXItem::DestroyItem()
{
	Destroy();
}

