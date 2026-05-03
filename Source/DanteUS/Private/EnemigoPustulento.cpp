#include "EnemigoPustulento.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h" 

AEnemigoPustulento::AEnemigoPustulento()
{
	// Configuramos las estadísticas específicas de este enemigo heredadas de la base
	SaludMaxima = 20.0f; // Salud de enemigo común
	Salud = SaludMaxima;
	DanoAtaque = 5.0f; // Daño físico pesado cuerpo a cuerpo
	DistanciaAtaque = 150.0f; // Rango corto

	// Construimos el Aura de Veneno
	AuraVeneno = CreateDefaultSubobject<USphereComponent>(TEXT("AuraVeneno"));
	AuraVeneno->SetupAttachment(RootComponent);
	AuraVeneno->InitSphereRadius(200.0f); // Rango del hedor
	AuraVeneno->SetCollisionProfileName(TEXT("Trigger"));

	DanteEnvenenado = nullptr;
}

void AEnemigoPustulento::BeginPlay()
{
	Super::BeginPlay();

	// Conectamos el Patrón Observador a los eventos del motor
	AuraVeneno->OnComponentBeginOverlap.AddDynamic(this, &AEnemigoPustulento::AlEntrarAlAura);
	AuraVeneno->OnComponentEndOverlap.AddDynamic(this, &AEnemigoPustulento::AlSalirDelAura);
}

// POLIMORFISMO: Este es su ataque normal de 5 puntos (Golpe Físico)
void AEnemigoPustulento::AtacarJugador()
{
	// Llamamos a la lógica base para aplicar el daño y reiniciar la máquina de estados
	Super::AtacarJugador();

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("¡El Pustulento te golpeó duro!"));
}

// --- LÓGICA PRIVADA DEL VENENO (MECÁNICA DE RIESGO) ---

void AEnemigoPustulento::AlEntrarAlAura(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Si el que entra es Dante (y no está muerto)...
	if (OtherActor && OtherActor != this && OtherActor->IsA(ACharacter::StaticClass()))
	{
		DanteEnvenenado = OtherActor;

		// Esperamos 1 segundo antes de empezar a hacer daño, como dice el documento de diseño
		GetWorldTimerManager().SetTimer(TemporizadorVeneno, this, &AEnemigoPustulento::AplicarVeneno, 1.0f, true);
	}
}

void AEnemigoPustulento::AlSalirDelAura(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == DanteEnvenenado)
	{
		// Dante aplicó la táctica de entrar, golpear y salir rápidamente. Lo salvamos del veneno.
		DanteEnvenenado = nullptr;
		GetWorldTimerManager().ClearTimer(TemporizadorVeneno);
	}
}

void AEnemigoPustulento::AplicarVeneno()
{
	if (DanteEnvenenado && EstadoActual != EEstadoEnemigo::Muerto)
	{
		// Si sigue adentro después de 1 segundo, recibe 2 de daño continuo
		UGameplayStatics::ApplyDamage(DanteEnvenenado, 2.0f, GetController(), this, UDamageType::StaticClass());

		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("¡Dante se está asfixiando (2 daño)!"));
	}
}