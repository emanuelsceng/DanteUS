#include "EnemyRanged.h"
#include "ProyectilBase.h"

AEnemyRanged::AEnemyRanged()
{
	DistanciaAtaque = 1200.0f;
	TamanoPiscina = 10;
}

void AEnemyRanged::BeginPlay()
{
	Super::BeginPlay();

	if (ClaseProyectil)
	{
		for (int32 i = 0; i < TamanoPiscina; i++)
		{
			FActorSpawnParameters SpawnParams;
			// ARREGLO DE PERSISTENCIA: Dejar en nullptr evita que las balas vivas desaparezcan si el enemigo muere en pleno vuelo
			SpawnParams.Owner = nullptr;
			SpawnParams.Instigator = Cast<APawn>(this);
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			// Ubicación en el cementerio subterráneo para salvaguardar el SensorVision del enemigo
			FVector PosicionCementerio = FVector(0.0f, 0.0f, -10000.0f);

			AProyectilBase* NuevaBala = GetWorld()->SpawnActor<AProyectilBase>(ClaseProyectil, PosicionCementerio, FRotator::ZeroRotator, SpawnParams);

			if (NuevaBala)
			{
				NuevaBala->bUsaObjectPool = true;
				NuevaBala->SetLifeSpan(0.0f); // Inmortalidad inicial en memoria
				NuevaBala->DesactivarProyectil();
				PiscinaProyectiles.Add(NuevaBala);
			}
		}
	}
}

AProyectilBase* AEnemyRanged::ObtenerProyectilDisponible()
{
	AProyectilBase* BalaSustituta = nullptr;

	for (AProyectilBase* Bala : PiscinaProyectiles)
	{
		if (!Bala) continue;

		// Estado Ideal: Encontró munición en reposo
		if (Bala->IsHidden()) return Bala;

		// Si no hay libres, registramos la primera activa por si el cargador colapsa
		if (!BalaSustituta) BalaSustituta = Bala;
	}

	// ARREGLO AGOTAMIENTO DE POOL (Bullet-Hell Pro Fix): Si el Boss vacía el cargador,
	// canibaliza la bala más antigua en el aire para garantizar que el flujo de disparos no se rompa
	if (BalaSustituta)
	{
		BalaSustituta->DesactivarProyectil();
		return BalaSustituta;
	}

	return nullptr;
}

void AEnemyRanged::EjecutarDisparo(FVector Origen, FRotator Rotacion, float Velocidad, float Gravedad)
{
	AProyectilBase* BalaParaUsar = ObtenerProyectilDisponible();
	if (BalaParaUsar)
	{
		BalaParaUsar->ActivarProyectil(Origen, Rotacion, Velocidad, Gravedad);
	}
}

void AEnemyRanged::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// ARREGLO DE RECOLECCIÓN DE PUNTEROS BASURA (Anti-Memory Leak):
	// Destruye físicamente de la memoria RAM del hardware todas las entidades asociadas al pool al morir
	for (AProyectilBase* Bala : PiscinaProyectiles)
	{
		if (Bala && Bala->IsValidLowLevel())
		{
			Bala->Destroy();
		}
	}
	PiscinaProyectiles.Empty();

	Super::EndPlay(EndPlayReason);
}