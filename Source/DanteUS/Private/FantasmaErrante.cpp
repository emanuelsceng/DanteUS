#include "FantasmaErrante.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"//para seguir con la mirada y deje usar UGameplayStatics en el super::beginplay();
#include "TimerManager.h"

AFantasmaErrante::AFantasmaErrante()
{
	// Ajustamos estadísticas según el documento
	SaludMaxima = 20.0f;
	Salud = SaludMaxima;
	DanoAtaque = 5.0f;       // Daño cuerpo a cuerpo de 5 puntos
	DistanciaAtaque = 120.0f;

	// Inicialización de la mecánica
	ContadorGolpes = 0;
	bEsInvulnerable = false;
	TiempoInvulnerabilidad = 4.0f;
	// Reducir la velocidad máxima de caminata del fantasma
	GetCharacterMovement()->MaxWalkSpeed =300.0f;
}

void AFantasmaErrante::BeginPlay()
{
	Super::BeginPlay();

	// CAMBIO 1: Ya no asignamos EstadoActual directamente con el UENUM.
	// Super::BeginPlay() (que es EnemyBase::BeginPlay()) ya hace el SpawnActor
	// de todos los estados y llama SetEstado(EstadoInactivo).
	// Solo necesitamos darle el objetivo y forzar la transición a Persiguiendo
	// usando SetEstado() con el getter, igual que hace el libro con SetState().
	
	// Buscamos a Dante directamente para poder seguirlo con la mirada
	APawn* Dante = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (Dante)
	{
		ObjetivoActual = Dante;
		// SetEstado() llama a Salir() del Inactivo e Ingresar() del Persiguiendo.
		// GetEstadoPersiguiendo() es el getter que pusimos en EnemyBase.h.
		SetEstado(GetEstadoPersiguiendo());
	}
}

float AFantasmaErrante::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// Si ya es invulnerable o está muerto, no recibe daño
	
	// CAMBIO 2: En lugar de comparar con el UENUM (EEstadoEnemigo::Muerto),
	// comparamos los punteros de los TScriptInterface usando GetObject().
	// GetObject() devuelve el UObject* subyacente para poder compararlos.
	//antes bEsInvulnerable || EstadoActual == EEstadoEnemigo::Muerto antes
	
	if (bEsInvulnerable || EstadoActual.GetObject() == GetEstadoMuerto().GetObject()) 
	{
		return 0.0f;// Si es invulnerable, devolvemos 0 daño.
	}

	// Ejecutamos la lógica de daño de EnemyBase (resta salud y verifica muerte)
	float DanioProcesado = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	//Si después del golpe sigue vivo, procesamos la cuenta de golpes
	//Contador de golpes. para recordar cuantas veces ha sido atacado
	// CAMBIO 3: Misma comparación con GetObject() para verificar si sigue vivo
	// Antes EstadoActual != EEstadoEnemigo::Muerto 
	if (EstadoActual.GetObject() != GetEstadoMuerto().GetObject())
	{
		ContadorGolpes++;// Incrementamos cada vez que Super::TakeDamage se ejecuta

		// "Tras recibir 2 golpes, se vuelven invencibles durante 4 segundos"
		if (ContadorGolpes >= 2)
		{
			bEsInvulnerable = true;
			ContadorGolpes = 0; // Reiniciamos para la próxima vez

			// Avisamos a la parte visual la activacion del temporizador para ser invulnerable
			OnInvulnerabilidadIniciada();

			// Programamos el fin de la invulnerabilidad(temporizador)
			GetWorldTimerManager().SetTimer(
				TemporizadorInvulnerabilidad,
				this,
				&AFantasmaErrante::RestaurarVulnerabilidad,
				TiempoInvulnerabilidad, //4.0f
				false
			);
		}
	}

	return DanioProcesado;
}

void AFantasmaErrante::RestaurarVulnerabilidad()
{
	bEsInvulnerable = false;
	OnInvulnerabilidadTerminada();// Se llama justo cuando bEsInvulnerable es true
}

//Funcion para seguir con la mirada
void AFantasmaErrante::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Solo mira a Dante si está vivo y tiene objetivo
	// CAMBIO 4: Misma comparación con GetObject() en el Tick.
	//Antes EstadoActual == EEstadoEnemigo::Muerto 
	if (EstadoActual.GetObject() == GetEstadoMuerto().GetObject()) return; 
	if (!ObjetivoActual) return;

	// Misma lógica que el Recolector de Almas
	FVector DireccionADante = ObjetivoActual->GetActorLocation() - GetActorLocation();
	DireccionADante.Z = 0.0f;

	if (!DireccionADante.IsNearlyZero())
	{
		FRotator RotacionObjetivo = DireccionADante.Rotation();
		FRotator RotacionSuave = FMath::RInterpTo(
			GetActorRotation(), RotacionObjetivo, DeltaTime, 6.0f);
		SetActorRotation(RotacionSuave);
	}
}