// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaManager.h"

// Sets default values
AArenaManager::AArenaManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SpawnersActivos = 0;
}

// Called when the game starts or when spawned
void AArenaManager::BeginPlay()
{
	Super::BeginPlay();
    // Auto-suscribimos la PuertaPasillo si está asignada
    if (PuertaObjetivo)
    {
        TScriptInterface<IObservadorArena> Suscriptor;
        Suscriptor.SetObject(PuertaObjetivo);
        Suscriptor.SetInterface(Cast<IObservadorArena>(PuertaObjetivo));
        Suscribir(Suscriptor);
    }
}


void AArenaManager::RegistrarSpawners(int32 CantidadSpawners)
{
    SpawnersActivos = CantidadSpawners;
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
        FString::Printf(TEXT("ArenaManager: Monitoreando %d spawners."), SpawnersActivos));
}

void AArenaManager::Suscribir(TScriptInterface<IObservadorArena> Suscriptor)
{
    if (Suscriptor)
    {
        ListaSuscriptores.Add(Suscriptor);
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan,
            TEXT("ArenaManager: Nuevo suscriptor registrado."));
    }
}

void AArenaManager::NotificarOleadaLimpia()
{
    SpawnersActivos = FMath::Max(0, SpawnersActivos - 1);

    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow,
        FString::Printf(TEXT("ArenaManager: Spawners restantes: %d"), SpawnersActivos));

    // Solo notifica cuando TODOS los spawners están limpios
    if (SpawnersActivos == 0)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
            TEXT("ArenaManager: Todas las oleadas limpias — notificando suscriptores."));

        // Equivalente a NotifySubscribers() del libro
        for (TScriptInterface<IObservadorArena>& Suscriptor : ListaSuscriptores)
        {
            if (Suscriptor)
            {
                Suscriptor->OnArenaLimpia();
            }
        }
    }
}