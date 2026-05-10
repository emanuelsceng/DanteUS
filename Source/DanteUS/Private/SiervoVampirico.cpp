// Fill out your copyright notice in the Description page of Project Settings.


// Dante: El Último Sello - Nivel 3
// Implementación del Siervo Vampírico

#include "SiervoVampirico.h"
#include "Kismet/GameplayStatics.h" // Para aplicar daño a Dante

ASiervoVampirico::ASiervoVampirico()
{
    // --- AJUSTAMOS LOS VALORES HEREDADOS DE EnemyBase ---
    // Según el documento Sección 0.1 y Nivel 3:

    // El Siervo tiene 20 HP (igual que enemigo común)
    SaludMaxima = 20.0f;
    Salud = SaludMaxima;

    // Hace 5 de daño por golpe según el documento
    DanoAtaque = 5.0f;

    // Distancia a la que puede golpear a Dante (cuerpo a cuerpo)
    DistanciaAtaque = 120.0f;

    // Cuánta vida recupera por cada golpe que da (mecánica vampírica)
    RegeneracionPorGolpe = 2.0f;
}

void ASiervoVampirico::BeginPlay()
{
    // Llamamos al BeginPlay del padre (EnemyBase)
    // Esto activa el sensor de visión y vincula AlVerJugador
    Super::BeginPlay();
}

void ASiervoVampirico::AtacarJugador()
{
    // Primero ejecutamos el ataque normal heredado de EnemyBase
    // Esto aplica el daño a Dante y activa el cooldown de 1.5 segundos
    Super::AtacarJugador();

    // --- MECÁNICA VAMPÍRICA ---
    // Después de golpear, el Siervo se cura a sí mismo
    // FMath::Clamp evita que la salud supere el máximo
    Salud = FMath::Clamp(Salud + RegeneracionPorGolpe, 0.0f, SaludMaxima);

    UE_LOG(LogTemp, Log,
        TEXT("SiervoVampirico: Ataco y regenero %.1f HP. Salud actual: %.1f"),
        RegeneracionPorGolpe, Salud);
}
