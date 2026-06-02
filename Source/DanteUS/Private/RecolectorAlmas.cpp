

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

	// CAMBIO 1: El FantasmaErrante arrancaba directo en Persiguiendo.
	// El RecolectorAlmas original no cambiaba de estado en BeginPlay,
	// solo guardaba el objetivo. Mantenemos eso — arranca en Inactivo
	// y el sensor de visión de EnemyBase lo activará cuando vea a Dante.
	APawn* Dante = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (Dante)
	{
		ObjetivoActual = Dante;
		// Lo ponemos directo en Persiguiendo igual que el original
		// porque el RecolectorAlmas siempre persigue desde el inicio
		SetEstado(GetEstadoPersiguiendo());
	}
}

void ARecolectorAlmas::Tick(float DeltaTime)
{
	// Si está muerto, bloqueamos todo
	//cambio 1 (State) Antes (EstadoActual == EEstadoEnemigo::Muerto)
	if (EstadoActual.GetObject() == GetEstadoMuerto().GetObject())
	{
		// Llamamos al Tick del AActor directamente, saltando EnemyBase::Tick()
	   // para que el estado Muerto no ejecute lógica de IA.
		Super::Tick(DeltaTime);
		return;
	}

	if (ObjetivoActual)
	{
		float Distancia = FVector::Dist(GetActorLocation(), ObjetivoActual->GetActorLocation());

		// 1. LÓGICA DE HUIDA (Dante muy cerca y el jefe no está en media animación de disparo)
		//cambio2 (state) Antes && EEstadoEnemigo::Atacando
		if (Distancia < DistanciaHuir && EstadoActual.GetObject() != GetEstadoAtacando())
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

			    //cambio3 (state) 
				// Ahora simplemente llamamos al Tick del AActor base directamente,
				// saltando EnemyBase::Tick() que es quien movería la IA.
				// Así el RecolectorAlmas controla su propio movimiento de huida
				// sin que EnemyBase::Tick() lo interrumpa.
				AActor::Tick(DeltaTime);


			// Salimos para no forzar la mirada y permitir que la animación de espalda funcione
			return;
		}
		else
		{
			bEstaHuyendo = false;
		}

		// 2. LÓGICA DE SEGUIR CON LA MIRADA (Solo si no está huyendo y no está disparando)
		//cambio4 (state) antes && EstadoActual != EEstadoEnemigo::Atacando
		if (!bEstaHuyendo && EstadoActual.GetObject() != GetEstadoAtacando().GetObject())
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
	//cambio5 (state) antes && EstadoActual != EEstadoEnemigo::Muerto
	if (DamageAmount > 0.0f && EstadoActual.GetObject() != GetEstadoMuerto().GetObject())
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