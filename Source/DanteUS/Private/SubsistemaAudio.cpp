// Fill out your copyright notice in the Description page of Project Settings.


#include "SubsistemaAudio.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "UObject/ConstructorHelpers.h"


ASubsistemaAudio::ASubsistemaAudio()
{
	PrimaryActorTick.bCanEverTick = false;

	// Buscamos el archivo de música en las carpetas de Unreal
	// Pega la referencia que copiaste dentro de los paréntesis del TEXT("")
	static ConstructorHelpers::FObjectFinder<USoundBase> ArchivoAudio(TEXT("/Game/ThirdPerson/Blueprints/Dante_Assets/Dark-Souls-_You-Died_-Animation-Sound-Effect-_4K_-_darksouls-_eldenring.Dark-Souls-_You-Died_-Animation-Sound-Effect-_4K_-_darksouls-_eldenring"));

	if (ArchivoAudio.Succeeded())
	{
		MusicaGameOver = ArchivoAudio.Object; // Si lo encuentra, lo guardamos
	}
}
void ASubsistemaAudio::EjecutarTarea(const FString& Tarea)
{
	if (Tarea.Equals("Reproducir_Musica"))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("%s: Reproduciendo música."), *GetNombreSubsistema()));
		// Si el archivo cargó bien, lo hacemos sonar en el juego
		if (MusicaGameOver)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), MusicaGameOver);
		}
	}

}

FString ASubsistemaAudio::GetNombreSubsistema()
{
	return "Manager de Audio";
}