// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ConstructorNivel.h"
#include "DirectorNivel.generated.h"

UCLASS()
class DANTEUS_API ADirectorNivel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADirectorNivel();
private:
    // Puntero a la INTERFAZ, nunca al concreto — igual que el libro
    IConstructorNivel* ConstructorActual;

public:
    // Equivalente a SetLodgingBuilder() del libro
    void SetConstructorNivel(AActor* Constructor);

    // Coordina los pasos de construcción — equivalente a ConstructLodging()
    void ConstruirNivelCompleto();

    // Retorna el producto final
    ANivelDante* ObtenerNivelConstruido();

};
