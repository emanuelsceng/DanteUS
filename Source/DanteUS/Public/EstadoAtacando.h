
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