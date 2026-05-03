#include "ProyectilBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h" 

AProyectilBase::AProyectilBase()
{
	// No necesitamos que el proyectil piense cada frame, el motor físico lo mueve solo
	PrimaryActorTick.bCanEverTick = false;

	// 1. CONFIGURAMOS LA ESFERA FÍSICA
	EsferaColision = CreateDefaultSubobject<USphereComponent>(TEXT("EsferaColision"));
	RootComponent = EsferaColision; // La esfera es el centro del objeto
	EsferaColision->InitSphereRadius(15.0f); // Tamaño genérico de la bala

	// 'BlockAllDynamic' significa que no es un fantasma, chocará contra paredes, piso y personajes
	EsferaColision->SetCollisionProfileName(TEXT("BlockAllDynamic"));

	// 2. CONFIGURAMOS EL COMPONENTE DE VUELO
	ComponenteMovimiento = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ComponenteMovimiento"));
	ComponenteMovimiento->UpdatedComponent = EsferaColision; // Le decimos que mueva la esfera
	ComponenteMovimiento->InitialSpeed = 1500.0f; // Velocidad de salida
	ComponenteMovimiento->MaxSpeed = 1500.0f;     // Velocidad máxima
	ComponenteMovimiento->bRotationFollowsVelocity = true; // Que apunte hacia donde va
	ComponenteMovimiento->ProjectileGravityScale = 0.0f; // Gravedad cero por defecto (vuela recto)

	// Daño por defecto de seguridad
	DanoProyectil = 5.0f;

	// GESTIÓN DE MEMORIA: Destruir a los 4 segundos si falla el tiro y se pierde en el vacío
	SetLifeSpan(4.0f);
}

void AProyectilBase::BeginPlay()
{
	Super::BeginPlay();

	// Conectamos la física del motor con nuestra función de daño (Patrón Observador)
	EsferaColision->OnComponentHit.AddDynamic(this, &AProyectilBase::AlChocar);
}

void AProyectilBase::AlChocar(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Filtro de seguridad: Que el actor exista, no se golpee a sí mismo, y sea un Personaje (Dante)
	if (OtherActor && OtherActor != this && OtherActor->IsA(ACharacter::StaticClass()))
	{
		// Aplicamos el daño que el diseñador configuró en el Blueprint
		UGameplayStatics::ApplyDamage(OtherActor, DanoProyectil, nullptr, this, UDamageType::StaticClass());

		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, TEXT("¡Proyectil impactó a Dante!"));S
	}

	// Sin importar si chocó con Dante, con una pared o el piso, el proyectil explota/desaparece
	Destroy();
}