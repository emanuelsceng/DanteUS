#include "MiniBossPeste.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "AIController.h"

AMiniBossPeste::AMiniBossPeste()
{
	SaludMaxima = 1000.0f;
	Salud = SaludMaxima;
	DanoAtaque = 15.0f;
	DistanciaAtaque = 150.0f;
	TamanoPiscina = 12;

	VelocidadMovimientoJefe = 180.0f;
	LimiteGolpesParaVomitar = 5;
	ContadorGolpesRecibidos = 0;
	bEstaEjecutandoHabilidad = false;

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = VelocidadMovimientoJefe;
	}
}

void AMiniBossPeste::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(TemporizadorRadarIA, this, &AMiniBossPeste::RutinaRadarPersecucion, 0.5f, true);
}

void AMiniBossPeste::RutinaRadarPersecucion()
{
	if (!bEstaEjecutandoHabilidad && EstadoActual == EEstadoEnemigo::Persiguiendo)
	{
		ACharacter* Dante = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		if (Dante) { AlVerJugador(Dante); }
	}
}

float AMiniBossPeste::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DanoReal = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (Salud > 0.0f && !bEstaEjecutandoHabilidad)
	{
		ContadorGolpesRecibidos++;
		if (ContadorGolpesRecibidos >= LimiteGolpesParaVomitar)
		{
			IniciarPreparacionVomito();
		}
	}
	return DanoReal;
}

void AMiniBossPeste::AtacarJugador()
{
	if (bEstaEjecutandoHabilidad) return;

	OnAtaqueCortaDistanciaBlueprint();
	Super::AtacarJugador();
	GetWorldTimerManager().SetTimer(TemporizadorZarpazo, this, &AMiniBossPeste::DespertarCerebro, 1.5f, false);
}

void AMiniBossPeste::IniciarPreparacionVomito()
{
	bEstaEjecutandoHabilidad = true;
	ContadorGolpesRecibidos = 0;
	GetCharacterMovement()->MaxWalkSpeed = 0.0f;
	OnPrepararVomitoBlueprint();
	GetWorldTimerManager().SetTimer(TemporizadorPreparacion, this, &AMiniBossPeste::EjecutarVomitoAbanico, 1.5f, false);
}

void AMiniBossPeste::EjecutarVomitoAbanico()
{
	ACharacter* Dante = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!Dante) return;

	FVector DireccionHaciaDante = Dante->GetActorLocation() - GetActorLocation();
	FRotator RotacionBase = FRotator(0.0f, DireccionHaciaDante.Rotation().Yaw, 0.0f);
	SetActorRotation(RotacionBase, ETeleportType::TeleportPhysics);

	float Angulos[6] = { -45.0f, -27.0f, -9.0f, 9.0f, 27.0f, 45.0f };
	// Lo sacamos más lejos para que las balas no choquen con su propia cápsula
	FVector Origen = GetActorLocation() + (GetActorForwardVector() * 500.0f) + FVector(0.0f, 0.0f, 50.0f);

	for (int i = 0; i < 6; i++)
	{
		FRotator RotacionDisparo = RotacionBase;
		RotacionDisparo.Yaw += Angulos[i];
		EjecutarDisparo(Origen, RotacionDisparo, 400.0f, 0.0f);
	}

	GetWorldTimerManager().SetTimer(TemporizadorRecuperacion, this, &AMiniBossPeste::FinalizarRecuperacion, 2.0f, false);
}

void AMiniBossPeste::FinalizarRecuperacion()
{
	GetCharacterMovement()->MaxWalkSpeed = VelocidadMovimientoJefe;
	bEstaEjecutandoHabilidad = false;
	DespertarCerebro();
}

void AMiniBossPeste::DespertarCerebro()
{
	if (EstadoActual != EEstadoEnemigo::Muerto)
	{
		EstadoActual = EEstadoEnemigo::Persiguiendo;
	}
}