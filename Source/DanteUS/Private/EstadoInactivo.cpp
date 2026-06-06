

#include "EstadoInactivo.h"
#include "EnemyBase.h"

AEstadoInactivo::AEstadoInactivo()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AEstadoInactivo::BeginPlay()
{
    Super::BeginPlay();
}

void AEstadoInactivo::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
 
}
void AEstadoInactivo::SetEnemigo(class AEnemyBase* MiEnemigo)
{
    Enemigo = MiEnemigo;
}

// INGRESAR
void AEstadoInactivo::Ingresar()
{
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::White,
        TEXT("[Estado] Inactivo: Enemigo en espera"));
}

// EJECUTAR

void AEstadoInactivo::Ejecutar(float DeltaTime)
{
    
}

// SALIR

void AEstadoInactivo::Salir()
{
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::White,
        TEXT("[Estado] Inactivo: Saliendo, enemigo activado"));
}

// TO STRING
FString AEstadoInactivo::ToString()
{
    return TEXT("Estado: Inactivo");
}