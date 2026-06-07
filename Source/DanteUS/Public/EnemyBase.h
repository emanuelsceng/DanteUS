// EnemyBase.h
// PASO 2: EnemyBase se convierte en el "CONTEXTO" del patrón State.
// Equivalente exacto a OldSchoolSlotMachine.h del libro.
// En el libro: la máquina de slots guarda punteros a cada estado y delega acciones.
// Aquí: EnemyBase gu
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

// Incluimos la interfaz que creamos en el Paso 1.
// Así EnemyBase conoce el tipo IEstadoEnemigo para declarar sus punteros.
#include "EstadoEnemigo.h"

#include "EnemyBase.generated.h"

// Forward declaration del componente de visión.
// Le decimos al compilador "esta clase existe" sin incluir su .h completo.
class UPawnSensingComponent;
class AEnemySpawner; // en ves de poner enemyspawner.h D

UCLASS()
class DANTEUS_API AEnemyBase : public ACharacter
{
    GENERATED_BODY()

public:
    AEnemyBase();
    // Función llamable desde Blueprint para saber si el enemigo está muerto.
    // Reemplaza la comparación con el UENUM que tenían los Animation Blueprints.
    // BlueprintPure: no necesita nodo de ejecución, devuelve solo un bool.
    UFUNCTION(BlueprintPure, Category = "Dante | Estado")
    bool EstasMuerto() const
    {
        // Verificamos también que EstadoMuerto exista (ya fue inicializado)
        // Si EstadoMuerto es null, significa que BeginPlay aún no corrió
        // y los estados no fueron creados con SpawnActor todavía.
        // En ese caso devolvemos false para no activar la animación de muerte prematuramente.
        if (!EstadoMuerto.GetObject()) return false;

        // GetObject() devuelve el UObject* del TScriptInterface para comparar punteros.
        // Si EstadoActual apunta al mismo objeto que EstadoMuerto, está muerto.
        return EstadoActual.GetObject() == EstadoMuerto.GetObject();
    }


    // ─── PROPIEDADES DEL ENEMIGO (sin cambios respecto a tu versión original) ───

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante | Enemigo")
    float Salud;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante | Enemigo")
    float SaludMaxima;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante | Enemigo")
    float DanoAtaque;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combate|Animacion")
    float DesfaseZMuerte = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combate|Animacion")
    float TiempoDesaparicion = 15.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dante | IA")
    UPawnSensingComponent* SensorVision;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante | IA")
    float DistanciaAtaque;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante | IA")
    class UAnimMontage* MontageAtaque;

    // ObjetivoActual: referencia al jugador detectado.
    // Es APawn* porque el jugador es un Pawn en Unreal.
    APawn* ObjetivoActual;

    FTimerHandle TemporizadorAtaque;

    // PATRÓN STATE
    UPROPERTY()
    TScriptInterface<IEstadoEnemigo> EstadoInactivo;

    UPROPERTY()
    TScriptInterface<IEstadoEnemigo> EstadoPersiguiendo;

    UPROPERTY()
    TScriptInterface<IEstadoEnemigo> EstadoAtacando;

    UPROPERTY()
    TScriptInterface<IEstadoEnemigo> EstadoMuerto;

    // ESTADO ACTUAL
    UPROPERTY()
    TScriptInterface<IEstadoEnemigo> EstadoActual;

    // SetEstado(): cambia el estado actual del enemigo.
    void SetEstado(TScriptInterface<IEstadoEnemigo> NuevoEstado);

    




    TScriptInterface<IEstadoEnemigo> GetEstadoInactivo()     const { return EstadoInactivo; }
    TScriptInterface<IEstadoEnemigo> GetEstadoPersiguiendo() const { return EstadoPersiguiendo; }
    TScriptInterface<IEstadoEnemigo> GetEstadoAtacando()     const { return EstadoAtacando; }
    TScriptInterface<IEstadoEnemigo> GetEstadoMuerto()       const { return EstadoMuerto; }

    // ─── MÉTODOS DE COMBATE (se mantienen, los usan los estados internamente) ─
    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
        class AController* EventInstigator, AActor* DamageCauser) override;
    virtual void Morir();
    virtual void AtacarJugador();
    void FinalizarAtaque();

    UFUNCTION()
    void AlVerJugador(APawn* JugadorVisto);

protected:
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable, Category = "Combate | IA")
    virtual void EjecutarGolpeMelee();

public:
    //para compilar esta mrd
    UFUNCTION(BlueprintCallable, Category = "Combate | IA")
    virtual void JugadorDerrotado();
    //


    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};



