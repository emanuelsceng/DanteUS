// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "Engine/Engine.h"
// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
    // Creamos la caja de colisión y la hacemos la raíz del objeto
    ZonaActivacion = CreateDefaultSubobject<UBoxComponent>(TEXT("ZonaActivacion"));
    RootComponent = ZonaActivacion;
    ZonaActivacion->SetBoxExtent(FVector(200.0f, 200.0f, 100.0f));

    // Le decimos a la caja que nos avise cuando alguien entre
    ZonaActivacion->OnComponentBeginOverlap.AddDynamic(this, &AEnemySpawner::AlEntrarEnZona);
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpawner::AlEntrarEnZona(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    // Solo se activa si el que entra es el Jugador (Dante) y si no se ha usado ya
    if (!bYaSeActivo && OtherActor && OtherActor->ActorHasTag("Player"))
    {
        bYaSeActivo = true;

        for (int32 i = 0; i < Cantidad; i++)
        {
            // Cálculo para que aparezcan en fila o ligero arco
            FVector SpawnPos = GetActorLocation() + FVector(i * 150.0f, 0.0f, 50.0f);

            // usamos la fabrica pasándole la clase del Blueprint (ClaseEnemigoBP) para que tenga cuerpo y malla
            AEnemyFactory::CrearEnemigo(GetWorld(), TipoA_Spawnear, SpawnPos, GetActorRotation());
        }

        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("¡Emboscada de la Fábrica activada!"));
        // Ordenamos al motor que elimine este Spawner del mundo al final de este frame
        this->Destroy();
    }
}