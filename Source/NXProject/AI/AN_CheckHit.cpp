#include "AI/AN_CheckHit.h"
#include "AI/NXNonPlayerCharacter.h"
#include "Player/NXPlayerCharacter.h"

void UAN_CheckHit::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (IsValid(MeshComp) == true)
	{
		ANXNonPlayerCharacter* AttackingCharacter = Cast<ANXNonPlayerCharacter>(MeshComp->GetOwner());
		if (IsValid(AttackingCharacter) == true)
		{
			AttackingCharacter->OnCheckHit();
		}
	}
}