#include "MiniBossPeste.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"

AMiniBossPeste::AMiniBossPeste()
{
    // --- BALANCEO DE DIFICULTAD ---
    SaludMaxima = 75.0f; // Aumentada para que el combate dure
    Salud = SaludMaxima;

    // --- CONFIGURACIÓN DE HABILIDAD ---
    LimiteGolpesParaVomitar = 3;   // Ahora requiere 3 golpes de Dante
    CantidadVomitosPorRafaga = 4;  // Lanza 4 proyectiles creando áreas de veneno

    // --- MOVIMIENTO ---
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = 300.0f; // Más rápido que el promedio
    }

    // Inicializaciones
    bHuyeDelJugador = false;
    TamanoPiscina = 15;
    ContadorGolpesRecibidos = 0;
    ContadorVomitosLanzados = 0;
    bEstaEjecutandoHabilidad = false;
}

void AMiniBossPeste::BeginPlay()
{
    Super::BeginPlay();
}

float AMiniBossPeste::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float DanoReal = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    // Solo sumamos golpes si NO está vomitando (para evitar reinicios o fallos)
    if (Salud > 0.0f && !bEstaEjecutandoHabilidad)
    {
        ContadorGolpesRecibidos++;
        if (ContadorGolpesRecibidos >= LimiteGolpesParaVomitar)
        {
            IniciarPreparacionVomito();
        }
    }
    return DanoReal;
}

void AMiniBossPeste::AtacarJugador()
{
    if (bEstaEjecutandoHabilidad) return;

    OnAtaqueCortaDistanciaBlueprint();
    AEnemyBase::AtacarJugador(); // Llama al abuelo (cuerpo a cuerpo)
    GetWorldTimerManager().SetTimer(TemporizadorZarpazo, this, &AMiniBossPeste::DespertarCerebroBoss, 1.5f, false);
}

void AMiniBossPeste::IniciarPreparacionVomito()
{
    bEstaEjecutandoHabilidad = true;
    ContadorGolpesRecibidos = 0;
    ContadorVomitosLanzados = 0;

    // Detenemos el movimiento físico y la IA
    if (GetCharacterMovement()) GetCharacterMovement()->MaxWalkSpeed = 0.0f;
    AAIController* ControladorIA = Cast<AAIController>(GetController());
    if (ControladorIA) ControladorIA->StopMovement();

    OnPrepararVomitoBlueprint();
    GetWorldTimerManager().SetTimer(TemporizadorPreparacion, this, &AMiniBossPeste::DispararVomitoSecuencial, 2.0f, false);
}

void AMiniBossPeste::DispararVomitoSecuencial()
{
    ACharacter* Dante = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

    if (Dante && EstadoActual != EEstadoEnemigo::Muerto)
    {
        FVector DireccionHaciaDante = Dante->GetActorLocation() - GetActorLocation();
        float DistanciaADante = DireccionHaciaDante.Size();

        // Ajustamos fuerza dinámicamente según la distancia
        float FuerzaDinamica = FMath::Clamp(DistanciaADante * 1.1f, 700.0f, 2500.0f);
        FRotator RotacionApunte = FRotator(30.0f, DireccionHaciaDante.Rotation().Yaw, 0.0f);

        SetActorRotation(FRotator(0.0f, DireccionHaciaDante.Rotation().Yaw, 0.0f));

        FVector Origen = GetActorLocation() + (GetActorForwardVector() * 100.0f) + FVector(0.0f, 0.0f, 150.0f);
        EjecutarDisparo(Origen, RotacionApunte, FuerzaDinamica, 1.5f);
    }

    ContadorVomitosLanzados++;

    if (ContadorVomitosLanzados < CantidadVomitosPorRafaga)
    {
        // Delay de 2.0s entre disparos para que Dante pueda esquivar
        GetWorldTimerManager().SetTimer(TemporizadorRafaga, this, &AMiniBossPeste::DispararVomitoSecuencial, 2.0f, false);
    }
    else
    {
        GetWorldTimerManager().SetTimer(TemporizadorRecuperacion, this, &AMiniBossPeste::FinalizarRecuperacion, 1.5f, false);
    }
}

void AMiniBossPeste::FinalizarRecuperacion()
{
    bEstaEjecutandoHabilidad = false;

    // Forzamos el reinicio de la velocidad para que no se quede "tonto" o lento
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = 300.0f;
    }

    DespertarCerebroBoss();
}

void AMiniBossPeste::DespertarCerebroBoss()
{
    if (EstadoActual != EEstadoEnemigo::Muerto)
    {
        EstadoActual = EEstadoEnemigo::Persiguiendo;
    }
}