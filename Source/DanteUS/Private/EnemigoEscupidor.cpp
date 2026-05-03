#include "EnemigoEscupidor.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

AEnemigoEscupidor::AEnemigoEscupidor()
{
	// Configuramos las estadísticas para este tirador
	SaludMaxima = 20.0f; // Sigue siendo enemigo común de la horda
	Salud = SaludMaxima;

	DanoAtaque = 3.0f; // Las bolas de vómito quitan 3 de daño

	// ¡OJO AQUÍ! Cambiamos la distancia. 
	// Como ataca de lejos, su distancia de ataque es mucho mayor (800 unidades)
	DistanciaAtaque = 800.0f;
}

void AEnemigoEscupidor::AtacarJugador()
{
	// NOTA IMPORTANTE: Aquí NO ponemos "Super::AtacarJugador();".
	// Si lo pusiéramos, ejecutaría el golpe cuerpo a cuerpo de la clase base.
	// Al no ponerlo, estamos reemplazando el ataque físico por completo.

	if (ClaseProyectilVomito && ObjetivoActual)
	{
		// 1. Calculamos desde dónde sale el disparo (un poco adelante de su cara)
		FVector PosicionDisparo = GetActorLocation() + (GetActorForwardVector() * 80.0f);

		// 2. Apuntamos hacia donde está mirando el enemigo
		FRotator RotacionDisparo = GetActorRotation();

		// 3. Instanciamos el proyectil en el mundo
		GetWorld()->SpawnActor<AActor>(ClaseProyectilVomito, PosicionDisparo, RotacionDisparo);

		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("¡El Escupidor lanzó vómito!"));
	}

	// Reutilizamos el temporizador heredado para el tiempo de recarga (cooldown) del disparo
	GetWorldTimerManager().SetTimer(TemporizadorAtaque, this, &AEnemyBase::FinalizarAtaque, 2.0f, false);
}