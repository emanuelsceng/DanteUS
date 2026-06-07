// Fill out your copyright notice in the Description page of Project Settings.


#include "SistemaTransicionFacade.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
ASistemaTransicionFacade::ASistemaTransicionFacade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ASistemaTransicionFacade::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASistemaTransicionFacade::EjecutarTransicion(FName NombreMapa)
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
        TEXT("SistemaTransicion: Iniciando secuencia de transicion..."));

    // Paso 1: guardar progreso
    GuardarProgreso();

    // Paso 2: limpiar el nivel actual
    LimpiarNivelActual();

    // Paso 3: cargar el siguiente mapa
    CargarSiguienteMapa(NombreMapa);
}

void ASistemaTransicionFacade::GuardarProgreso()
{
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow,
        TEXT("SistemaTransicion: Guardando progreso..."));
    // TODO: aqui conectaras tu sistema de guardado cuando lo implementes
}

void ASistemaTransicionFacade::LimpiarNivelActual()
{
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange,
        TEXT("SistemaTransicion: Liberando memoria del nivel actual..."));
    // TODO: aqui destruyes actores innecesarios antes de cambiar de mapa
}

void ASistemaTransicionFacade::CargarSiguienteMapa(FName NombreMapa)
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
        FString::Printf(TEXT("SistemaTransicion: Cargando mapa -> %s"), *NombreMapa.ToString()));

    // OpenLevel es la forma directa en UE5 — sin pantalla de carga visible
    UGameplayStatics::OpenLevel(this, NombreMapa, true);
}

