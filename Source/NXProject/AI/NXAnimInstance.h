#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "NXAnimInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCheckHit); // 델리게이트 자료형 정의


UCLASS()
class NXPROJECT_API UNXAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UFUNCTION()
	void AnimNotify_CheckHit();

public:
	FOnCheckHit OnCheckHit;
	
};
