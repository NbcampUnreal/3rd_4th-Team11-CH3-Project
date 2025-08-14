#include "Items/BaseItem.h"
#include "Engine/DataTable.h"
#include "Items/ItemDataRow.h"
#include "TimerManager.h"
#include "Engine/World.h"

UBaseItem::UBaseItem()
{
    Quantity = 1;
	RemainingCooldown = 0.f;
	CooldownDuration = 0.f;
}

// UObject의 GetWorld() 메서드를 오버라이드하여 월드 정보를 반환
UWorld* UBaseItem::GetWorld() const
{
	if (const UObject* MyOuter = GetOuter())
	{
		return MyOuter->GetWorld();
	}
	return nullptr;
}

FItemDataRow UBaseItem::GetItemData() const
{
    if (ItemDataHandle.DataTable)
    {        const FItemDataRow* Row = ItemDataHandle.GetRow<FItemDataRow>(TEXT("UBaseItem::GetItemData"));
        if (Row)
        {            
			return *Row;
        }
    }
    // 기본값 또는 빈 구조체 반환
    return FItemDataRow();
}

bool UBaseItem::Use_Implementation(AActor* User)
{
	// 아이템 수량이 0 이하인 경우 사용 불가
	if(Quantity <= 0)
	{
		return false;
	}
    // 쿨타임 상태일 때
    if(bIsOnCooldown)
    {
        UE_LOG(LogTemp, Warning, TEXT("%s: Item is on cooldown. Remaining: %f"), *GetName(), RemainingCooldown);
        return false;
    }

    // 아이템 데이터 테이블에서 쿨타임 가져오기
    if(ItemDataHandle.DataTable && ItemDataHandle.RowName.IsValid())
    {        FItemDataRow* ItemRow = ItemDataHandle.DataTable->FindRow<FItemDataRow>(ItemDataHandle.RowName, TEXT("Use_Implementation"));
        if(ItemRow)
        {
            CooldownDuration = ItemRow->CooldownTime;
        }
    }

    if (CooldownDuration > 0.f)
    {
	    bIsOnCooldown = true;
		RemainingCooldown = CooldownDuration;
		if (GetWorld())
		{
			GetWorld()->GetTimerManager().SetTimer(
				CooldownTimerHandle,
				this,
				&UBaseItem::UpdateCooldown,
				0.1f, true
			);
			UE_LOG(LogTemp, Log, TEXT("%s: Started %f second cooldown."), *GetName(), CooldownDuration);
		}
		OnCooldownUpdate.Broadcast(RemainingCooldown, ItemDataHandle.RowName);
    }
    
    return true;
}

// 쿨타임 업데이트 함수
void UBaseItem::UpdateCooldown()
{
	// 0.1초마다 쿨타임 업데이트
	RemainingCooldown -= 0.1f;
	if (RemainingCooldown > 0.f)
	{
		UE_LOG(LogTemp, Log, TEXT("%s: Cooldown remaining: %f seconds"), *GetName(), RemainingCooldown);
		OnCooldownUpdate.Broadcast(RemainingCooldown, ItemDataHandle.RowName);
	}

	// 쿨타임이 0 이하가 되면 타이머를 중지하고 쿨타임을 초기화
	else
	{
		if (GetWorld())
		{
			GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);
		}
		ResetCooldown();
	}
}

// 쿨타임 해제 함수
void UBaseItem::ResetCooldown()
{
    bIsOnCooldown = false;
	RemainingCooldown = 0.f;
    UE_LOG(LogTemp, Log, TEXT("%s: Cooldown finished."), *GetName());
	OnCooldownUpdate.Broadcast(RemainingCooldown, ItemDataHandle.RowName);
}
