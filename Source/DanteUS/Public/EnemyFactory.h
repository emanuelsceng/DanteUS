// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBase.h" // Incluimos la base para que la fábrica sepa qué está creando
#include "EnemyFactory.generated.h"


//DEFINICIÓN DE PRODUCTOS: Enumerador con todos los enemigos del documento de diseño
UENUM(BlueprintType)
enum class ETipoEnemigo : uint8
{
	Ceniza              UMETA(DisplayName = "Soldado de Ceniza (Nivel 1)"),
	Peste_Escupidor     UMETA(DisplayName = "Escupidor (Nivel 2)"),
	Peste_Pustulento    UMETA(DisplayName = "Pustulento (Nivel 2)"),
	Vampirico_Siervo     UMETA(DisplayName = "Siervo Vampirico (Nivel 3)"),
	Vampirico_Acechador  UMETA(DisplayName = "Acechador Sanguinario (Nivel 3)"),
	Vampirico_Guardian   UMETA(DisplayName = "Guardian Vampirico (Nivel 3)"),
	Fantasma            UMETA(DisplayName = "Fantasma Errante (Nivel 4)")
};

UCLASS()
class DANTEUS_API AEnemyFactory : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyFactory();
	/**
	 * 2. EL MÉTODO FÁBRICA: Es 'static' para que puedas invocarlo desde cualquier
	 * parte del código sin necesidad de buscar una instancia de la fábrica en el mapa.
	 */
	UFUNCTION(BlueprintCallable, Category = "Dante | Factory")
	static AEnemyBase* CrearEnemigo(UWorld* Mundo, ETipoEnemigo Tipo, FVector Posicion, FRotator Rotacion);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
