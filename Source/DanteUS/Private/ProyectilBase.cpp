#include "ProyectilBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "EnemyBase.h" 

AProyectilBase::AProyectilBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Colision = CreateDefaultSubobject<USphereComponent>(TEXT("Colision Esfera"));
	RootComponent = Colision;
	Colision->SetCollisionProfileName(TEXT("BlockAllDynamic"));

	MallaVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Malla Visual"));
	MallaVisual->SetupAttachment(RootComponent);

	ComponenteMovimiento = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Componente Movimiento"));
	ComponenteMovimiento->bAutoActivate = false;
	ComponenteMovimiento->ProjectileGravityScale = 0.0f;

	DanoAtaque = 10.0f;
	bUsaObjectPool = true;
}

void AProyectilBase::BeginPlay()
{
	Super::BeginPlay();
	// Aquí es donde estaba el error de la 'w' suelta. Ya está corregido con su punto y coma.
	Colision->OnComponentHit.AddDynamic(this, &AProyectilBase::AlChocar);
}

void AProyectilBase::Disparar(FVector Direccion, float Velocidad, float Gravedad)
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);

	ComponenteMovimiento->ProjectileGravityScale = Gravedad;
	ComponenteMovimiento->Velocity = Direccion * Velocidad;
	ComponenteMovimiento->Activate(true);

	// ¡NUEVA LÍNEA! Le avisa al Blueprint que la bala acaba de ser reciclada
	OnProyectilDisparado();
}

void AProyectilBase::DesactivarProyectil()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	ComponenteMovimiento->Deactivate();
	ComponenteMovimiento->Velocity = FVector::ZeroVector;
}

void AProyectilBase::AlChocar(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// GUARD CLAUSE: No choca consigo mismo, ni con quien lo disparó, ni con otros enemigos, ni con otras balas.
	if (!OtherActor || OtherActor == this || OtherActor == GetOwner() ||
		OtherActor->IsA(AEnemyBase::StaticClass()) || OtherActor->IsA(AProyectilBase::StaticClass()))
	{
		return;
	}

	// Aplicamos daño si es un personaje (Como Dante)
	if (OtherActor->IsA(ACharacter::StaticClass()))
	{
		AController* ControladorDuenio = nullptr;
		if (GetOwner())
		{
			ControladorDuenio = GetOwner()->GetInstigatorController();
		}
		UGameplayStatics::ApplyDamage(OtherActor, DanoAtaque, ControladorDuenio, this, UDamageType::StaticClass());
	}

	// Como usamos Object Pool, lo desactivamos en lugar de destruirlo
	if (bUsaObjectPool)
	{
		DesactivarProyectil();
	}
	else
	{
		Destroy();
	}
}