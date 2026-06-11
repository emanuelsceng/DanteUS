// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SubsistemaMenu.h"
#include "MenuPrincipalFacade.generated.h"

UCLASS()
class DANTEUS_API AMenuPrincipalFacade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMenuPrincipalFacade();

private:
	// Punteros de la clase abstracta base
	// La lista polimórfica que guarda a los oficiales/trabajadores del menú
	TArray<ASubsistemaMenu*> ListaSubsistemas;

	// La lista de tareas basada en Strings, 
	TArray<FString> ListaTareas;

	// Método privado de despacho masivo mediante bucles anidados 
	void DespacharTareas(TArray<ASubsistemaMenu*> MisSubsistemas, TArray<FString> MisTareas);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Funciones que llamará el Widget de la interfaz visual (Tu boceto)
	UFUNCTION(BlueprintCallable, Category = "Dante | Menú")
	void InicializarMenuInicio();

	UFUNCTION(BlueprintCallable, Category = "Dante | Menú")
	void PresionaronBotonPlay();

	UFUNCTION(BlueprintCallable, Category = "Dante | Menú")
	void PresionaronBotonCargarNivel(int NumeroNivel);

};
