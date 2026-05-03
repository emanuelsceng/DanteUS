// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFactory.h"


// Sets default values
AEnemyFactory::AEnemyFactory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// Desactivamos el Tick porque esta clase es puramente lógica, no necesita actualizarse cada frame
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AEnemyFactory::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyFactory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


//
AEnemyBase* AEnemyFactory::CrearEnemigo(UWorld* Mundo, ETipoEnemigo Tipo, FVector Posicion, FRotator Rotacion)
{
    if (!Mundo) return nullptr;

    UClass* ClaseParaSpawnear = nullptr;

    // El switch que el ingeniero espera ver
    switch (Tipo)
    {
    case ETipoEnemigo::Ceniza:
        // IMPORTANTE: Verifica que esta ruta sea la correcta en tu Content Browser
        ClaseParaSpawnear = LoadClass<AEnemyBase>(nullptr, TEXT("/Game/ThirdPerson/Blueprints/Enemies/Nivel1_Guerra/BP_EnemigoCeniza.BP_EnemigoCeniza_C"));
        break;

    case ETipoEnemigo::Peste_Escupidor:
        // Aquí pondrías la ruta del siguiente enemigo cuando lo tengas
        break;
    }

    if (ClaseParaSpawnear)
    {
        FActorSpawnParameters Params;
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        return Mundo->SpawnActor<AEnemyBase>(ClaseParaSpawnear, Posicion, Rotacion, Params);
    }

    return nullptr;
}