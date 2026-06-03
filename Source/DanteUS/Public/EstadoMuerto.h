// EstadoMuerto.h
// El estado final e irreversible del enemigo.
// Equivalente a WonDollarsState.h del libro — el estado terminal
// donde ya no hay más transiciones posibles.
// Una vez aquí, el enemigo detiene toda IA, desactiva colisiones,
// ajusta la malla y programa su destrucción.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EstadoEnemigo.h"
#include "EstadoMuerto.generated.h"

UCLASS()
class DANTEUS_API AEstadoMuerto : public AActor, public IEstadoEnemigo
{
    GENERATED_BODY()

public:
    AEstadoMuerto();

private:
    // Referencia al enemigo dueño.
    // UPROPERTY() protege el puntero del garbage collector de Unreal.
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