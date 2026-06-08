// Fill out your copyright notice in the Description page of Project Settings.


#include "ConstructorNivel2.h"
#include "NivelDante.h"
#include "Engine/World.h"
// Sets default values
AConstructorNivel2::AConstructorNivel2()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
}

// Called when the game starts or when spawned
void AConstructorNivel2::BeginPlay()
{
	Super::BeginPlay();
	// Crea el producto vacío
	Nivel = GetWorld()->SpawnActor<ANivelDante>(ANivelDante::StaticClass());
	Nivel->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
}

void AConstructorNivel2::ConstruirConfiguracionBase()
{
    if (!Nivel) return;
    Nivel->SetNombreNivel(TEXT("Nivel 2 - La Peste"));
    Nivel->SetConfiguracionBase(TEXT("Alcantarillado monumental, neblina verdosa, charcos de acido"));
    Nivel->SetNombreMapa(FName("Nivel2_ArenaComunes"));
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
        TEXT("Builder Nivel2: Config base construida."));
}

void AConstructorNivel2::ConstruirEnemigosComunes()
{
    if (!Nivel) return;
    Nivel->AgregarEnemigoComun(TEXT("EnemigoPustulento"));
    Nivel->AgregarEnemigoComun(TEXT("EnemigoEscupidor"));
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
        TEXT("Builder Nivel2: Enemigos comunes configurados."));
}

void AConstructorNivel2::ConstruirMiniBoss()
{
    if (!Nivel) return;
    Nivel->SetDatosMiniBoss(TEXT("MiniBossPeste"));
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
        TEXT("Builder Nivel2: MiniBoss configurado."));
}

void AConstructorNivel2::ConstruirBoss()
{
    if (!Nivel) return;
    Nivel->SetDatosBoss(TEXT("Por implementar"));
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
        TEXT("Builder Nivel2: Boss pendiente."));
}
ANivelDante* AConstructorNivel2::ObtenerNivel()
{
    return Nivel;
}