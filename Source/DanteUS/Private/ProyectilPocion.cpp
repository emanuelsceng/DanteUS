
#include "ProyectilPocion.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h" // Necesario para el LineTrace
#include "GameFramework/Character.h"

AProyectilPocion::AProyectilPocion()
{
    PrimaryActorTick.bCanEverTick = false;

    ColisionEsfera = CreateDefaultSubobject<USphereComponent>(TEXT("ColisionEsfera"));
    ColisionEsfera->InitSphereRadius(25.0f);

    // 🛠️ OPTIMIZACIÓN DE COLISIÓN: Ignoramos canales basura y solo escuchamos lo importante
    ColisionEsfera->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    ColisionEsfera->SetCollisionObjectType(ECC_WorldDynamic);

    ColisionEsfera->SetCollisionResponseToAllChannels(ECR_Ignore); // Apagamos todo por defecto
    ColisionEsfera->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);  // Suelo y paredes
    ColisionEsfera->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);         // Dante y enemigos

    RootComponent = ColisionEsfera;

    MeshPocion = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshPocion"));
    MeshPocion->SetupAttachment(RootComponent);
    MeshPocion->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    MovimientoProyectil = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovimientoProyectil"));
    MovimientoProyectil->UpdatedComponent = ColisionEsfera;
    MovimientoProyectil->InitialSpeed = 800.0f;
    MovimientoProyectil->MaxSpeed = 800.0f;
    MovimientoProyectil->ProjectileGravityScale = 0.5f;
    MovimientoProyectil->bShouldBounce = false;
    MovimientoProyectil->bRotationFollowsVelocity = true;
    MovimientoProyectil->bInitialVelocityInLocalSpace = true;

    InitialLifeSpan = 5.0f;
}

void AProyectilPocion::BeginPlay()
{
    Super::BeginPlay();
    ColisionEsfera->OnComponentBeginOverlap.AddDynamic(this, &AProyectilPocion::AlSuperponerse);
}

void AProyectilPocion::AlSuperponerse(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (bYaImpacto) return;
    if (OtherActor == GetOwner()) return;
    if (OtherActor && OtherActor->IsA(AProyectilPocion::StaticClass())) return;

    bYaImpacto = true;
    FVector PuntoImpacto = GetActorLocation();

    // Verificación segura mediante clase heredada
    if (OtherActor && OtherActor != this && OtherActor->IsA(ACharacter::StaticClass()))
    {
        UGameplayStatics::ApplyDamage(OtherActor, Dano, nullptr, this, UDamageType::StaticClass());

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("¡POCIÓN IMPACTÓ A DANTE! Daño aplicado"));
        }
    }

    //  OPTIMIZACIÓN PROFESIONAL: Forzar que el charco spawnee exactamente en el suelo
    FVector PosicionSuelo = PuntoImpacto;
    FHitResult HitSuelo;
    FVector TraceStart = PuntoImpacto;
    FVector TraceEnd = PuntoImpacto - FVector(0.0f, 0.0f, 500.0f); // Tira un rayo hacia abajo de 5 metros
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);
    QueryParams.AddIgnoredActor(GetOwner());
    // Ignorar a Dante para que el trace llegue al suelo
    if (OtherActor)
    {
        QueryParams.AddIgnoredActor(OtherActor);
    }

    if (GetWorld()->LineTraceSingleByChannel(HitSuelo, TraceStart, TraceEnd, ECC_WorldStatic, QueryParams))
    {
        PosicionSuelo = HitSuelo.ImpactPoint; // Si encuentra suelo, usa la coordenada del piso real
    }

    CrearZonaCongelada(PosicionSuelo);
    Destroy();
}

void AProyectilPocion::CrearZonaCongelada(FVector Ubicacion)
{
    // Spawneamos el efecto de escarcha al impactar
    if (EfectoImpacto)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            EfectoImpacto,
            Ubicacion,
            FRotator::ZeroRotator
        );
    }
    if (!ClaseZonaCongelada || !GetWorld()) return;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    GetWorld()->SpawnActor<AActor>(ClaseZonaCongelada, Ubicacion, FRotator::ZeroRotator, Params);
    UE_LOG(LogTemp, Warning, TEXT("ProyectilPocion: Zona Congelada creada en el suelo firmemente."));
}