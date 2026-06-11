// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuPrincipalFacade.h"
#include "ControladorUIMenu.h"
#include "ControladorAudioMenu.h"
#include "ControladorNivelesMenu.h"
#include "Engine/World.h"
// Sets default values
AMenuPrincipalFacade::AMenuPrincipalFacade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AMenuPrincipalFacade::BeginPlay()
{
	Super::BeginPlay();
	// 1. Instanciamos dinámicamente las clases hijas específicas (Los oficiales de la nave)
	ASubsistemaMenu* ComponenteUI = GetWorld()->SpawnActor<AControladorUIMenu>(AControladorUIMenu::StaticClass());
	ASubsistemaMenu* ComponenteAudio = GetWorld()->SpawnActor<AControladorAudioMenu>(AControladorAudioMenu::StaticClass());
	ASubsistemaMenu* ComponenteNiveles = GetWorld()->SpawnActor<AControladorNivelesMenu>(AControladorNivelesMenu::StaticClass());

	// 2. REQUERIMIENTO DEL INGENIERO: Almacenamos los hijos en punteros del Padre Abstracto
	ListaSubsistemas.Add(ComponenteUI);
	ListaSubsistemas.Add(ComponenteAudio);
	ListaSubsistemas.Add(ComponenteNiveles);

	// Inicializamos todo el ecosistema al arrancar
	for (ASubsistemaMenu* Subsistema : ListaSubsistemas)
	{
		if (Subsistema)
		{
			Subsistema->InicializarSubsistema();
		}
	}
}


void AMenuPrincipalFacade::InicializarMenuInicio()
{
	ListaTareas.Empty();
	ListaTareas.Add("Mostrar_Pantalla_Principal");
	ListaTareas.Add("Reproducir_Musica_Fondo");

	DespacharTareas(ListaSubsistemas, ListaTareas);
}

void AMenuPrincipalFacade::PresionaronBotonPlay()
{
	ListaTareas.Empty();
	ListaTareas.Add("Cargar_Secuencia_Completa");// va al controlador niveles Menu

	DespacharTareas(ListaSubsistemas, ListaTareas);
}


void AMenuPrincipalFacade::PresionaronBotonCargarNivel(int NumeroNivel)
{
	ListaTareas.Empty();

	// Generamos el comando dinámico basado en la opción elegida en la UI
	FString ComandoNivel = FString::Printf(TEXT("Cargar_Nivel_%i"), NumeroNivel);
	ListaTareas.Add(ComandoNivel);

	DespacharTareas(ListaSubsistemas, ListaTareas);
}


// EL BUCLE DEL PATRÓN: Copiado idéntico a la función PerformTasks
void AMenuPrincipalFacade::DespacharTareas(TArray<ASubsistemaMenu*> MisSubsistemas, TArray<FString> MisTareas)
{
	for (ASubsistemaMenu* Subsistema : MisSubsistemas)
	{
		for (const FString& Tarea : MisTareas)
		{
			if (Subsistema)
			{
				Subsistema->EjecutarComandoMenu(Tarea);
			}
		}
	}
}


