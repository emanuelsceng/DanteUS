// Fill out your copyright notice in the Description page of Project Settings.
// Dante: El Último Sello - Nivel 3
// Mini-Boss: El Guardián del Sello del Hambre

#pragma once
#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "MiniBossHambre.generated.h"

UENUM(BlueprintType)
enum class EEstadoMiniBoss : uint8
{
    Rondando        UMETA(DisplayName = "Rondando - Mantiene distancia"),
    Atacando        UMETA(DisplayName = "Atacando - Se detiene y golpea"),
    Retrocediendo   UMETA(DisplayName = "Retrocediendo - Dante muy cerca"),
    Invocando       UMETA(DisplayName = "Invocando - Spawneando enemigos"),
    Muerto          UMETA(DisplayName = "Muerto")
};

UCLASS()
class DANTEUS_API AMiniBossHambre : public AEnemyBase
{
    GENERATED_BODY()

public:
    AMiniBossHambre();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // Regenera +5 HP por golpe
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante|MiniBoss|Stats")
    float RegeneracionPorGolpe = 5.0f;

    // Distancia optima que mantiene con Dante
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante|MiniBoss|Movimiento")
    float DistanciaOptima = 400.0f;

    // Si Dante se acerca mas que esto, retrocede
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante|MiniBoss|Movimiento")
    float DistanciaMinima = 200.0f;

    // Cuantos enemigos invoca por vez (Limpiado el float fantasma de arriba)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante|MiniBoss|Invocacion")
    int32 CantidadInvocados = 2;

    // Blueprint del enemigo a invocar (asignar BP_SiervoVampirico en editor)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante|MiniBoss|Invocacion")
    TSubclassOf<AEnemyBase> ClaseEnemigoInvocado;

    // Blueprint del proyectil a lanzar (asignar BP_ProyectilPocion en editor)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante|MiniBoss|Ataque")
    TSubclassOf<AActor> ClaseProyectil;

    // Estado actual del Mini-Boss (visible en editor para debug)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dante|MiniBoss")
    EEstadoMiniBoss EstadoMiniBoss;

    // Montaje de animacion de ataque - asignar desde el editor
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante|MiniBoss|Animaciones")
    UAnimMontage* MontajeAtaque;
    // Montaje de animacion de aparicion
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante|MiniBoss|Animaciones")
    UAnimMontage* MontajeAparicion;

    // Montaje de animacion de muerte
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dante|MiniBoss|Animaciones")
    UAnimMontage* MontajeMuerte;

    virtual void AtacarJugador() override;
    virtual void Morir() override;
    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

private:
    FTimerHandle TimerInvocacion;
    FTimerHandle TimerRetroceder;

    void ActualizarMovimiento(float DeltaTime);
    void InvocarEnemigos();
    void Retroceder();
    void VolverARondear();
    void LanzarProyectil();
};