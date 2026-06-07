// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObservadorArena.h"
#include "PuertaPasillo.h"
#include "ArenaManager.generated.h"

UCLASS()
class DANTEUS_API AArenaManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArenaManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	// Lista de suscriptores — equivalente a Subscribers[] del libro
	TArray<TScriptInterface<IObservadorArena>> ListaSuscriptores;

	// Cuántos Spawners activos quedan sin limpiar
	int32 SpawnersActivos = 0;
public:	
	// Llamado por cada Spawner al terminar su oleada
	void NotificarOleadaLimpia();

	// Registra un suscriptor — equivalente a Subscribe() del libro
	void Suscribir(TScriptInterface<IObservadorArena> Suscriptor);

	// Registra cuántos Spawners va a monitorear esta arena
	UFUNCTION(BlueprintCallable, Category = "Arena | Observer")
	void RegistrarSpawners(int32 CantidadSpawners);
	// La puerta que se desbloqueará cuando todas las oleadas terminen
// Se asigna desde el editor
	UPROPERTY(EditAnywhere, Category = "Arena | Config")
	class APuertaPasillo* PuertaObjetivo;

};
