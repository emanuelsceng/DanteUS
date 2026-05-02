// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFactory.h"
#include "EnemigoCeniza.h" // Incluimos tu clase de soldado explosivo
//poner los demas enemigos aqui

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
AEnemyBase* AEnemyFactory::CrearEnemigo(UObject* Contexto, ETipoEnemigo Tipo, FVector Posicion, FRotator Rotacion)
{
	// 3. VALIDACIÓN: Verificamos que el contexto (el mundo) sea válido para evitar crasheos
	if (!Contexto) return nullptr;

	UWorld* Mundo = Contexto->GetWorld();
	if (!Mundo) return nullptr;

	AEnemyBase* NuevoEnemigo = nullptr;

	// Configuración de Spawn: Ajusta la posición si hay colisiones para que no aparezcan dentro de paredes
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// 4. EL SELECTOR (Corazón del Factory): El switch decide qué "Producto" fabricar[cite: 1]
	switch (Tipo)
	{
	case ETipoEnemigo::Ceniza:
		// Fabricamos un Soldado de Ceniza
		NuevoEnemigo = Mundo->SpawnActor<AEnemigoCeniza>(AEnemigoCeniza::StaticClass(), Posicion, Rotacion, Params);
		break;

	case ETipoEnemigo::Peste_Escupidor:
		// Aquí su código de creación para el Nivel 2
		break;

	case ETipoEnemigo::Vampirico:
		// Aquí su código de creación para el Nivel 3
		break;

	case ETipoEnemigo::Fantasma:
		// Aquí su código de creación para el Nivel 4
		break;
	}

	// 5. RETORNO: Devolvemos el enemigo creado para que quien lo pidió pueda usarlo
	return NuevoEnemigo;
}

