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

	/*USkeletalMeshComponent* GetMesh() const
	{
		return Mesh;
	}*/


protected:

	virtual void BeginPlay() override;
	
public:

	// **총알 발사 함수**
	void Fire();

private:

	// **총의 Skeletal Mesh**
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	TObjectPtr<USkeletalMeshComponent> Mesh;

	// **총구 위치를 나타내는 SceneComponent**
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<USceneComponent> MuzzleOffset;

	// **발사 애니메이션 몽타주**
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<class UAnimMontage> FireMontage;

	// **발사될 총알 클래스**
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	TSubclassOf<class ANX_Bullet> Bullet;



};
