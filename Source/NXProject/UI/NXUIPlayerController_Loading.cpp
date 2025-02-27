// NXUIPlayerController_Loading.cpp

#include "UI/NXUIPlayerController_Loading.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"

void ANXUIPlayerController_Loading::BeginPlay()
{
	Super::BeginPlay();

    AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this);
    if (IsValid(GameMode))
    {
        // "NextLevel=" �ɼǿ��� ���� �̸� ��������
        FString NextLevelString = UGameplayStatics::ParseOption(GameMode->OptionsString, TEXT("NextLevel"));

        // ���� ��ȿ�ϸ� �ش� ������ �̵�
        if (!NextLevelString.IsEmpty())
        {
            UE_LOG(LogTemp, Log, TEXT("Next Level: %s"), *NextLevelString);
            UGameplayStatics::OpenLevel(this, *NextLevelString);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("NextLevel option not found!"));
        }
    }
}