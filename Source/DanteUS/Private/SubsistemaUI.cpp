// Fill out your copyright notice in the Description page of Project Settings.


#include "SubsistemaUI.h"

void ASubsistemaUI::EjecutarTarea(const FString& Tarea)
{
	if (Tarea.Equals("Mostrar_GameOver"))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("%s: Mostrando pantalla de muerte ensangrentada."), *GetNombreSubsistema()));
	}
}

FString ASubsistemaUI::GetNombreSubsistema()
{
	return "Manager de UI";
}