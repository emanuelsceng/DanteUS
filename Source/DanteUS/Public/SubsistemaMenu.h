// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SubsistemaMenu.generated.h"

UCLASS(Abstract)
class DANTEUS_API ASubsistemaMenu : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASubsistemaMenu();
	// Método para arrancar el subsistema
	virtual void InicializarSubsistema() PURE_VIRTUAL(ASubsistemaMenu::InicializarSubsistema, );

	// Método para recibir comandos del Facade
	virtual void EjecutarComandoMenu(const FString& Comando) PURE_VIRTUAL(ASubsistemaMenu::EjecutarComandoMenu, );

	// Método para limpiar memoria o apagar cosas cuando se cambie de nivel
	virtual void ApagarSubsistema() PURE_VIRTUAL(ASubsistemaMenu::ApagarSubsistema, );

	// Método de reporte de estado
	virtual FString ObtenerNombreSubsistema() PURE_VIRTUAL(ASubsistemaMenu::ObtenerNombreSubsistema, return "";);
};
