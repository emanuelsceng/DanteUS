// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemigoCeniza.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
AEnemigoCeniza::AEnemigoCeniza()
{
	// Ajustamos los atributos según el Nivel 1
	SaludMaxima = 20.0f;
	Salud = SaludMaxima;
	DanoAtaque = 2.0f; // Los soldados de ceniza quitan 2 de vida

	// Le decimos al Padre que la cuenta regresiva para Morir() sea de 2 segundos (lo que dura el grito)
	TiempoDesaparicion = 2.0f;
}

void AEnemigoCeniza::BeginPlay()
{
	// Le decimos al hijo que ejecute el código del padre para que el sensor de visión se active y no nazca ciego.
	Super::BeginPlay();
}
// Sobrescribimos la función de morir para que ejecute la explosión después de 2 segundos
void AEnemigoCeniza::Morir()
{
	// Como el padre ya se encargó de esperar los 2 segundos de la animación, explotamos DE INMEDIATO
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Ceniza: EXPLOSIÓN"));
	}

	// Hacemos aparecer la explosión visual de Niagara
	if (FX_ExplosionCeniza)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FX_ExplosionCeniza, GetActorLocation());
	}

	// 2. Infligimos 5 de daño en área al instante
	UGameplayStatics::ApplyRadialDamage(
		this,
		5.0f, // Daño de la explosión
		GetActorLocation(),
		300.0f, // Radio de la explosión
		UDamageType::StaticClass(),
		TArray<AActor*>(),
		this
	);

	// 3. Finalmente, llamamos a la función Morir() del PADRE (EnemyBase), la cual contiene el Destroy() 
	// para limpiar la memoria dinámica y borrar el actor del nivel.
	AEnemyBase::Morir();
}

