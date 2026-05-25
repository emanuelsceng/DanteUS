

#include "RecolectorAlmas.h"
#include "Engine/World.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"

ARecolectorAlmas::ARecolectorAlmas()
{
	PrimaryActorTick.bCanEverTick = true;

	SaludMaxima = 75.0f;
	Salud = SaludMaxima;
	DanoAtaque = 8.0f; // Daño del Nivel 4

	// Rangos del jefe
	DistanciaAtaque = 800.0f;
	DistanciaHuir = 350.0f;   // Si cruza esta línea, se da la vuelta y huye

	bYaInvocoFantasmas = false;
	bEstaHuyendo = false;
	ContadorGolpesRecibidos = 0;
}

void ARecolectorAlmas::BeginPlay()
{
	Super::BeginPlay();

	Salud = SaludMaxima;
	ContadorGolpesRecibidos = 0;
	bYaInvocoFantasmas = false;
	bEstaHuyendo = false;

	APawn* Dante = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (Dante)
	{
		ObjetivoActual = Dante;
	}
}

void ARecolectorAlmas::Tick(float DeltaTime)
{
	// Si está muerto, bloqueamos todo
	if (EstadoActual == EEstadoEnemigo::Muerto)
	{
		Super::Tick(DeltaTime);
		return;
	}

	if (ObjetivoActual)
	{
		float Distancia = FVector::Dist(GetActorLocation(), ObjetivoActual->GetActorLocation());

		// 1. LÓGICA DE HUIDA (Dante muy cerca y el jefe no está en media animación de disparo)
		if (Distancia < DistanciaHuir && EstadoActual != EEstadoEnemigo::Atacando)
		{
			bEstaHuyendo = true;

			AAIController* ControladorIA = Cast<AAIController>(GetController());
			if (ControladorIA)
			{
				// Calculamos la ruta de escape dando la espalda a Dante
				FVector DireccionHuir = GetActorLocation() - ObjetivoActual->GetActorLocation();
				DireccionHuir.Z = 0.0f;
				DireccionHuir.Normalize();

				FVector PuntoEscape = GetActorLocation() + (DireccionHuir * 600.0f);
				ControladorIA->MoveToLocation(PuntoEscape, 50.0f);
			}

			// TRUCO DE HERENCIA: Engañamos a EnemyBase poniéndolo inactivo 1 frame para que no interrumpa la huida
			EEstadoEnemigo EstadoTemporal = EstadoActual;
			EstadoActual = EEstadoEnemigo::Inactivo;
			Super::Tick(DeltaTime);
			EstadoActual = EstadoTemporal;

			// Salimos para no forzar la mirada y permitir que la animación de espalda funcione
			return;
		}
		else
		{
			bEstaHuyendo = false;
		}

		// 2. LÓGICA DE SEGUIR CON LA MIRADA (Solo si no está huyendo y no está disparando)
		if (!bEstaHuyendo && EstadoActual != EEstadoEnemigo::Atacando)
		{
			FVector DireccionADante = ObjetivoActual->GetActorLocation() - GetActorLocation();
			DireccionADante.Z = 0.0f;

			if (!DireccionADante.IsNearlyZero())
			{
				FRotator RotacionObjetivo = DireccionADante.Rotation();
				FRotator RotacionSuave = FMath::RInterpTo(GetActorRotation(), RotacionObjetivo, DeltaTime, 6.0f);
				SetActorRotation(RotacionSuave);
			}
		}
	}

	// 3. EJECUTAR EL CEREBRO NORMAL DE PERSECUCIÓN Y ATAQUE
	Super::Tick(DeltaTime);
}

float ARecolectorAlmas::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (DamageAmount > 0.0f && EstadoActual != EEstadoEnemigo::Muerto)
	{
		ContadorGolpesRecibidos++;

		if (ContadorGolpesRecibidos == 7 && !bYaInvocoFantasmas)
		{
			InvocarFantasmasErrantes();
		}
	}

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ARecolectorAlmas::AtacarJugador()
{
	if (!ObjetivoActual) return;

	// Forzamos un giro de encare exacto en el frame donde decide disparar
	FVector DireccionADante = ObjetivoActual->GetActorLocation() - GetActorLocation();
	DireccionADante.Z = 0.0f;
	SetActorRotation(DireccionADante.Rotation());

	if (MontageAtaque && GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(MontageAtaque);
	}

	FTimerHandle TimerProyectil;
	GetWorldTimerManager().SetTimer(TimerProyectil, this, &ARecolectorAlmas::LanzarMagiaOscura, 0.5f, false);

	GetWorldTimerManager().SetTimer(TemporizadorAtaque, this, &AEnemyBase::FinalizarAtaque, 2.0f, false);
}

void ARecolectorAlmas::LanzarMagiaOscura()
{
	if (!ClaseProyectilMagia || !ObjetivoActual) return;

	// El proyectil sale desde el frente del Recolector de Almas
	FVector PosicionSpawn = GetActorLocation() + (GetActorForwardVector() * 60.0f) + FVector(0.0f, 0.0f, 60.0f);

	FVector Direccion = ObjetivoActual->GetActorLocation() - PosicionSpawn;
	Direccion.Normalize();
	FRotator Rotacion = Direccion.Rotation();

	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.Instigator = GetInstigator();
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<AActor>(ClaseProyectilMagia, PosicionSpawn, Rotacion, Params);
}

void ARecolectorAlmas::InvocarFantasmasErrantes()
{
	bYaInvocoFantasmas = true;

	if (ClaseFantasmaErrante && GetWorld())
	{
		FVector Adelante = GetActorForwardVector();
		FVector Derecha = GetActorRightVector();
		FVector PosicionBaseFrontal = GetActorLocation() + (Adelante * 200.0f);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		for (int i = 0; i < 4; i++)
		{
			float OffsetLateral = (i - 1.5f) * 100.0f;
			FVector PosicionFinal = PosicionBaseFrontal + (Derecha * OffsetLateral);
			FRotator RotacionMismaQueJefe = GetActorRotation();

			GetWorld()->SpawnActor<AActor>(ClaseFantasmaErrante, PosicionFinal, RotacionMismaQueJefe, SpawnParams);
		}
	}
}