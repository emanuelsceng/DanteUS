// Fill out your copyright notice in the Description page of Project Settings.


#include "ControladorAudioMenu.h"

void AControladorAudioMenu::InicializarSubsistema()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%s: Conectado al motor de audio."), *ObtenerNombreSubsistema()));
}

void AControladorAudioMenu::EjecutarComandoMenu(const FString& Comando)
{
	if (Comando.Equals("Reproducir_Musica_Fondo"))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Magenta, TEXT("Audio: Reproduciendo soundtrack epico de inicio."));
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