// Fill out your copyright notice in the Description page of Project Settings.


#include "DirectorNivel.h"
#include "NivelDante.h"

ADirectorNivel::ADirectorNivel()
{
    PrimaryActorTick.bCanEverTick = false;
    ConstructorActual = nullptr;
}

void ADirectorNivel::SetConstructorNivel(AActor* Constructor)
{
    // Cast a la interfaz, igual que SetLodgingBuilder() del libro
    ConstructorActual = Cast<IConstructorNivel>(Constructor);
    if (!ConstructorActual)
    {
        UE_LOG(LogTemp, Error,
            TEXT("SetConstructorNivel(): El Actor no implementa IConstructorNivel."));
    }
}

void ADirectorNivel::ConstruirNivelCompleto()
{
    if (!ConstructorActual)
    {
        UE_LOG(LogTemp, Error,
            TEXT("ConstruirNivelCompleto(): ConstructorActual es NULL."));
        return;
    }
    // Los pasos, en orden — equivalente al cuerpo de ConstructLodging()
    ConstructorActual->ConstruirConfiguracionBase();
    ConstructorActual->ConstruirEnemigosComunes();
    ConstructorActual->ConstruirMiniBoss();
    ConstructorActual->ConstruirBoss();
}

ANivelDante* ADirectorNivel::ObtenerNivelConstruido()
{
    if (!ConstructorActual)
    {
        UE_LOG(LogTemp, Error, TEXT("ObtenerNivelConstruido(): ConstructorActual es NULL."));
        return nullptr;
    }
    return ConstructorActual->ObtenerNivel();
}



