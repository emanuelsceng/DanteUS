// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NivelDante.generated.h"

UCLASS()
class DANTEUS_API ANivelDante : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANivelDante();
private:
    // Las "habitaciones" del nivel — equivalente a los campos de ALodging
    FString NombreNivel;
    FString ConfiguracionBase;      // Nombre del mapa, iluminación, etc.
    TArray<FString> EnemigosComunes;
    FString DatosMiniBoss;
    FString DatosBoss;

    // TODO: Aquí inyectarás el NombreDelMapa para UGameplayStatics::OpenLevel
    FName NombreMapaUE;

public:
    // Setters llamados por los Builders concretos
    void SetNombreNivel(const FString& Nombre);
    void SetConfiguracionBase(const FString& Config);
    void AgregarEnemigoComun(const FString& Enemigo);
    void SetDatosMiniBoss(const FString& MiniBoss);
    void SetDatosBoss(const FString& Boss);
    void SetNombreMapa(const FName& NombreMapa);

    // Getters
    FORCEINLINE FName GetNombreMapa() const { return NombreMapaUE; }

    // Reporte de diagnóstico — equivalente a LodgingCharacteristics()
    void ReportarCaracteristicasNivel();
};
