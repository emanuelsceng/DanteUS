// EstadoAtacando.cpp
// El estado donde el enemigo ejecuta su animación de ataque.
// Equivalente a CoinInsertedState.cpp del libro — donde ocurre PullLever() y Payout().
// La diferencia clave con el switch original:
// ANTES: el case Atacando estaba vacío y AtacarJugador() se llamaba desde
//        la transición en Persiguiendo.
// AHORA: Ingresar() dispara el ataque al ENTRAR al estado,
//        Ejecutar() está vacío (el timer de AtacarJugador maneja el resto),
//        y FinalizarAtaque() en EnemyBase hace la transición de vuelta.

#include "EstadoAtacando.h"
#include "EnemyBase.h"

AEstadoAtacando::AEstadoAtacando()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AEstadoAtacando::BeginPlay()
{
    Super::BeginPlay();
}

void AEstadoAtacando::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    // Vacío: igual que los otros estados, la lógica va en Ejecutar().
}

// SET ENEMIGO
// 
void AEstadoAtacando::SetEnemigo(class AEnemyBase* MiEnemigo)
{
    Enemigo = MiEnemigo;
}

// INGRESAR
// 
// Este es el método más importante de este estado.
// Se llama UNA VEZ cuando el enemigo ENTRA a Atacando.
// Aquí disparamos AtacarJugador() inmediatamente.
//
// ¿Por qué en Ingresar() y no en Ejecutar()?
// Porque el ataque se dispara UNA vez al entrar, no cada frame.
// Ejecutar() se llama cada frame — si llamáramos AtacarJugador() ahí,
// lanzaría montajes y timers infinitamente.
//
// Equivalente al libro donde CoinInsertedState::PullLever() ejecuta
// la acción principal cuando se acciona la palanca.
void AEstadoAtacando::Ingresar()
{
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red,
        TEXT("[Estado] Atacando: Ejecutando ataque"));

    if (Enemigo)
    {
        // AtacarJugador() reproduce el montaje de animación y
        // setea el timer que llama a FinalizarAtaque() en 1.5 segundos.
        // FinalizarAtaque() en EnemyBase llama SetEstado(EstadoPersiguiendo),
        // cerrando el ciclo: Atacando → espera 1.5s → Persiguiendo.
        //
        // IMPORTANTE: como AtacarJugador() es virtual en EnemyBase,
        // si el enemigo es un AcechadorSanguinario, se ejecutará
        // SU versión sobrescrita (con regeneración vampírica).
        // El polimorfismo hace todo el trabajo automáticamente.
        Enemigo->AtacarJugador();
    }
}
// EJECUTAR
//
// Intencionalmente vacío mientras dura el ataque.
// El enemigo está "bloqueado" en la animación.
// El timer de AtacarJugador() se encarga de la transición de vuelta.
// Equivalente al libro donde CoinInsertedState simplemente espera
// el resultado del Payout antes de cambiar de estado.
void AEstadoAtacando::Ejecutar(float DeltaTime)
{
    // Vacío a propósito.
    // El enemigo no persigue ni hace nada mientras ataca.
    // El timer en EnemyBase::AtacarJugador() maneja la salida de este estado.
}
// SALIR
//
// Se llama cuando el enemigo termina de atacar y vuelve a perseguir,o cuando recibe daño letal y transiciona a Muerto.
void AEstadoAtacando::Salir()
{
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red,
        TEXT("[Estado] Atacando: Saliendo del ataque"));
}
// TO STRING
//
FString AEstadoAtacando::ToString()
{
    return TEXT("Estado: Atacando");
}