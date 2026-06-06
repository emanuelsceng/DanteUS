// Fill out your copyright notice in the Description page of Project Settings.


#include "ControladorUIMenu.h"

void AControladorUIMenu::InicializarSubsistema()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%s: Listo y en espera."), *ObtenerNombreSubsistema()));
}

void AControladorUIMenu::EjecutarComandoMenu(const FString& Comando)
{
	if (Comando.Equals("Mostrar_Pantalla_Principal"))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, TEXT("UI: Dibujando botones de PLAY y Niveles en pantalla."));
	}
}

void AControladorUIMenu::ApagarSubsistema()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("UI: Destruyendo widgets para liberar memoria."));
}

FString AControladorUIMenu::ObtenerNombreSubsistema()
{
	return "Controlador de UI del Menu";
}