#include "EnemigoEscupidor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

AEnemigoEscupidor::AEnemigoEscupidor()
{
	// Estadísticas del esbirro común
	SaludMaxima = 15.0f;
	Salud = SaludMaxima;
	DanoAtaque = 10.0f;
	DistanciaAtaque = 1500.0f;

	// Configuración del cargador para este enemigo específico
	TamanoPiscina = 5;
}

void AEnemigoEscupidor::AtacarJugador()
{
	ACharacter* Dante = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (Dante)
	{
		// --- NUEVO: HACER QUE EL CUERPO DEL ENEMIGO TE MIRE ---
		// 1. Calculamos el vector de dirección restando las posiciones (Destino - Origen)
		FVector DireccionHaciaDante = Dante->GetActorLocation() - GetActorLocation();

		// 2. Aislamos SOLO el eje Yaw (Rotación horizontal). 
		// Ponemos Pitch y Roll en 0.0f para que el enemigo no se incline hacia el suelo o vuele.
		FRotator RotacionMirada = FRotator(0.0f, DireccionHaciaDante.Rotation().Yaw, 0.0f);

		// En lugar de usar solo SetActorRotation(RotacionMirada); 
		// le añadimos ETeleportType::TeleportPhysics.
		// Esto le dice a Unreal: "Gíralo, y si choca un milímetro con la pared al girar, 
		// no lo trabes, simplemente reacomódalo suavemente".
		SetActorRotation(RotacionMirada, ETeleportType::TeleportPhysics);

		// -------------------------------------------------------

		// (El código de la bala que ya teníamos)
		FVector Origen = GetActorLocation() + (GetActorForwardVector() * 150.0f) + FVector(0.0f, 0.0f, 50.0f);
		FRotator RotacionDisparo = ((Dante->GetActorLocation() + FVector(0.0f, 0.0f, 50.0f)) - Origen).Rotation();

		float VelocidadBala = 850.0f;
		float GravedadBala = 0.0f;

		EjecutarDisparo(Origen, RotacionDisparo, VelocidadBala, GravedadBala);
	}

	// El Cooldown de 3.5 segundos que definimos para balancear el juego
	GetWorldTimerManager().SetTimer(TemporizadorAtaque, this, &AEnemyBase::FinalizarAtaque, 3.5f, false);
}