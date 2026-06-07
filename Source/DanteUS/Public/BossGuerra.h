#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "BossGuerra.generated.h"

UENUM(BlueprintType)
enum class EEstadoBossGuerra : uint8
{
    Rondando,
    Atacando,
    Retrocediendo,
    Muerto
};

class UNiagaraSystem;
class AProyectilBase;

UCLASS()
class DANTEUS_API ABossGuerra : public AEnemyBase
{
    GENERATED_BODY()

public:
    ABossGuerra();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante|Boss")
    float DistanciaOptima;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante|Boss")
    float DistanciaMinima;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante|Boss")
    TSubclassOf<AProyectilBase> ClaseProyectil;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dante|Boss")
    EEstadoBossGuerra EstadoBoss;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante|Boss")
    UAnimMontage* MontajeAtaque;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante|Boss")
    UNiagaraSystem* FX_ExplosionCeniza;

    virtual void AtacarJugador() override;
    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

private:
    FTimerHandle TimerRetroceder;
    int32 ContadorGolpes = 0;

    void ActualizarMovimiento(float DeltaTime);
    void Retroceder();
    void VolverARondear();
    void LanzarProyectil();
    void DetonarExplosionRechazo();
};