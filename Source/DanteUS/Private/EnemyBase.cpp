// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Valores iniciales de salud para "Enemigo Común" 
	SaludMaxima = 20.0f; 
		Salud = SaludMaxima;
	DanoAtaque = 2.0f; 

}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Función que se activa cuando Dante golpea al enemigo
float AEnemyBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    // Restamos la salud
    Salud -= DamageToApply;

    // Si la salud llega a 0, ejecutamos la muerte
    if (Salud <= 0.0f)
    {
        Morir();
    }

    return DamageToApply;
}

void AEnemyBase::Morir()
{
    // Limpieza de memoria dinámica 
    // Esto hace que el enemigo desaparezca del nivel
    Destroy(); 
}
