// Fill out your copyright notice in the Description page of Project Settings.


#include "NivelDante.h"

// Sets default values
ANivelDante::ANivelDante()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void ANivelDante::SetNombreNivel(const FString& Nombre) { NombreNivel = Nombre; }
void ANivelDante::SetConfiguracionBase(const FString& Config) { ConfiguracionBase = Config; }
void ANivelDante::AgregarEnemigoComun(const FString& Enemigo) { EnemigosComunes.Add(Enemigo); }
void ANivelDante::SetDatosMiniBoss(const FString& MiniBoss) { DatosMiniBoss = MiniBoss; }
void ANivelDante::SetDatosBoss(const FString& Boss) { DatosBoss = Boss; }
void ANivelDante::SetNombreMapa(const FName& NombreMapa) { NombreMapaUE = NombreMapa; }

void ANivelDante::ReportarCaracteristicasNivel()
{
    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::White,
        FString::Printf(TEXT("=== %s ==="), *NombreNivel));
    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan,
        FString::Printf(TEXT("Config: %s"), *ConfiguracionBase));
    for (const FString& Enemigo : EnemigosComunes)
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green,
            FString::Printf(TEXT("  Enemigo: %s"), *Enemigo));
    }
    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow,
        FString::Printf(TEXT("MiniBoss: %s"), *DatosMiniBoss));
    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
        FString::Printf(TEXT("Boss: %s"), *DatosBoss));
}