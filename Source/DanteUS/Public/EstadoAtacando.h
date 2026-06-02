// EstadoAtacando.h
// Este estado representa al enemigo cuando está en rango y ejecutando el ataque.
// Equivalente a CoinInsertedState.h del libro — el estado donde ocurre
// la acción principal (PullLever/Payout → aquí: golpear a Dante).
// Mientras este estado está activo, el enemigo NO persigue ni hace otra cosa.
// Solo espera a que termine la animación de ataque (FinalizarAtaque lo maneja).

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EstadoEnemigo.h"
#include "EstadoAtacando.generated.h"

UCLASS()
class DANTEUS_API AEstadoAtacando : public AActor, public IEstadoEnemigo
{
    GENERATED_BODY()

public:
    AEstadoAtacando();

private:
    // Referencia al enemigo dueño.
    // UPROPERTY() protege el puntero del garbage collector.
    UPROPERTY()
    class AEnemyBase* Enemigo;

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // ── Implementación de la interfaz ────────────────────────────────────────
    virtual void Ingresar() override;
    virtual void Ejecutar(float DeltaTime) override;
    virtual void Salir() override;
    virtual FString ToString() override;
    virtual void SetEnemigo(class AEnemyBase* MiEnemigo) override;
};