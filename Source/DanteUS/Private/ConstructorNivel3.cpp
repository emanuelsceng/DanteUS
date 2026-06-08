// Fill out your copyright notice in the Description page of Project Settings.


#include "ConstructorNivel3.h"
#include "NivelDante.h"
#include "Engine/World.h"
// Sets default values
AConstructorNivel3::AConstructorNivel3()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	// Crea el producto vacío
	
}

// Called when the game starts or when spawned
void AConstructorNivel3::BeginPlay()
{
	Super::BeginPlay();
	// Crea el producto vacío

	Nivel = GetWorld()->SpawnActor<ANivelDante>(ANivelDante::StaticClass());
	Nivel->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	
}

void AConstructorNivel3::ConstruirConfiguracionBase()
{
    if (!Nivel) return;
    Nivel->SetNombreNivel(TEXT("Nivel 3 - El Hambre"));
    Nivel->SetConfiguracionBase(TEXT("Castillo corrupto, energia vampirica, sombras"));
    Nivel->SetNombreMapa(FName("Nivel3_ArenaComunes"));
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
        TEXT("Builder Nivel3: Config base construida."));
}

void AConstructorNivel3::ConstruirEnemigosComunes()
{
    if (!Nivel) return;
    Nivel->AgregarEnemigoComun(TEXT("AcechadorSanguinario"));
    Nivel->AgregarEnemigoComun(TEXT("GuardianVampirico"));
    Nivel->AgregarEnemigoComun(TEXT("SiervoVampirico"));
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
        TEXT("Builder Nivel3: Enemigos comunes configurados."));
}

void AConstructorNivel3::ConstruirMiniBoss()
{
    if (!Nivel) return;
    Nivel->SetDatosMiniBoss(TEXT("MiniBossHambre"));
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
        TEXT("Builder Nivel3: MiniBoss configurado."));
}

void AConstructorNivel3::ConstruirBoss()
{
    if (!Nivel) return;
    Nivel->SetDatosBoss(TEXT("Por implementar"));
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
        TEXT("Builder Nivel3: Boss pendiente."));
}
ANivelDante* AConstructorNivel3::ObtenerNivel()
{
    return Nivel;
}
