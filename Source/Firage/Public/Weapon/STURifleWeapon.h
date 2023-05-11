// Firage game:

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeapon.h"
#include "STURifleWeapon.generated.h"

class USTUWeaponFXComponent;
class UNiagaraSystem;
class UNiagaraComponent;
class UAudioComponent;

UCLASS()
class Firage_API ASTURifleWeapon : public ASTUBaseWeapon
{
    GENERATED_BODY()

public:
    ASTURifleWeapon();

    virtual void StartFire() override;
    virtual void StopFire() override;

    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;
    virtual void Zoom(bool Enabled) override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spread")
    bool BulletSpread = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spread", meta = (EditCondition = "BulletSpread"))
    float InitialBulletSpreadModifier = 1.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spread", meta = (EditCondition = "BulletSpread"))
    float MaxBulletSpreadModifier = 6.0f;

    /* Через какое время будет увеличиваться CurrentBulletSpread */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spread", meta = (EditCondition = "BulletSpread"))
    float BulletSpreadIncreaseTime = 0.15f;

    /* Через какое время будет уменьшаться CurrentBulletSpread */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spread", meta = (EditCondition = "BulletSpread"))
    float BulletSpreadDecreaseTime = 0.15f;

    /* Через какое время после последнего выстрела обновится CurretBulletSpread */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spread", meta = (EditCondition = "BulletSpread"))
    float BulletSpreadResetTime = 1.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spread", meta = (EditCondition = "BulletSpread"))
    FRotator RecoilModifier = FRotator(0.1f, 0.1f, 0.0f);

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float TimeBetweenShots = 0.1f;

    //UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    //float BulletSpread = 0.15f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float BotBulletSpread = 100;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stat")
    float Damage = 10.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    USTUWeaponFXComponent* WeaponFXComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UNiagaraSystem* TraceFX;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FString TraceTargetName = "TraceTarget";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float FOVZoomAngle = 50.0f;

    virtual void MakeShot() override;

    virtual void BeginPlay() override;

private:
    FTimerHandle ShotTimerHandle;

    FTimerHandle IncreaseBulletSpreadTimerHandle;
    FTimerHandle DecreaseBulletSpreadTimerHandle;
    FTimerHandle ResetBulletSpreadTimerHandle;

    float CurrentBulletSpreadModifier;

    void ResetBulletSpreadModifier();
    void IncreaseBulletSpreadModifier();
    void DecreaseBulletSpreadModifier();


    UPROPERTY()
    UNiagaraComponent* MuzzleFXComponent;

    UPROPERTY()
    UAudioComponent* FireAudioComponent;

    void InitFX();
    void SetFXActive(bool IsActive);
    void SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd);
    void MakeDamage(FHitResult& HitResult);


    AController* GetController() const;

    float DefaultCameraFOV = 90.0f;

};
