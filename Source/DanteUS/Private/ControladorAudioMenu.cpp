// Fill out your copyright notice in the Description page of Project Settings.


#include "ControladorAudioMenu.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Sound/SoundBase.h"

AControladorAudioMenu::AControladorAudioMenu()
{
	PrimaryActorTick.bCanEverTick = false;

	// 1. CARGAMOS EL AUDIO DESDE EL DISCO 
	static ConstructorHelpers::FObjectFinder<USoundBase> SonidoFondo(TEXT("/Script/Engine.SoundWave'/Game/MapsLvl/MapaMenuPrincipal/Vordt-of-the-Boreal-Valley.Vordt-of-the-Boreal-Valley'"));

	if (SonidoFondo.Succeeded())
	{
		MusicaMenuFondo = SonidoFondo.Object;
	}
}

void AControladorAudioMenu::InicializarSubsistema()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%s: Conectado al motor de audio."), *ObtenerNombreSubsistema()));
}

void AControladorAudioMenu::EjecutarComandoMenu(const FString& Comando)
{
	// 2. ESCUCHAMOS LA ORDEN DEL FACADE
	if (Comando.Equals("Reproducir_Musica_Fondo"))
	{
		if (MusicaMenuFondo)
		{
			// Reproducimos la música en 2D 
			UGameplayStatics::PlaySound2D(this, MusicaMenuFondo);

			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Audio: Reproduciendo pista principal."));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ControladorAudioMenu: No se encontro el archivo de musica. Revisa la ruta."));
		}
	}
}

void AControladorAudioMenu::ApagarSubsistema()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Audio: Haciendo Fade Out a la musica."));
}

FString AControladorAudioMenu::ObtenerNombreSubsistema()
{
	return "Controlador de Audio del Menu";
}