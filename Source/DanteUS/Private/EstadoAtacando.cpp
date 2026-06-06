
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


void AEstadoAtacando::Ingresar()
{
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red,
        TEXT("[Estado] Atacando: Ejecutando ataque"));

    if (Enemigo)
    {

        Enemigo->AtacarJugador();
    }
}
// EJECUTAR

void AEstadoAtacando::Ejecutar(float DeltaTime)
{
   
}
// SALIR
void AEstadoAtacando::Salir()
{
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red,
        TEXT("[Estado] Atacando: Saliendo del ataque"));
}
// TO STRING
FString AEstadoAtacando::ToString()
{
    return TEXT("Estado: Atacando");
}