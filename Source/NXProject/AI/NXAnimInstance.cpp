#include "AI/NXAnimInstance.h"

void UNXAnimInstance::AnimNotify_CheckHit()
{
	if (OnCheckHit.IsBound() == true) // 해당 델리게이트에 1개의 함수라도 바인드 되어 있다면 true를 반환하는 IsBound() 함수 
	{
		OnCheckHit.Broadcast();
	}
}

