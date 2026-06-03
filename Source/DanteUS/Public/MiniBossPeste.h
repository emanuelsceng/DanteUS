#pragma once

#include "CoreMinimal.h"
#include "EnemyRanged.h"
#include "MiniBossPeste.generated.h"

UCLASS()
class DANTEUS_API AMiniBossPeste : public AEnemyRanged
{
    GENERATED_BODY()

public:
    AMiniBossPeste();

    // Sobrescribimos el daño para controlar el contador de golpes
    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

    // Sobrescribimos el ataque para asegurar que solo pegue de cerca
    virtual void AtacarJugador() override;

    // Eventos para el Blueprint (Visuales)
    UFUNCTION(BlueprintImplementableEvent, Category = "Dante | Boss | Eventos")
    void OnAtaqueCortaDistanciaBlueprint();

    UFUNCTION(BlueprintImplementableEvent, Category = "Dante | Boss | Eventos")
    void OnPrepararVomitoBlueprint();

protected:
    virtual void BeginPlay() override;

    // Variables de configuración
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante | Boss | Balance")
    int32 LimiteGolpesParaVomitar;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante | Boss | Balance")
    int32 CantidadVomitosPorRafaga;

    // Estado del Boss
    int32 ContadorGolpesRecibidos;
    int32 ContadorVomitosLanzados;
    bool bEstaEjecutandoHabilidad;

    // Timers
    FTimerHandle TemporizadorPreparacion;
    FTimerHandle TemporizadorRafaga;
    FTimerHandle TemporizadorRecuperacion;
    FTimerHandle TemporizadorZarpazo;

    // Funciones de habilidad
    void IniciarPreparacionVomito();
    void DispararVomitoSecuencial();
    void FinalizarRecuperacion();
    void DespertarCerebroBoss();
};