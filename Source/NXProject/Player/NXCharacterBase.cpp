#include "Player/NXCharacterBase.h"


ANXCharacterBase::ANXCharacterBase()
{
 	
	PrimaryActorTick.bCanEverTick = true;

}


void ANXCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ANXCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ANXCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

