// Fill out your copyright notice in the Description page of Project Settings.

#include "RecolectorAlmas.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"

ARecolectorAlmas::ARecolectorAlmas()
{
	SaludMaxima = 75.0f;
	Salud = SaludMaxima;
	DanoAtaque = 4.0f; // Daño físico base temporal
	DistanciaAtaque = 120.0f;

	bYaInvocoFantasmas = false;
	ContadorGolpesRecibidos = 0;

	// Reducir la velocidad máxima de caminata del fantasma
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
}

void ARecolectorAlmas::BeginPlay()
{
	Super::BeginPlay();

	// Reseteamos las variables al iniciar el nivel
	Salud = SaludMaxima;
	ContadorGolpesRecibidos = 0;
	bYaInvocoFantasmas = false;
}

float ARecolectorAlmas::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// 1. PRIMERO HACEMOS NUESTRA LÓGICA ANTES DE QUE ENEMYBASE NOS RESTE LA VIDA
	if (DamageAmount > 0 && EstadoActual != EEstadoEnemigo::Muerto)
	{
		ContadorGolpesRecibidos++;

		// Si Dante logra acertar el 7mo golpe y no se ha invocado antes
		if (ContadorGolpesRecibidos == 7 && !bYaInvocoFantasmas)
		{
			InvocarFantasmasErrantes();
		}
	}

	// 2. AHORA SÍ, DEJAMOS QUE LA CLASE PADRE HAGA SU TRABAJO (Restar salud y matar si llega a 0)
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ARecolectorAlmas::InvocarFantasmasErrantes()
{
	bYaInvocoFantasmas = true;

	if (ClaseFantasmaErrante && GetWorld())
	{
		// Obtenemos hacia dónde está mirando el jefe y cuál es su lado derecho
		FVector Adelante = GetActorForwardVector();
		FVector Derecha = GetActorRightVector();

		// El punto base será 200 centímetros justo enfrente del Recolector
		FVector PosicionBaseFrontal = GetActorLocation() + (Adelante * 200.0f);

		// Configuramos el Spawn para que forzosamente aparezcan aunque choquen un poco con el suelo
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		// Bucle para crear la barrera de 4 fantasmas
		for (int i = 0; i < 4; i++)
		{
			// Fórmula matemática para separarlos en fila: -150, -50, 50, 150 centímetros
			float OffsetLateral = (i - 1.5f) * 100.0f;

			// Posición final de cada fantasma en la fila
			FVector PosicionFinal = PosicionBaseFrontal + (Derecha * OffsetLateral);

			// Hacemos que nazcan mirando en la misma dirección que el Jefe (hacia Dante)
			FRotator RotacionMismaQueJefe = GetActorRotation();

			// ¡Spawneamos al Fantasma!
			GetWorld()->SpawnActor<AActor>(ClaseFantasmaErrante, PosicionFinal, RotacionMismaQueJefe, SpawnParams);
		}

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, TEXT("¡El Recolector ha levantado su barrera de fantasmas!"));
		}
	}
}