// Fill out your copyright notice in the Description page of Project Settings.


// Dante: El Último Sello - Nivel 3
// Implementación del Acechador Sanguinario

#include "AcechadorSanguinario.h"

AAcechadorSanguinario::AAcechadorSanguinario()
{
    // --- AJUSTAMOS LOS VALORES HEREDADOS DE EnemyBase ---
    // Según el documento Nivel 3:

    // El Acechador tiene 20 HP igual que los demás comunes
    SaludMaxima = 20.0f;
    Salud = SaludMaxima;

    // Hace MENOS daño que el Siervo: solo 3 por golpe
    // Es más ágil pero menos fuerte
    DanoAtaque = 3.0f;

    // Distancia de ataque cuerpo a cuerpo
    DistanciaAtaque = 120.0f;

    // Regenera 2 HP por golpe, igual que el Siervo
    RegeneracionPorGolpe = 2.0f;
}

void AAcechadorSanguinario::BeginPlay()
{
    // Llamamos al BeginPlay del padre (EnemyBase)
    // Activa el sensor de visión y vincula AlVerJugador
    Super::BeginPlay();
}

void AAcechadorSanguinario::AtacarJugador()
{
    // Ejecutamos el ataque normal heredado de EnemyBase
    // Aplica DanoAtaque (3) a Dante y activa el cooldown de 1.5 segundos
    Super::AtacarJugador();

    // --- MECÁNICA VAMPÍRICA ---
    // Después de golpear, el Acechador se cura a sí mismo
    // FMath::Clamp evita que la salud supere el máximo permitido
    Salud = FMath::Clamp(Salud + RegeneracionPorGolpe, 0.0f, SaludMaxima);

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, TEXT("¡Acechador Sanguinario encajó un zarpazo ágil!"));
    }
}