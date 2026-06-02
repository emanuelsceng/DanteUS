// Fill out your copyright notice in the Description page of Project Settings.


// Dante: El Último Sello - Nivel 3
// Implementación del Guardián Vampírico
#include "GuardianVampirico.h" // ESTO DEBE SER LA LÍNEA 1
#include "AIController.h"       // Luego los demás
#include "Kismet/GameplayStatics.h"


AGuardianVampirico::AGuardianVampirico()
{
    // --- AJUSTAMOS LOS VALORES HEREDADOS DE EnemyBase ---
    // Según el documento Nivel 3:

    // El Guardián tiene 20 HP igual que los demás comunes
    SaludMaxima = 30.0f;
    Salud = SaludMaxima;

    // Es el MÁS FUERTE de los comunes: 10 de daño por golpe
    // Dante necesita 2 ataques para matarlo (Dante hace 5 dmg)
    DanoAtaque = 5.0f;

    // Distancia de ataque cuerpo a cuerpo
    DistanciaAtaque = 120.0f;

    // Regenera 4 HP por golpe, el DOBLE que Siervo y Acechador
    // Si Dante no lo elimina rápido, se vuelve muy difícil de matar
    RegeneracionPorGolpe = 4.0f;
}

void AGuardianVampirico::BeginPlay()
{
    // Llamamos al BeginPlay del padre (EnemyBase)
    // Activa el sensor de visión y vincula AlVerJugador
    Super::BeginPlay();
}
void AGuardianVampirico::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AGuardianVampirico::AtacarJugador()
{
    // Detener movimiento al atacar
    AAIController* ControladorIA = Cast<AAIController>(GetController());
    if (ControladorIA)
    {
        ControladorIA->StopMovement();
    }
    // Ejecutamos el ataque normal heredado de EnemyBase
    // Aplica DanoAtaque (10) a Dante y activa el cooldown de 1.5 segundos
    Super::AtacarJugador();

    // --- MECÁNICA VAMPÍRICA MEJORADA ---
    // El Guardián se cura 4 HP después de cada golpe
    // FMath::Clamp evita que la salud supere el máximo permitido
    Salud = FMath::Clamp(Salud + RegeneracionPorGolpe, 0.0f, SaludMaxima);

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Purple, TEXT("¡Guardián Vampírico conectó un golpe pesado!"));
    }
}

void AGuardianVampirico::Morir()
{
    // Reproducir animacion de muerte
    UAnimInstance* AnimInstancia = GetMesh()->GetAnimInstance();
    if (AnimInstancia && MontajeMuerte)
    {
        AnimInstancia->Montage_Play(MontajeMuerte);
    }

    // Esperamos que termine la animacion antes de destruir
    FTimerHandle TimerMuerte;
    GetWorldTimerManager().SetTimer(
        TimerMuerte,
        [this]() { Super::Morir(); },
        2.0f,
        false
    );
}