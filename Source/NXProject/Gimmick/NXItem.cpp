#include "Gimmick/NXItem.h"

ANXItem::ANXItem()
{
	// ƽ(�� ������ ����)�� �ʿ� �����Ƿ� ���ϴ�.
	PrimaryActorTick.bCanEverTick = false;
}

// �÷��̾ ������ ������ ������ �� ����
void ANXItem::OnItemOverlap(AActor* OverlapActor)
{
	// �⺻�� �� �Լ� - �� �ڽ� Ŭ�������� ����
}

// �÷��̾ ������ ������ ����� �� ����
void ANXItem::OnItemEndOverlap(AActor* OverlapActor)
{
	// �⺻�� �� �Լ� - �ʿ��ϴٸ� �ڽ� Ŭ�������� Ȱ��
}

// �������� ���(Activate)�Ǿ��� �� ����
void ANXItem::ActivateItem(AActor* Activator)
{
	// �⺻�� �� �Լ� - �ڽ� Ŭ�������� ����
}

// ������ ������ ��ȯ
FName ANXItem::GetItemType() const
{
	return ItemType;
}

// �������� �ı�(����)�ϴ� �Լ�
void ANXItem::DestroyItem()
{
	// AActor���� �����ϴ� Destroy() �Լ��� ��ü ����
	Destroy();
}

