// Fill out your copyright notice in the Description page of Project Settings.


// Dante: El Último Sello - Nivel 3
// Implementación de la Zona Congelada

#include "ZonaCongelada.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

AZonaCongelada::AZonaCongelada()
{
    PrimaryActorTick.bCanEverTick = false;

    // --- ZONA DE DETECCION ---
    // Capsula que detecta cuando Dante entra o sale
    ZonaDeteccion = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ZonaDeteccion"));
    ZonaDeteccion->InitCapsuleSize(900.0f, 50.0f); // Radio amplio, poca altura (es una zona en el suelo)
    ZonaDeteccion->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    RootComponent = ZonaDeteccion;

    // --- MESH VISUAL ---
    // El charco/efecto visual en el suelo
    MeshZona = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshZona"));
    MeshZona->SetupAttachment(RootComponent);
    MeshZona->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // La zona se destruye automaticamente despues de DuracionZona segundos
    // Se configura en BeginPlay con el timer
}

void AZonaCongelada::BeginPlay()
{
    Super::BeginPlay();

    // Vinculamos las funciones de entrada y salida a la zona
    ZonaDeteccion->OnComponentBeginOverlap.AddDynamic(this, &AZonaCongelada::AlEntrar);
    ZonaDeteccion->OnComponentEndOverlap.AddDynamic(this, &AZonaCongelada::AlSalir);

    // Timer para destruir la zona despues de DuracionZona segundos
    GetWorldTimerManager().SetTimer(
        TimerDestruccion,
        this,
        &AZonaCongelada::DestruirZona,
        DuracionZona,
        false
    );

    UE_LOG(LogTemp, Log, TEXT("ZonaCongelada: Creada. Durara %.0f segundos"), DuracionZona);
}

void AZonaCongelada::AlEntrar(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    // Verificamos que sea Dante quien entro (tiene el tag "Player")
    if (!OtherActor || !OtherActor->ActorHasTag("Player")) return;

    ACharacter* Dante = Cast<ACharacter>(OtherActor);
    if (!Dante) return;

    // Guardamos referencia a Dante para restaurar su velocidad al salir
    DanteDentro = Dante;

    // Guardamos su velocidad original antes de ralentizarlo
    VelocidadOriginalDante = 500.0f;

    // Aplicamos la ralentizacion multiplicando su velocidad
    // MultiplicadorVelocidad = 0.4 ? Dante camina al 40% de su velocidad normal
    Dante->GetCharacterMovement()->MaxWalkSpeed = VelocidadOriginalDante * MultiplicadorVelocidad;

    //Cartelazo directo en pantalla asegurado
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Cyan, TEXT("¡DANTE ENTRÓ A LA ZONA CONGELADA!"));
        }
}

void AZonaCongelada::AlSalir(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    // Verificamos que sea Dante quien salio
    if (!OtherActor || !OtherActor->ActorHasTag("Player")) return;

    ACharacter* Dante = Cast<ACharacter>(OtherActor);
    if (!Dante) return;

    // Restauramos la velocidad original de Dante
    Dante->GetCharacterMovement()->MaxWalkSpeed = 500.0f;
    DanteDentro = nullptr;

    // Mensaje de éxito en verde en tu pantalla
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Green, TEXT("¡DANTE SALIÓ DE LA ZONA CONGELADA!"));
    }
}

void AZonaCongelada::DestruirZona()
{
    // Si Dante sigue dentro de la zona cuando esta se destruye
    // le restauramos la velocidad para que no quede ralentizado para siempre
    if (DanteDentro)
    {
        DanteDentro->GetCharacterMovement()->MaxWalkSpeed = 500.0f;
        UE_LOG(LogTemp, Log, TEXT("ZonaCongelada: Destruida con Dante adentro. Velocidad restaurada."));
    }

    UE_LOG(LogTemp, Log, TEXT("ZonaCongelada: Zona destruida."));
    Destroy();
}