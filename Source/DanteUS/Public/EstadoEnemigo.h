// EstadoEnemigo.h
// PASO 1: Esta es la INTERFAZ que define el "contrato" que todos los estados deben cumplir.
// Es el equivalente exacto al IState.h del libro (página del capítulo State Pattern).
// En el libro: "class DESIGN_PATTERNS_API IState" con sus pure virtual functions.

#pragma once
#include "CoreMinimal.h"
// Interface.h: es OBLIGATORIO para que Unreal reconozca esto como una interfaz UE5
#include "UObject/Interface.h"
#include "EstadoEnemigo.generated.h"

// BLOQUE 1: La clase U (UEstadoEnemigo)
// En Unreal, toda interfaz NECESITA dos clases:
//   - La clase U: es solo un "marcador" para el sistema de Unreal (reflection). NO la tocas, NO la modificas, exactamente igual al libro:
//   "//This class does not need to be modified."
//   - La clase I: es donde defines los métodos reales de la interfaz.
//
// UINTERFACE(MinimalAPI): le dice a Unreal que esta es una interfaz.
// MinimalAPI significa que se exporta con el mínimo necesario.
UINTERFACE(MinimalAPI)
class UEstadoEnemigo : public UInterface
{
    // GENERATED_BODY(): macro de Unreal que genera código interno automáticamente.
    // Sin esto, el compilador lanza error. Igual que en el libro.
    GENERATED_BODY()
};

// BLOQUE 2: La clase I (IEstadoEnemigo) — AQUÍ es donde trabajamos

// Esta es la interfaz REAL. Equivalente exacto a "class IState" del libro.
// La "I" al inicio es la convención de Unreal para interfaces (como la "A" para Actors).
// DANTEUS_API: macro que permite que esta clase sea visible en todo el proyecto.
class DANTEUS_API IEstadoEnemigo
{
    GENERATED_BODY()

public:

    // Método: Ingresar() 
    // Se llama UNA VEZ cuando el enemigo ENTRA a este estado.
    // "= 0" significa que es VIRTUAL PURA: todas las clases que implementen
    virtual void Ingresar() = 0;

    // Método: Ejecutar(float DeltaTime) 
    // Se llama CADA FRAME desde el Tick() de EnemyBase.
    // DeltaTime: tiempo transcurrido desde el frame anterior (necesario para IA y movimiento).
    // Aquí va la lógica principal de cada estado (perseguir, atacar, etc.)
    virtual void Ejecutar(float DeltaTime) = 0;

    // Método: Salir() 
    // Se llama UNA VEZ cuando el enemigo SALE de este estado.
    // Ejemplo: al salir de "Atacando", aquí limpias timers o animaciones.
    virtual void Salir() = 0;

    // Método: ToString() 
    // Retorna el nombre del estado como texto (FString es el String de Unreal).
    // Útil para debug en pantalla
    virtual FString ToString() = 0;

    // Método: SetEnemigo(AEnemyBase*)
    // Le pasa al estado una referencia a su dueño (el enemigo). Sin esto, el estado no sabría a QUIÉN mover, atacar o matar.
    // Equivalente al "SetSlotMachine()" del libro:
    // "virtual void SetSlotMachine(class AOldSchoolSlotMachine* SlotMachine) = 0;"
    //
    // "class AEnemyBase*": usamos forward declaration (declaración adelantada).
    // Esto le dice al compilador "AEnemyBase existe" sin incluir el .h completo,
    // evitando dependencias circulares (EstadoEnemigo.h ? EnemyBase.h).
    virtual void SetEnemigo(class AEnemyBase* Enemigo) = 0;
};