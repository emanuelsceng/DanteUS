// Fill out your copyright notice in the Description page of Project Settings.


#include "SistemaJuegoFacade.h"
#include "SubsistemaUI.h"
#include "SubsistemaAudio.h"
// Sets default values
ASistemaJuegoFacade::ASistemaJuegoFacade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ASistemaJuegoFacade::BeginPlay()
{
	Super::BeginPlay();
	// 1. Instanciamos los oficiales (Subsistemas)
	ASubsistemaUI* SistemaUI = GetWorld()->SpawnActor<ASubsistemaUI>(ASubsistemaUI::StaticClass());
	ASubsistemaAudio* SistemaAudio = GetWorld()->SpawnActor<ASubsistemaAudio>(ASubsistemaAudio::StaticClass());

	// 2. Los metemos a la lista de tripulación
	Subsistemas.Add(SistemaUI);
	Subsistemas.Add(SistemaAudio);
	
}
void ASistemaJuegoFacade::DanteDerrotado()
{
	Tareas.Empty();

	// Preparamos la lista de órdenes
	Tareas.Add("Mostrar_GameOver");
	Tareas.Add("Reproducir_Musica");

	// Ejecutamos las órdenes
	ProcesarTareas(Subsistemas, Tareas);
}

void ASistemaJuegoFacade::ProcesarTareas(TArray<ASubsistemaNivel*> MisSubsistemas, TArray<FString> MisTareas)
{
	// Bucle que recorre cada subsistema y le pasa las tareas
	for (ASubsistemaNivel* Subsistema : MisSubsistemas)
	{
		for (const FString& Tarea : MisTareas)
		{
			Subsistema->EjecutarTarea(Tarea);
		}
	}
}


