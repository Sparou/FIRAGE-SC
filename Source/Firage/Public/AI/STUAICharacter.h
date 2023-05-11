// Firage game:

#pragma once

#include "CoreMinimal.h"
#include "Player/STUBaseCharacter.h"
#include "STUAICharacter.generated.h"

class UWidgetComponent;
class UBehaviorTree;

UCLASS()
class Firage_API ASTUAICharacter : public ASTUBaseCharacter
{
    GENERATED_BODY()

public:
    ASTUAICharacter(const FObjectInitializer& ObjInit);

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    UBehaviorTree* BehaviorTreeAsset;

    virtual void Tick(float DeltaSeconds) override;
    virtual bool IsRunning() const override;
    void StartRun(bool IfWant) { WantToRun = IfWant; };

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UWidgetComponent* HealthWidgetComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    float HealthVisibilityDistance = 1000.0f;

    virtual void OnDeath() override;
    virtual void OnHealthChanged(float Health, float HealthDelta) override;
    virtual void BeginPlay() override;

private:
    void UpdateHealthWidgetVisibility();
    bool IsMovingForward() const;
    bool WantToRun = false;
};
