#include "ProyectilBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

AProyectilBase::AProyectilBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// Construimos el núcleo físico
	Colision = CreateDefaultSubobject<USphereComponent>(TEXT("Colision"));
	RootComponent = Colision;
	Colision->InitSphereRadius(10.0f);
	Colision->SetCollisionProfileName(TEXT("BlockAllDynamic"));

	// Conectamos el choque
	Colision->OnComponentHit.AddDynamic(this, &AProyectilBase::AlChocar);

	// Construimos el motor de vuelo
	Movimiento = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movimiento"));
	Movimiento->UpdatedComponent = Colision;

	// Valores genéricos (Cada enemigo los cambiará en su Blueprint)
	Movimiento->InitialSpeed = 1000.0f;
	Movimiento->MaxSpeed = 1000.0f;
	DanoAtaque = 10.0f;

	// Limpieza automática: Si la bala no le da a nada en 3 segundos, se borra para no dar lag.
	SetLifeSpan(3.0f);
}

void AProyectilBase::AlChocar(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// 1. LA MATEMÁTICA: Si golpeó a Dante, le restamos la vida
	if (OtherActor && OtherActor != this && OtherActor->IsA(ACharacter::StaticClass()))
	{
		UGameplayStatics::ApplyDamage(OtherActor, DanoAtaque, nullptr, this, UDamageType::StaticClass());
	}

	// 2. EL ARTE: Avisamos al Blueprint que ya chocamos para que haga explotar sus partículas o sonidos
	AlImpactarEfectosVisuales();

	// 3. DESTRUCCIÓN: Borramos la esfera invisible matemática
	Destroy();
}