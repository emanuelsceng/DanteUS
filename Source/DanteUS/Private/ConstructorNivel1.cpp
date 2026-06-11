// Fill out your copyright notice in the Description page of Project Settings.


#include "ConstructorNivel1.h"
#include "NivelDante.h"
#include "Engine/World.h"

// Sets default values
AConstructorNivel1::AConstructorNivel1()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AConstructorNivel1::BeginPlay()
{
	Super::BeginPlay();
	// Crea el producto vacío
	Nivel = GetWorld()->SpawnActor<ANivelDante>(ANivelDante::StaticClass());//crea la plantilla vacia
	Nivel->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);	
}

void AConstructorNivel1::ConstruirConfiguracionBase()
{
    if (!Nivel) { UE_LOG(LogTemp, Error, TEXT("ConstruirConfiguracionBase: Nivel NULL")); return; }
    Nivel->SetNombreNivel(TEXT("Nivel 1 - La Guerra"));
    Nivel->SetConfiguracionBase(TEXT("Ciudad en ruinas, incendios activos, lluvia de ceniza"));
    Nivel->SetNombreMapa(FName("Nivel1_ArenaComunes"));
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
        TEXT("Builder Nivel1: Config base construida."));
}

void AConstructorNivel1::ConstruirEnemigosComunes()
{
    if (!Nivel) return;
    Nivel->AgregarEnemigoComun(TEXT("EnemigoCeniza"));
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
        TEXT("Builder Nivel1: Enemigos comunes configurados."));
}

void AConstructorNivel1::ConstruirMiniBoss()
{
    if (!Nivel) return;
    Nivel->SetDatosMiniBoss(TEXT("MiniBossGuerra"));
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
        TEXT("Builder Nivel1: MiniBoss configurado."));
}

void AConstructorNivel1::ConstruirBoss()
{
    if (!Nivel) return;
    Nivel->SetDatosBoss(TEXT("BossGuerra"));
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
        TEXT("Builder Nivel1: Boss configurado."));
}
ANivelDante* AConstructorNivel1::ObtenerNivel()
{
    return Nivel;
}


