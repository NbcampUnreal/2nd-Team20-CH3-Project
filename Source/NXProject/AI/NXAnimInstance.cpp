#include "AI/NXAnimInstance.h"

void UNXAnimInstance::AnimNotify_CheckHit()
{
	if (OnCheckHit.IsBound() == true) 
	{
		OnCheckHit.Broadcast();
	}
}

