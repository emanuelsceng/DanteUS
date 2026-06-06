// Fill out your copyright notice in the Description page of Project Settings.


#include "ControladorNivelesMenu.h"
#include "ConstructorNivel1.h"
#include "ConstructorNivel2.h"
#include "ConstructorNivel3.h"
#include "ConstructorNivel4.h"
#include "DirectorNivel.h"
#include "NivelDante.h"
#include "Engine/World.h"

void AControladorNivelesMenu::InicializarSubsistema()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%s: Enlazado al sistema de carga."), *ObtenerNombreSubsistema()));
}

void AControladorNivelesMenu::EjecutarComandoMenu(const FString& Comando)
{
    // Crea el Director una sola vez por comando
    ADirectorNivel* Director = GetWorld()->SpawnActor<ADirectorNivel>(ADirectorNivel::StaticClass());
    if (!Director) return;

    AActor* ConstructorElegido = nullptr;

    if (Comando.Equals("Cargar_Nivel_1"))
    {
        ConstructorElegido = GetWorld()->SpawnActor<AConstructorNivel1>(AConstructorNivel1::StaticClass());
    }
    else if (Comando.Equals("Cargar_Nivel_2"))
    {
        ConstructorElegido = GetWorld()->SpawnActor<AConstructorNivel2>(AConstructorNivel2::StaticClass());
    }
    else if (Comando.Equals("Cargar_Nivel_3"))
    {
        ConstructorElegido = GetWorld()->SpawnActor<AConstructorNivel3>(AConstructorNivel3::StaticClass());
    }
    else if (Comando.Equals("Cargar_Nivel_4"))
    {
        ConstructorElegido = GetWorld()->SpawnActor<AConstructorNivel4>(AConstructorNivel4::StaticClass());
    }
    else if (Comando.Equals("Cargar_Secuencia_Completa"))
    {
        // Modo PLAY: construye el Nivel 1 para empezar
        ConstructorElegido = GetWorld()->SpawnActor<AConstructorNivel1>(AConstructorNivel1::StaticClass());
        GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Orange,
            TEXT("Niveles: Iniciando secuencia completa desde Nivel 1."));
    }

    if (ConstructorElegido)
    {
        // Inyectamos el builder concreto en el director — igual que el libro
        Director->SetConstructorNivel(ConstructorElegido);
        Director->ConstruirNivelCompleto();

        ANivelDante* NivelFinal = Director->ObtenerNivelConstruido();
        if (NivelFinal)
        {
            NivelFinal->ReportarCaracteristicasNivel();
            // TODO: aquí usarás UGameplayStatics::OpenLevel(this, NivelFinal->GetNombreMapa())
            // cuando tengas los mapas creados
        }
    }
}

void AControladorNivelesMenu::ApagarSubsistema()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Niveles: Limpiando cache de mapas previos."));
}

FString AControladorNivelesMenu::ObtenerNombreSubsistema()
{
	return "Controlador de Carga de Niveles";
}