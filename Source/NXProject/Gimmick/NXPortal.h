#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h" 
#include "Components/StaticMeshComponent.h"
#include "NXPortal.generated.h"

UCLASS()
class NXPROJECT_API ANXPortal : public AActor
{
	GENERATED_BODY()
	
public:	
	ANXPortal();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Portal|Components")
	class USceneComponent* RootSceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Portal|Components")
	class UStaticMeshComponent* PortalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Portal|Components")
	class UBoxComponent* PortalBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Portal|Settings")
	FName TargetLevelName = FName("Metro1");

private:
	UFUNCTION()
	void OnPortalOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
};
