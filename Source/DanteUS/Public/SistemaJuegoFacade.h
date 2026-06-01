// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SubsistemaNivel.h"
#include "SistemaJuegoFacade.generated.h"

UCLASS()
class DANTEUS_API ASistemaJuegoFacade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASistemaJuegoFacade();
private:
	// Las listas para almacenar los subsistemas y las tareas
	TArray<ASubsistemaNivel*> Subsistemas;
	TArray<FString> Tareas;

	// Función privada que ejecuta el bucle de órdenes
	void ProcesarTareas(TArray<ASubsistemaNivel*> MisSubsistemas, TArray<FString> MisTareas);

protected:
	virtual void BeginPlay() override;

public:
	// La función que llamará Dante al morir
	UFUNCTION(BlueprintCallable, Category = "Dante | Juego")
	void DanteDerrotado();

};
