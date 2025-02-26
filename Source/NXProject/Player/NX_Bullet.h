#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NX_Bullet.generated.h"

UCLASS()
class NXPROJECT_API ANX_Bullet : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ANX_Bullet();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, 
		AActor* OtherActer, 
		UPrimitiveComponent* OntherComponent, 
		FVector NormalImpulse,
		const FHitResult& Hit
	);

private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComponent;
};
