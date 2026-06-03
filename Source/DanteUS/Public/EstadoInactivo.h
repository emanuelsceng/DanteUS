// EstadoInactivo.h
// Primera clase de estado concreta.
// Equivalente exacto a NoDollarsState.h del libro:
// "class DESIGN_PATTERNS_API ANoDollarsState : public AActor, public IState"
// Nosotros: AEstadoInactivo hereda de AActor Y de IEstadoEnemigo.
//
// Este estado representa al enemigo cuando AÚN NO ha visto al jugador.
// No hace nada en Ejecutar() — solo espera a que AlVerJugador() lo despierte.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// Incluimos la interfaz del Paso 1.
// Sin esto, el compilador no sabe qué es IEstadoEnemigo.
#include "EstadoEnemigo.h"

#include "EstadoInactivo.generated.h"

UCLASS()
class DANTEUS_API AEstadoInactivo : public AActor, public IEstadoEnemigo
{
    GENERATED_BODY()

public:
    AEstadoInactivo();

private:
    // Puntero al enemigo dueño de este estado.
    // Equivalente exacto al libro:
    // "class AOldSchoolSlotMachine* OldSchoolSlotMachine;"
    // Con UPROPERTY() para que Unreal no destruya la referencia (garbage collection).
    // Es un puntero crudo (raw pointer) porque AEnemyBase es un AActor
    // y Unreal gestiona su ciclo de vida automáticamente.
    UPROPERTY()
    class AEnemyBase* Enemigo;

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // ── Implementación de la interfaz IEstadoEnemigo ─────────────────────────
    // "virtual void override" significa que estamos cumpliendo el contrato
    // de la interfaz. El compilador lanzará error si falta alguno.
    // Equivalente al libro donde cada estado implementa todos los métodos de IState.

    // Ingresar(): cuando el enemigo entra a este estado.
    // En Inactivo no hace nada especial — el enemigo simplemente para.
    virtual void Ingresar() override;

    // Ejecutar(): llamado cada frame desde EnemyBase::Tick().
    // En Inactivo no hace nada — espera al sensor de visión.
    virtual void Ejecutar(float DeltaTime) override;

    // Salir(): cuando el enemigo sale de este estado.
    // En Inactivo tampoco necesita limpiar nada.
    virtual void Salir() override;

    // ToString(): para debug en pantalla, igual que el libro.
    // "virtual FString ToString() = 0;" → aquí lo implementamos.
    virtual FString ToString() override;

    // SetEnemigo(): recibe la referencia al dueño.
    // Equivalente exacto al libro:
    // "virtual void SetSlotMachine(AOldSchoolSlotMachine* SlotMachine) override;"
    virtual void SetEnemigo(class AEnemyBase* MiEnemigo) override;
};