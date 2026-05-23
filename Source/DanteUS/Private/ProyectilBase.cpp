#include "ProyectilBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "EnemyBase.h" // Incluimos la base para identificar esbirros

AProyectilBase::AProyectilBase()
{
	// OPTIMIZACIÓN CPU: Falso elimina el Tick por fotograma del procesador
	PrimaryActorTick.bCanEverTick = false;

	Colision = CreateDefaultSubobject<USphereComponent>(TEXT("Colision"));
	RootComponent = Colision;
	Colision->InitSphereRadius(12.0f);
	Colision->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	Colision->OnComponentHit.AddDynamic(this, &AProyectilBase::AlChocar);
	Colision->CanCharacterStepUpOn = ECB_No; // Evita saltos físicos raros sobre la bala

	Movimiento = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movimiento"));
	Movimiento->UpdatedComponent = Colision;
	Movimiento->bRotationFollowsVelocity = true; // Alinea mallas como flechas automáticamente
	Movimiento->InitialSpeed = 2000.0f;
	Movimiento->MaxSpeed = 7000.0f;

	DanoAtaque = 10.0f;
	bUsaObjectPool = false;
}

void AProyectilBase::BeginPlay()
{
	Super::BeginPlay();
}

void AProyectilBase::ActivarProyectil(FVector NuevaPosicion, FRotator NuevaRotacion, float Velocidad, float Gravedad)
{
	// 1. Teletransportar y rotar físicamente el proyectil antes de encender el motor
	SetActorLocation(NuevaPosicion);
	SetActorRotation(NuevaRotacion);

	// 2. Volverlo visible en el juego
	SetActorHiddenInGame(false);

	// 3. ¡EL ARREGLO DE ORO!: Forzar al componente esférico a encender sus sensores
	SetActorEnableCollision(true);
	if (Colision)
	{
		// Despierta la colisión física y de consultas (Query) para que registre impactos
		Colision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}

	// 4. Configurar y arrancar el motor de movimiento balístico
	if (Movimiento)
	{
		Movimiento->ProjectileGravityScale = Gravedad;
		Movimiento->Velocity = NuevaRotacion.Vector() * Velocidad;

		// Mantenemos el sistema teledirigido (Homing) hacia Dante
		ACharacter* Dante = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		if (Dante)
		{
			Movimiento->bIsHomingProjectile = true;
			Movimiento->HomingAccelerationMagnitude = 1800.0f;
			Movimiento->HomingTargetComponent = Dante->GetRootComponent();
		}

		Movimiento->UpdateComponentVelocity(); // Sincroniza la velocidad con el motor de físicas
		Movimiento->Activate(true);            // Enciende el componente de simulación
	}

	// 5. Temporizador de seguridad (Auto-reciclaje en 4 segundos si falla el tiro)
	GetWorldTimerManager().SetTimer(TemporizadorReciclaje, this, &AProyectilBase::DesactivarProyectil, 4.0f, false);
}

void AProyectilBase::DesactivarProyectil()
{
	// Limpiamos el reloj para evitar ejecuciones duplicadas en memoria
	GetWorldTimerManager().ClearTimer(TemporizadorReciclaje);

	// Ocultar al actor del renderizado de la tarjeta gráfica
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	// ¡DORMIR EL COMPONENTE!: Apagamos la esfera para que no cause colisiones fantasma bajo tierra
	if (Colision)
	{
		Colision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// Frenar por completo el motor de movimiento
	if (Movimiento)
	{
		Movimiento->StopMovementImmediately();
		Movimiento->Deactivate();
	}
}
void AProyectilBase::AlChocar(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// 1. PATRÓN GUARD CLAUSE (El Blindaje Supremo)
	// Si choca contra la nada, contra sí mismo, contra el dueño (Jefe), contra otro enemigo, o contra OTRA BALA... ¡lo ignoramos!
	if (!OtherActor || OtherActor == this || OtherActor == GetOwner() ||
		OtherActor->IsA(AEnemyBase::StaticClass()) || OtherActor->IsA(AProyectilBase::StaticClass()))
	{
		return; // Salimos de la función inmediatamente, evitando el crash
	}

	// 2. LÓGICA DE DAÑO (Si llega aquí, es porque chocó contra Dante o el suelo/pared)
	if (OtherActor->IsA(ACharacter::StaticClass()))
	{
		AController* ControladorEnemigo = GetInstigatorController();
		if (!ControladorEnemigo && GetOwner())
		{
			ControladorEnemigo = GetOwner()->GetInstigatorController();
		}

		UGameplayStatics::ApplyDamage(OtherActor, DanoAtaque, ControladorEnemigo, this, UDamageType::StaticClass());
	}

	// 3. EFECTOS Y RECICLAJE (Object Pool)
	AlImpactarEfectosVisuales();

	if (bUsaObjectPool)
	{
		DesactivarProyectil(); // Lo apagamos para reutilizarlo
	}
	else
	{
		Destroy();
	}
}