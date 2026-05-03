// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemigoCeniza.h"
#include "Kismet/GameplayStatics.h"

AEnemigoCeniza::AEnemigoCeniza()
{
	// Ajustamos los atributos según el Nivel 1
	SaludMaxima = 20.0f;
	Salud = SaludMaxima;
	DanoAtaque = 2.0f; // Los soldados de ceniza quitan 2 de vida
}
// Sobrescribimos la función de morir para que ejecute la explosión después de 2 segundos
void AEnemigoCeniza::Morir()
{
	// En lugar de llamar a Destroy() de inmediato, iniciamos la mecánica de riesgo

	// 1. Cambiamos el estado para que deje de perseguir a Dante
	EstadoActual = EEstadoEnemigo::Muerto;

	// 2. Aquí podrías activar un efecto de luz o material (el "cuerpo se torna con luz")
	if (GEngine)
	{// Mostrar un mensaje en pantalla para indicar que el enemigo se está preparando para explotar
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("¡Enemigo preparándose para explotar!"));
	}

	// 3. Esperamos 2 segundos antes de la explosión
	GetWorldTimerManager().SetTimer(TemporizadorExplosion, this, &AEnemigoCeniza::EjecutarExplosion, 2.0f, false);
}
// Función que ejecutará la explosión final
void AEnemigoCeniza::EjecutarExplosion()
{
	// 4. Infligimos 5 de daño en área
	UGameplayStatics::ApplyRadialDamage(
		this,
		5.0f, // Daño de la explosión
		GetActorLocation(),
		300.0f, // Radio de la explosión
		UDamageType::StaticClass(),
		TArray<AActor*>(),
		this
	);

	// 5. Finalmente, eliminamos al enemigo del nivel para liberar memoria
	Destroy();
}
