// Fill out your copyright notice in the Description page of Project Settings.


#include "PuertaPasillo.h"
#include "SistemaTransicionFacade.h"
// Sets default values
APuertaPasillo::APuertaPasillo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ZonaTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("ZonaTrigger"));
	RootComponent = ZonaTrigger;
	ZonaTrigger->SetBoxExtent(FVector(200.f, 400.f, 150.f));
}

// Called when the game starts or when spawned
void APuertaPasillo::BeginPlay()
{
	Super::BeginPlay();
    // Creamos los estados como UObjects ligeros
    // NewObject es el equivalente a SpawnActor pero para UObject
    EstBloqueada = NewObject<UEstadoPuertaBloqueada>(this);
    EstBloqueada->SetPuerta(this);

    EstAbierta = NewObject<UEstadoPuertaAbierta>(this);
    EstAbierta->SetPuerta(this);

    // Estado inicial: bloqueado
    EstadoActual = EstBloqueada;

    ZonaTrigger->OnComponentBeginOverlap.AddDynamic(
        this, &APuertaPasillo::AlPisarTrigger);
	
}

void APuertaPasillo::AlPisarTrigger(UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->ActorHasTag("Player") && !bTransicionIniciada)
    {
        // Delegamos al estado actual — sin if/else, el State decide
        EstadoActual->AlPisarTrigger();
    }
}

void APuertaPasillo::OnArenaLimpia()
{
    // Observer recibe el aviso del ArenaManager
    EstadoActual->AlRecibirAviso();
}

void APuertaPasillo::CambiarAEstadoAbierto()
{
    EstadoActual = EstAbierta;
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
        TEXT("PuertaPasillo: Estado cambiado a ABIERTO."));
}

void APuertaPasillo::IniciarTransicion()
{
    if (!MiFacade || MapaDestino.IsNone()) return;

    bTransicionIniciada = true;
    MiFacade->EjecutarTransicion(MapaDestino);
}
