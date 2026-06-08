// Fill out your copyright notice in the Description page of Project Settings.


#include "ConstructorNivel4.h"
#include "NivelDante.h"
#include "Engine/World.h"
// Sets default values
AConstructorNivel4::AConstructorNivel4()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	// Crea el producto vacío
	
}

// Called when the game starts or when spawned
void AConstructorNivel4::BeginPlay()
{
	Super::BeginPlay();
	Nivel = GetWorld()->SpawnActor<ANivelDante>(ANivelDante::StaticClass());
	Nivel->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
}

void AConstructorNivel4::ConstruirConfiguracionBase()
{
    if (!Nivel) return;
    Nivel->SetNombreNivel(TEXT("Nivel 4 - La Muerte"));
    Nivel->SetConfiguracionBase(TEXT("Santuario olvidado, piedra negra, vortice violeta"));
    Nivel->SetNombreMapa(FName("Nivel4_ArenaComunes"));
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
        TEXT("Builder Nivel4: Config base construida."));
}

void AConstructorNivel4::ConstruirEnemigosComunes()
{
    if (!Nivel) return;
    Nivel->AgregarEnemigoComun(TEXT("FantasmaErrante"));
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
        TEXT("Builder Nivel4: Enemigos comunes configurados."));
}

void AConstructorNivel4::ConstruirMiniBoss()
{
    if (!Nivel) return;
    Nivel->SetDatosMiniBoss(TEXT("RecolectorAlmas"));
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
        TEXT("Builder Nivel4: MiniBoss configurado."));
}

void AConstructorNivel4::ConstruirBoss()
{
    if (!Nivel) return;
    Nivel->SetDatosBoss(TEXT("La Muerte - Por implementar"));
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
        TEXT("Builder Nivel4: Boss pendiente."));
}
ANivelDante* AConstructorNivel4::ObtenerNivel()
{
    return Nivel;
}