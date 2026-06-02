// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

// Incluimos la interfaz que creamos Así EnemyBase conoce el tipo IEstadoEnemigo para declarar sus punteros.
#include "EstadoEnemigo.h"
#include "EnemyBase.generated.h"


class UPawnSensingComponent;



UCLASS()
class DANTEUS_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();
    // PROPIEDADES DEL ENEMIGO (sin cambios respecto a tu versión original)

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

    /////////
    // Salud actual del enemigo
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante | Enemigo")
    float Salud;

    // Salud máxima (Sección 0.1: 20, 75 o 150 HP)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante | Enemigo")
    float SaludMaxima;

    // Daño que inflige este enemigo (2 puntos para comunes)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante | Enemigo")
    float DanoAtaque;
    // Ajuste de altura al morir para evitar que el cadáver traspase el suelo
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combate|Animacion")
    float DesfaseZMuerte = 0.0f;

    // Tiempo que el cadáver se queda en el suelo antes de llamar a Morir()
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combate|Animacion")
    float TiempoDesaparicion = 15.0f;

	// Componente para detectar a Dante
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dante | IA")
    UPawnSensingComponent* SensorVision;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante | IA")
    float DistanciaAtaque;

    // Referencia a la animación de ataque
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante | IA")
    class UAnimMontage* MontageAtaque;

    // ObjetivoActual: referencia al jugador detectado.
    // Es APawn* porque el jugador es un Pawn en Unreal.
    APawn* ObjetivoActual;
    FTimerHandle TemporizadorAtaque;

    //PATRÓN STATE: LOS 4 PUNTEROS DE ESTADO 
    // Equivalente exacto al libro:
    // "IState* NoDollarsState;"
    // "IState* NoCoinState;" etc.
    //
    // Cada puntero es de tipo IEstadoEnemigo* (la interfaz del Paso 1).
    // Esto es polimorfismo: el puntero puede apuntar a CUALQUIER clase
    // que implemente IEstadoEnemigo (Inactivo, Persiguiendo, Atacando, Muerto).
    //
    // UPROPERTY(): necesario para que Unreal gestione la memoria
    // y no destruya estos objetos inesperadamente (garbage collection).
    UPROPERTY()
    TScriptInterface<IEstadoEnemigo> EstadoInactivo;

    UPROPERTY()
    TScriptInterface<IEstadoEnemigo> EstadoPersiguiendo;

    UPROPERTY()
    TScriptInterface<IEstadoEnemigo> EstadoAtacando;

    UPROPERTY()
    TScriptInterface<IEstadoEnemigo> EstadoMuerto;

    // ESTADO ACTUAL
    // Este es el puntero que cambia según la situación del enemigo.
    // Equivalente al libro: "IState* State;"
    // SetEstado() lo cambia, Tick() lo usa para delegar.
    UPROPERTY()
    TScriptInterface<IEstadoEnemigo> EstadoActual;

    // MÉTODOS DEL CONTEXTO 

    // SetEstado(): cambia el estado actual del enemigo.
    // Equivalente al libro: "void SetState(IState* myState);"
    // Llama a Salir() del estado anterior y a Ingresar() del nuevo.
    void SetEstado(TScriptInterface<IEstadoEnemigo> NuevoEstado);

    // Getters de cada estado: permiten que las clases de estado
    // puedan pedirle al enemigo "dame el estado Persiguiendo" para transicionar.
    // Equivalente al libro: "IState* GetNoCoinState();" etc.
    TScriptInterface<IEstadoEnemigo> GetEstadoInactivo()     const { return EstadoInactivo; }
    TScriptInterface<IEstadoEnemigo> GetEstadoPersiguiendo() const { return EstadoPersiguiendo; }
    TScriptInterface<IEstadoEnemigo> GetEstadoAtacando()     const { return EstadoAtacando; }
    TScriptInterface<IEstadoEnemigo> GetEstadoMuerto()       const { return EstadoMuerto; }

    // MÉTODOS DE COMBATE (se mantienen, los usan los estados internamente)
    //
    // Función que se activa cuando Dante lo golpea (le quita 5 HP)
    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

    // Función para manejar la muerte y limpieza de memoria
    virtual void Morir();

	// Función que se activa cuando el enemigo ve a Dante
    UFUNCTION()
    void AlVerJugador(APawn* JugadorVisto);

    virtual void AtacarJugador();
    void FinalizarAtaque(); // Reemplaza a ResetearAtaque

 protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    UFUNCTION(BlueprintCallable, Category = "Combate | IA")
    virtual void EjecutarGolpeMelee();

 public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
