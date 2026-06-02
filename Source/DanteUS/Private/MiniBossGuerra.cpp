// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniBossGuerra.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"


AMiniBossGuerra::AMiniBossGuerra()
{
	// Atributos iniciales "Bosteadas" (Es un Boss, necesita más vida)
	SaludMaxima = 75.0f;
	Salud = SaludMaxima;
	DanoAtaque = 5.0f; // Daño del ataque normal

	// Variables de la máquina de estados
	ContadorAtaques = 0;
	TiempoRecuperacion = 2.0f;

	bEstaRecuperandose = false;
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
}

// ---------------------------------------------------------
// LA MÁQUINA DE ESTADOS: Lógica de los 3 golpes
// ---------------------------------------------------------
void AMiniBossGuerra::AtacarJugador()
{
	if (ContadorAtaques < 2)
	{

		// REPRODUCIR ANIMACIÓN VISUAL
		if (MontageAtaque) // Variable heredada de EnemyBase
		{
			PlayAnimMontage(MontageAtaque);
		}

		ContadorAtaques++;
		// Usamos el mismo Timer base para volver a perseguir
		GetWorldTimerManager().SetTimer(TemporizadorAtaque, this, &AMiniBossGuerra::FinalizarAtaque, 1.5f, false);
	}
	else
	{
		// ATAQUE ESPECIAL
		if (MontageEspecial)
		{
			PlayAnimMontage(MontageEspecial);
		}
		ContadorAtaques = 0; // Reiniciamos el patrón de ataques
	}
}


// ---------------------------------------------------------
// EL ATAQUE ESPECIAL
// ---------------------------------------------------------
void AMiniBossGuerra::EjecutarAtaqueEspecial()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Comandante: ¡ONDA DE CHOQUE (10 DANO)!"));

	// Guardamos las coordenadas exactas de la "grieta" en el suelo
	CentroDeExplosion = GetActorLocation() - FVector(0.0f, 0.0f, 120.0f);
	//  Hacemos aparecer el círculo mágico rojo de advertencia
	if (FX_CirculoAdvertencia)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FX_CirculoAdvertencia, CentroDeExplosion);
	}
	// PONEMOS AL JEFE EN RECUPERACIÓN (Inactivo por 2 segundos
	//cambio1(state) Antes EstadoActual = EEstadoEnemigo::Atacando 
	SetEstado(GetEstadoAtacando()); // Lo forzamos a quedarse en estado de ataque para que el Tick() no lo mueva
	GetCharacterMovement()->DisableMovement(); // Detenemos sus piernas

	//Programamos la DETONACIÓN para dentro de 2 segundos (TiempoRecuperacion)
	FTimerHandle TimerDetonacion;
	GetWorldTimerManager().SetTimer(TimerDetonacion, this, &AMiniBossGuerra::DetonarOndaChoque, TiempoRecuperacion, false);
}

// LA EXPLOSIÓN (Ocurre 2 segundos después)
// ---------------------------------------------------------
void AMiniBossGuerra::DetonarOndaChoque()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("¡BOOOM! ONDA DE CHOQUE (10 DANO, 600 RADIO)"));

	// Hacemos aparecer la explosión 
	if (FX_ExplosionFinal)
	{
		// Guardamos el efecto en una variable al crearlo
		UNiagaraComponent* ExplosionComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FX_ExplosionFinal, CentroDeExplosion);

		if (ExplosionComp)
		{
			// FVector(X, Y, Z). Al poner 0.5f, reducimos su tamaño a la mitad exacta. 
			// Puedes jugar con este valor (ej. 0.4f o 0.6f) hasta que el hielo coincida con la runa roja.
			ExplosionComp->SetWorldScale3D(FVector(0.5f));
		}
	}

	// Infligimos el daño radial usando las coordenadas que guardamos
	UGameplayStatics::ApplyRadialDamage(
		this,
		10.0f,
		CentroDeExplosion,   // La explosión ocurre en la grieta, no persigue al jefe
		700.0f,
		UDamageType::StaticClass(),
		TArray<AActor*>(),
		this,
		GetController()
	);

	//La explosión terminó, el jefe vuelve a la normalidad
	FinalizarRecuperacion();
}


// VOLVER A LA NORMALIDAD

void AMiniBossGuerra::FinalizarRecuperacion()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Comandante: Recuperado, persiguiendo..."));

	// Le devolvemos el movimiento
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);

	// Finalizamos el ataque usando la función heredada
	FinalizarAtaque();
}


// LA MUERTE Y LA LIMPIEZA DE MEMORIA

void AMiniBossGuerra::Morir()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("¡COMANDANTE DERROTADO! Curando a Dante y limpiando memoria..."));

	// 1. Curar a Dante (usaremos ApplyDamage pero con valores negativos, un truco de ingenieros)
	if (ObjetivoActual)
	{
		UGameplayStatics::ApplyDamage(ObjetivoActual, -100.0f, GetController(), this, UDamageType::StaticClass());
	}

	// 2. Limpieza de Memoria: Destruimos todos los enemigos base muertos del mapa
	TArray<AActor*> EnemigosEnElMapa;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyBase::StaticClass(), EnemigosEnElMapa);


	for (AActor* ActorEnemigo : EnemigosEnElMapa)
	{
		AEnemyBase* Enemigo = Cast<AEnemyBase>(ActorEnemigo);

		// Añadimos "Enemigo != this" para que el jefe no cometa "suicidio" dentro del bucle
		//cambio 2(state) Antes EstadoActual == EEstadoEnemigo::Muerto
		if (Enemigo && Enemigo != this && Enemigo->EstadoActual.GetObject() == GetEstadoMuerto().GetObject())
		{
			Enemigo->Destroy();
		}
	}


	// 3. Suelta el Manto de Ceniza (Esto lo programaremos después)
	// GetWorld()->SpawnActor(...)

	// Por último, llamamos a la muerte del padre para que el jefe también se destruya
	Super::Morir();
}
