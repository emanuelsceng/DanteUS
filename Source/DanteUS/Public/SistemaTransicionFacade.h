// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SistemaTransicionFacade.generated.h"

UCLASS()
class DANTEUS_API ASistemaTransicionFacade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASistemaTransicionFacade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Llamado por PuertaPasillo — orquesta todo el proceso
	UFUNCTION(BlueprintCallable, Category = "Dante | Transicion")
	void EjecutarTransicion(FName NombreMapa);

private:
	void GuardarProgreso();
	void LimpiarNivelActual();
	void CargarSiguienteMapa(FName NombreMapa);

};
