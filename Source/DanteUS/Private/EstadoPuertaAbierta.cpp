// Fill out your copyright notice in the Description page of Project Settings.


#include "EstadoPuertaAbierta.h"
#include "PuertaPasillo.h"

void UEstadoPuertaAbierta::AlPisarTrigger()
{
    // Estado abierto — Dante pisa el trigger e inicia la transición
    if (MiPuerta)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
            TEXT("Puerta: Abierta — iniciando carga del siguiente mapa."));
        MiPuerta->IniciarTransicion();
    }
}

void UEstadoPuertaAbierta::AlRecibirAviso()
{
    // Ya estamos abiertos — no hacemos nada
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::White,
        TEXT("Puerta: Ya estaba abierta."));
}
