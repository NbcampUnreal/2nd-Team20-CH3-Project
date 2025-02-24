#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NXWeaponRifle.generated.h"

UCLASS()
class NXPROJECT_API ANXWeaponRifle : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ANXWeaponRifle();

	USkeletalMeshComponent* GetMesh() const
	{
		return Mesh;
	}

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	TObjectPtr<USkeletalMeshComponent> Mesh;
	

};
