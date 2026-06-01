// Archivo: EnemigoEscupidor.cpp

// Importamos nuestro propio encabezado
#include "EnemigoEscupidor.h"
// Para buscar al jugador en el nivel
#include "Kismet/GameplayStatics.h"
// Para controlar el movimiento de la Inteligencia Artificial[cite: 7]
#include "AIController.h"
// Clase base de personajes
#include "GameFramework/Character.h"
// Para manejar los temporizadores del motor[cite: 7]
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"


// Constructor: Configuración inicial
AEnemigoEscupidor::AEnemigoEscupidor()
{
    // Distancia máxima de ataque[cite: 6]
    this->DistanciaParaAtacar = 2000.0f;
    // Distancia mínima para asustarse[cite: 6]
    this->DistanciaParaHuir = 800.0f;
    // Permitir comportamiento de huida[cite: 6, 7]
    this->bHuyeDelJugador = true;
    // 5 balas de vómito en memoria[cite: 6, 7]
    this->TamanoPiscina = 5;

    // Salud balanceada a 20[cite: 1]
    SaludMaxima = 20.0f;
    // Llenar salud
    Salud = SaludMaxima;

    if (GetCharacterMovement())
    {
        // Aumentamos la velocidad máxima al caminar a 600.0f para que pueda escapar de Dante
        GetCharacterMovement()->MaxWalkSpeed = 600.0f;
    }
}

// ARREGLO 1: LAS TRES ETAPAS DE DISTANCIA
void AEnemigoEscupidor::RutinaCerebroTactico()
{
    // Regla de oro: Si está muerto o en medio de la animación de grito, apagamos el cerebro para que no se mueva[cite: 2]
    if (EstadoActual == EEstadoEnemigo::Muerto || EstadoActual == EEstadoEnemigo::Atacando) return;

    // Buscamos a Dante
    ACharacter* Dante = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    // Obtenemos el controlador de movimiento de la IA[cite: 7]
    AAIController* ControladorIA = Cast<AAIController>(GetController());

    // Evitamos crashes si faltan componentes
    if (!Dante || !ControladorIA) return;

    // Calculamos la distancia exacta entre el Escupidor y Dante[cite: 7]
    float Distancia = FVector::Dist(GetActorLocation(), Dante->GetActorLocation());

    // ETAPA 1: HUIR (Demasiado cerca)
    // Le sumamos 50 unidades de "colchón" (Histéresis) para evitar el tartamudeo al girar
    if (this->bHuyeDelJugador && Distancia < (this->DistanciaParaHuir + 50.0f))
    {
        // Llama a la función del padre para escapar[cite: 7]
        HuirDeDante(Dante);
    }
    // ETAPA 2: QUEDARSE Y ATACAR (Zona perfecta)
    else if (Distancia <= this->DistanciaParaAtacar)
    {
        // Frenamos en seco al enemigo[cite: 7]
        ControladorIA->StopMovement();
        // Cambiamos el estado a Atacando para que el cerebro se bloquee (Línea 40)[cite: 2]
        EstadoActual = EEstadoEnemigo::Atacando;
        // Iniciamos la secuencia de disparo
        AtacarJugador();
    }
    // ETAPA 3: ACERCARSE (Fuera de rango)
    else
    {
        // El enemigo camina hacia Dante hasta entrar en la zona perfecta de ataque
        ControladorIA->MoveToActor(Dante, this->DistanciaParaAtacar - 100.0f);
    }
}

// ARREGLO 2: ANIMACIÓN COMPLETA
void AEnemigoEscupidor::AtacarJugador()
{
    // Verificamos si ya podemos disparar
    ACharacter* Dante = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    // Si no hay Dante, salimos
    if (!Dante) return;

    // Apuntado en el eje Z
    FVector DireccionHaciaDante = Dante->GetActorLocation() - GetActorLocation();
    // Giramos hacia el jugador
    FRotator RotacionMirada = FRotator(0.0f, DireccionHaciaDante.Rotation().Yaw, 0.0f);
    // Aplicamos rotación física
    SetActorRotation(RotacionMirada);

    // Creamos una variable para medir cuánto dura exactamente el grito
    float DuracionAnimacion = 1.5f;

    // Si asignamos el Montage en Unreal...
    if (MontageAtaque)
    {
        // La función PlayAnimMontage devuelve automáticamente los segundos que dura el video
        DuracionAnimacion = PlayAnimMontage(MontageAtaque);
    }

    // Usamos la duración exacta del grito para reactivar al enemigo justo al terminar[cite: 4]
    GetWorldTimerManager().SetTimer(TemporizadorAtaque, this, &AEnemyBase::FinalizarAtaque, DuracionAnimacion, false);
}
void AEnemigoEscupidor::DispararDesdeAnimacion()
{
    // Buscamos a Dante en el mundo
    ACharacter* Dante = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (!Dante) return;

    // ¡AQUÍ ESTÁ LA SOLUCIÓN MAGISTRAL!
    // Aumentamos de 100.0f a 250.0f. Esto garantiza que la bala nazca en el aire libre,
    // fuera del cuerpo y la cápsula física del zombi.
    FVector Origen = GetActorLocation() + (GetActorForwardVector() * 250.0f) + FVector(0.0f, 0.0f, 60.0f);

    // Calculamos la rotación
    FRotator RotacionDisparo = (Dante->GetActorLocation() - Origen).Rotation();

    // Disparamos con la función de tu compañero[cite: 6, 7]
    EjecutarDisparo(Origen, RotacionDisparo, 1500.0f, 0.0f);
}