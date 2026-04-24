// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Valores iniciales de salud para "Enemigo Común" 
	SaludMaxima = 20.0f; 
		Salud = SaludMaxima;
	DanoAtaque = 2.0f; 
    //distancia del ataque
    DistanciaAtaque = 150.0f;


    // Inicializamos el patrón de estado
    EstadoActual = EEstadoEnemigo::Inactivo;

    SensorVision = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("SensorVision"));
    SensorVision->SightRadius = 1500.0f;
    SensorVision->SetPeripheralVisionAngle(45.0f);


}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	

	// Vinculamos la función AlVerJugador al evento OnSeePawn del SensorVision
    if (SensorVision)
    {
        SensorVision->OnSeePawn.AddDynamic(this, &AEnemyBase::AlVerJugador);
    }

}




// 2. LA MÁQUINA DE ESTADOS EN ACCIÓN
void AEnemyBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Si está muerto, ignoramos todo (no hace nada)
    if (EstadoActual == EEstadoEnemigo::Muerto) return;

    // Comportamiento según el estado exacto
    switch (EstadoActual)
    {
    case EEstadoEnemigo::Inactivo:
        // Aquí el enemigo podría reproducir una animación de respirar o mirar a los lados
        break;

    case EEstadoEnemigo::Persiguiendo:
        if (ObjetivoActual)
        {
            float DistanciaADante = FVector::Dist(GetActorLocation(), ObjetivoActual->GetActorLocation());
            AAIController* ControladorIA = Cast<AAIController>(GetController());

            if (DistanciaADante <= DistanciaAtaque)
            {
                // Transición de estado: Alcanzamos a Dante
                EstadoActual = EEstadoEnemigo::Atacando;
                if (ControladorIA) ControladorIA->StopMovement();
                AtacarJugador();
            }
            else
            {
                // Sigue moviéndose hacia Dante
                if (ControladorIA) ControladorIA->MoveToActor(ObjetivoActual, DistanciaAtaque - 20.0f);
            }
        }
        break;

    case EEstadoEnemigo::Atacando:
        // En este estado, el enemigo está bloqueado haciendo la animación de ataque.
        // No persigue ni hace otra cosa hasta que el ataque termine.
        break;
    }
}

void AEnemyBase::AlVerJugador(APawn* JugadorVisto)
{
    // Solo reacciona si está inactivo (Patrón Observador)
    if (EstadoActual == EEstadoEnemigo::Inactivo && JugadorVisto != nullptr)
    {
        ObjetivoActual = JugadorVisto;
        EstadoActual = EEstadoEnemigo::Persiguiendo; // Transición de estado
    }
}

void AEnemyBase::AtacarJugador()
{
    if (ObjetivoActual)
    {
        UGameplayStatics::ApplyDamage(ObjetivoActual, DanoAtaque, GetController(), this, UDamageType::StaticClass());
    }

    // Cooldown del ataque: Vuelve a perseguir en 1.5 segundos
    GetWorldTimerManager().SetTimer(TemporizadorAtaque, this, &AEnemyBase::FinalizarAtaque, 1.5f, false);
}

void AEnemyBase::FinalizarAtaque()
{
    // Una vez que termina el golpe, el cerebro vuelve a la fase de persecución
    if (EstadoActual != EEstadoEnemigo::Muerto)
    {
        EstadoActual = EEstadoEnemigo::Persiguiendo;
    }
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
    //
    if (Salud <= 0.0f && EstadoActual != EEstadoEnemigo::Muerto)
    {
        EstadoActual = EEstadoEnemigo::Muerto; // Transición final
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
