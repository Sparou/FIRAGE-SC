// Firage game:

#include "Weapon/STURifleWeapon.h"
#include "Engine/World.h"
#include "Weapon/Components/STUWeaponFXComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "ExtendedPlayerCharacter.h"
#include "STUAICharacter.h"
#include "STUAIController.h"
#include "Components/AudioComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Perception/AISense_Hearing.h"

DEFINE_LOG_CATEGORY_STATIC(LogRifleWeapon, All, All)

ASTURifleWeapon::ASTURifleWeapon()
{
    WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");
}

void ASTURifleWeapon::StartFire()
{
    Super::StartFire();
    InitFX();

    UE_LOG(LogRifleWeapon, Display, TEXT("Current Bullet Spread = %f"), CurrentBulletSpreadModifier)
    
    GetWorldTimerManager().ClearTimer(DecreaseBulletSpreadTimerHandle);
    GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTURifleWeapon::MakeShot, TimeBetweenShots, true);

    if (BulletSpread)
    {
        GetWorldTimerManager().SetTimer(IncreaseBulletSpreadTimerHandle, this, &ASTURifleWeapon::IncreaseBulletSpreadModifier,
            BulletSpreadIncreaseTime, true);
    }

    MakeShot();
}

void ASTURifleWeapon::StopFire()
{
    GetWorldTimerManager().ClearTimer(ShotTimerHandle);

    if (BulletSpread)
    {
        GetWorldTimerManager().ClearTimer(IncreaseBulletSpreadTimerHandle);
        GetWorldTimerManager().SetTimer(ResetBulletSpreadTimerHandle, this, &ASTURifleWeapon::ResetBulletSpreadModifier,
            BulletSpreadResetTime, false);
    }

    SetFXActive(false);
}

void ASTURifleWeapon::ResetBulletSpreadModifier()
{
    GetWorldTimerManager().ClearTimer(ResetBulletSpreadTimerHandle);
    GetWorldTimerManager().SetTimer(DecreaseBulletSpreadTimerHandle, this, &ASTURifleWeapon::DecreaseBulletSpreadModifier,
        BulletSpreadDecreaseTime, true);
}

void ASTURifleWeapon::IncreaseBulletSpreadModifier()
{
    if (CurrentBulletSpreadModifier < MaxBulletSpreadModifier)
    {
        CurrentBulletSpreadModifier += BulletSpreadIncreaseTime;
    }
    else
    {
        GetWorldTimerManager().ClearTimer(IncreaseBulletSpreadTimerHandle);
    }
}

void ASTURifleWeapon::DecreaseBulletSpreadModifier()
{
    if (CurrentBulletSpreadModifier > InitialBulletSpreadModifier)
    {
        CurrentBulletSpreadModifier -= BulletSpreadDecreaseTime;
    }
    else
    {
        GetWorldTimerManager().ClearTimer(DecreaseBulletSpreadTimerHandle);
    }
}

void ASTURifleWeapon::MakeShot()
{
    if (!GetWorld() || IsAmmoEmpty() || Cast<ASTUBaseCharacter>(GetOwner())->IsRunning())
    {
        StopFire();
        return;
    }

    const auto OwnerActor = Cast<ACharacter>(GetOwner());
    if (OwnerActor && OwnerActor->IsPlayerControlled())
    {
        if (const auto OwnerActorController = OwnerActor->GetController())
        {
            const auto CurrentControllerRotation = OwnerActorController->GetControlRotation();
            const auto PitchModifier = FMath::RandRange(0.0, RecoilModifier.Pitch);
            const auto YawModifier = FMath::RandRange(-RecoilModifier.Yaw, RecoilModifier.Yaw);
            OwnerActorController->SetControlRotation(CurrentControllerRotation + FRotator(PitchModifier, YawModifier, 0.0f));
        }
    }

    FVector TraceStart, TraceEnd;             // Основный трейс по которому будут считаться попадания
    FVector MuzzleTraceStart, MuzzleTraceEnd; // Дополнительный трейс, блокирующий стрельбу через стены

    MuzzleTraceStart = GetMuzzleWorldLocation();
    if (!GetTraceData(TraceStart, TraceEnd))
    {
        StopFire();
        return;
    }

    GetTraceData(MuzzleTraceStart, MuzzleTraceEnd);

    FHitResult HitResult;
    FHitResult MuzzleHitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);
    MakeHit(MuzzleHitResult, MuzzleTraceStart, MuzzleTraceEnd);

    FVector TraceFXEnd = TraceEnd;
    if (MuzzleHitResult.bBlockingHit &&                     // Проверка дополнительного трейса на блок
        MuzzleHitResult.Distance < 350.0f &&                // Проверка на валидность дистанции 
        HitResult.GetActor() != MuzzleHitResult.GetActor()) // Проверка на идентичность акторов
    {
        TraceFXEnd = MuzzleHitResult.ImpactPoint;
        MakeDamage(MuzzleHitResult);
        WeaponFXComponent->PlayImpactFX(MuzzleHitResult);
    }
    else if (HitResult.bBlockingHit)
    {
        TraceFXEnd = HitResult.ImpactPoint;
        MakeDamage(HitResult);
        WeaponFXComponent->PlayImpactFX(HitResult);
    }
    SpawnTraceFX(GetMuzzleWorldLocation(), TraceFXEnd);
    DecreaseAmmo();
}

