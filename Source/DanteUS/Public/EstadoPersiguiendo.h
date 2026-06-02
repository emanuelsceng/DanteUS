// EstadoPersiguiendo.h
// Este estado representa al enemigo cuando YA vio a Dante y lo persigue.
// Equivalente a NoCoinState.h del libro — el estado "activo esperando acción".
// Aquí el enemigo mueve su IA hacia el jugador cada frame en Ejecutar().
// Cuando llega cerca, transiciona a EstadoAtacando.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EstadoEnemigo.h"
#include "EstadoPersiguiendo.generated.h"

UCLASS()
class DANTEUS_API AEstadoPersiguiendo : public AActor, public IEstadoEnemigo
{
    GENERATED_BODY()

public:
    AEstadoPersiguiendo();

private:
    // Referencia al enemigo dueño, igual que en EstadoInactivo.
    // UPROPERTY() protege el puntero del garbage collector de Unreal.
    UPROPERTY()
    class AEnemyBase* Enemigo;

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // ── Implementación completa de la interfaz ───────────────────────────────
    // Todos estos métodos son obligatorios porque la interfaz los declara "= 0".
    // Si falta uno, el compilador lanza error diciéndonos que la clase
    // es "abstracta" y no puede instanciarse.
    virtual void Ingresar() override;
    virtual void Ejecutar(float DeltaTime) override;
    virtual void Salir() override;
    virtual FString ToString() override;
    virtual void SetEnemigo(class AEnemyBase* MiEnemigo) override;
};