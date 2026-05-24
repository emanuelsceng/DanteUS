#include "EnemigoPustulento.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemigoPustulento::AEnemigoPustulento()
{
    PrimaryActorTick.bCanEverTick = true;

    // Inicialización de variables
    DanteEnvenenado = nullptr;
    bEstaAtacando = false;
    VelocidadMovimiento = 0.0f;

    // Stats Base
    SaludMaxima = 20.0f;
    Salud = SaludMaxima;
    DanoAtaque = 5.0f;

    // Construcción del Aura
    AuraVeneno = CreateDefaultSubobject<USphereComponent>(TEXT("AuraVeneno"));
    AuraVeneno->SetupAttachment(RootComponent);
    AuraVeneno->InitSphereRadius(200.0f);
    AuraVeneno->SetCollisionProfileName(TEXT("Trigger"));
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = 450.0f; // Sube este valor para ir más rápido
    }
}

void AEnemigoPustulento::BeginPlay()
{
    Super::BeginPlay();
    AuraVeneno->OnComponentBeginOverlap.AddDynamic(this, &AEnemigoPustulento::AlEntrarAlAura);
    AuraVeneno->OnComponentEndOverlap.AddDynamic(this, &AEnemigoPustulento::AlSalirDelAura);
}

void AEnemigoPustulento::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    VelocidadMovimiento = GetVelocity().Size();
}

void AEnemigoPustulento::AtacarJugador()
{
    Super::AtacarJugador();
    bEstaAtacando = true;

    // Resetear animación
    GetWorldTimerManager().SetTimer(TimerHandle_ResetAnim, [this]() {
        bEstaAtacando = false;
        }, 1.0f, false);
}

void AEnemigoPustulento::AlEntrarAlAura(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this && OtherActor->IsA(ACharacter::StaticClass()))
    {
        DanteEnvenenado = OtherActor;
        GetWorldTimerManager().SetTimer(TemporizadorVeneno, this, &AEnemigoPustulento::AplicarVeneno, 1.0f, true);
    }
}

void AEnemigoPustulento::AlSalirDelAura(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor == DanteEnvenenado)
    {
        DanteEnvenenado = nullptr;
        GetWorldTimerManager().ClearTimer(TemporizadorVeneno);
    }
}

void AEnemigoPustulento::AplicarVeneno()
{
    if (DanteEnvenenado)
    {
        // Aplicar el daño
        UGameplayStatics::ApplyDamage(DanteEnvenenado, 2.0f, GetInstigatorController(), this, UDamageType::StaticClass());

        // --- RETROALIMENTACIÓN VISUAL (Feedback) ---
        // Esto aparecerá en tu pantalla mientras estés dentro del aura.
        // El primer parámetro es el Key (pon -1 para que no reemplace el anterior)
        // El segundo es el tiempo, el tercero es el color
        GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, TEXT("¡VENENO ACTIVADO! Daño recibido..."));
    }
}