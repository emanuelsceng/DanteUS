#include "ZonaEfectoBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AZonaEfectoBase::AZonaEfectoBase()
{
    AreaColision = CreateDefaultSubobject<USphereComponent>(TEXT("AreaColision"));
    RootComponent = AreaColision;
    AreaColision->SetSphereRadius(200.0f);
    AreaColision->SetCollisionProfileName(TEXT("Trigger"));

    MallaZona = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaZona"));
    MallaZona->SetupAttachment(RootComponent);

    MultiplicadorVelocidad = 0.5f;
    InitialLifeSpan = 5.0f;
    VelocidadOriginal = 600.0f; // Valor por defecto
}

void AZonaEfectoBase::BeginPlay()
{
    Super::BeginPlay();
    AreaColision->OnComponentBeginOverlap.AddDynamic(this, &AZonaEfectoBase::OnEntraEnZona);
    AreaColision->OnComponentEndOverlap.AddDynamic(this, &AZonaEfectoBase::OnSaleDeZona);
}

void AZonaEfectoBase::OnEntraEnZona(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ACharacter* Dante = Cast<ACharacter>(OtherActor);
    if (Dante && Dante->GetCharacterMovement())
    {
        VelocidadOriginal = Dante->GetCharacterMovement()->MaxWalkSpeed;
        Dante->GetCharacterMovement()->MaxWalkSpeed *= MultiplicadorVelocidad;

        UGameplayStatics::ApplyDamage(Dante, 5.0f, nullptr, this, UDamageType::StaticClass());
    }
}

void AZonaEfectoBase::OnSaleDeZona(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    ACharacter* Dante = Cast<ACharacter>(OtherActor);
    if (Dante && Dante->GetCharacterMovement())
    {
        Dante->GetCharacterMovement()->MaxWalkSpeed = VelocidadOriginal;
    }
}