void ASTURifleWeapon::BeginPlay()
{
    Super::BeginPlay();
    check(WeaponFXComponent);
    CurrentBulletSpreadModifier = InitialBulletSpreadModifier;
}

void ASTURifleWeapon::InitFX()
{
    if (!MuzzleFXComponent)
    {
        MuzzleFXComponent = SpawnMuzzleFX();
    }
    if (!FireAudioComponent)
    {
        FireAudioComponent = UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh, MuzzleSocketName);
    }
    SetFXActive(true);
}

void ASTURifleWeapon::SetFXActive(bool IsActive)
{
    if (MuzzleFXComponent)
    {
        MuzzleFXComponent->SetPaused(!IsActive);
        MuzzleFXComponent->SetVisibility(IsActive, true);
    }

    if (FireAudioComponent)
    { 
        FireAudioComponent->SetPaused(!IsActive);
        UAISense_Hearing::ReportNoiseEvent(GetWorld(), GetMuzzleWorldLocation(), 1.0f, GetOwner(), 30000.0f, FName("Fire"));
    }
}

void ASTURifleWeapon::SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd)
{
    const auto TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, TraceStart);
    if (TraceFXComponent)
    {
        TraceFXComponent->SetNiagaraVariableVec3(TraceTargetName, TraceEnd);
    }
}

void ASTURifleWeapon::MakeDamage(FHitResult& HitResult)
{

    const auto DamagedActor = HitResult.GetActor();
    FPointDamageEvent PointDamageEvent;
    PointDamageEvent.HitInfo = HitResult;
    DamagedActor->TakeDamage(Damage, PointDamageEvent, GetController(), this);
}


AController* ASTURifleWeapon::GetController() const
{
    const auto Pawn = Cast<APawn>(GetOwner());
    return Pawn ? Pawn->GetController() : nullptr;
}

bool ASTURifleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

    TraceStart = ViewLocation;
    auto HalfRad = FMath::DegreesToRadians(CurrentBulletSpreadModifier);
    const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);

    const auto STUCharacter = Cast<ACharacter>(GetOwner());
    if (!STUCharacter) return false;

    if (!STUCharacter->IsPlayerControlled())
    {
        const auto AIController = Cast<ASTUAIController>(STUCharacter->GetController());
        if (!AIController) return false;
        HalfRad = FMath::DegreesToRadians(BulletSpread);
        if (const auto AimActor = Cast<ASTUBaseCharacter>(AIController->GetFocusOnActor()))
        {
            if (AimActor->IsRunning())
                TraceEnd = AimActor->GetActorLocation() + FVector(0, 0, 40) + FVector(FMath::FRand() * BotBulletSpread,
                               FMath::FRand() * BotBulletSpread, FMath::FRand() * BotBulletSpread);
            else
            {
                TraceEnd = AimActor->GetActorLocation() + FVector(0, 0, 60) + FVector(FMath::FRand() * BotBulletSpread,
                               FMath::FRand() * BotBulletSpread, FMath::FRand() * BotBulletSpread);
            }
        }
    }
    else
    {
        TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
    }

    return true;
}

void ASTURifleWeapon::Zoom(bool Enabled)
{
    const auto Controller = Cast<APlayerController>(GetController());
    if (!Controller || !Controller->PlayerCameraManager) return;

    if (Enabled)
    {
        DefaultCameraFOV = Controller->PlayerCameraManager->GetFOVAngle();
    }
    Controller->PlayerCameraManager->SetFOV(Enabled ? FOVZoomAngle : DefaultCameraFOV);
}
