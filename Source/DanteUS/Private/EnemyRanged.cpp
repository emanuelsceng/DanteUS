#include "EnemyRanged.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"

AEnemyRanged::AEnemyRanged()
{
    // Valores por defecto (si una clase hija no los cambia, usará estos)
    this->bHuyeDelJugador = true;
    this->DistanciaParaHuir = 400.0f;
    this->DistanciaParaAtacar = 800.0f;
    this->TamanoPiscina = 10;
}

void AEnemyRanged::BeginPlay()
{
    Super::BeginPlay();

    // Inicializamos el Pool de objetos
    InicializarPiscina();

    // Iniciamos el cerebro (IA) para que piense cada 0.5 segundos
    GetWorldTimerManager().SetTimer(TemporizadorCerebroTactico, this, &AEnemyRanged::RutinaCerebroTactico, 0.5f, true);
}

void AEnemyRanged::RutinaCerebroTactico()
{   //cambio state Antes EstadoActual == EEstadoEnemigo::Muerto
    if (EstadoActual.GetObject() == GetEstadoMuerto().GetObject()) return;

    ACharacter* Dante = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    AAIController* ControladorIA = Cast<AAIController>(GetController());
    if (!Dante || !ControladorIA) return;

    float Distancia = FVector::Dist(GetActorLocation(), Dante->GetActorLocation());

    // 1. Huida (si está muy cerca)
    if (this->bHuyeDelJugador && Distancia < this->DistanciaParaHuir)
    {
        HuirDeDante(Dante);
    }
    // 2. Ataque (si está en rango)
    else if (Distancia <= this->DistanciaParaAtacar)
    {
        ControladorIA->StopMovement(); // Detener movimiento para disparar
        AtacarJugador();
    }
    // 3. PERSECUCIÓN (Aquí está el cambio clave para que se quede lejos)
    else
    {
        // En lugar de acercarse hasta el borde (DistanciaParaAtacar - 100),
        // le decimos que se detenga mucho antes (al 70% de su rango de ataque).
        // Si tu rango es 2000, se detendrá a 1400 de distancia.
        float DistanciaSegura = this->DistanciaParaAtacar * 0.7f;
        ControladorIA->MoveToActor(Dante, DistanciaSegura);
    }
}
void AEnemyRanged::HuirDeDante(ACharacter* Dante)
{
    AAIController* ControladorIA = Cast<AAIController>(GetController());
    if (!ControladorIA) return;

    FVector DireccionEscape = GetActorLocation() - Dante->GetActorLocation();
    DireccionEscape.Normalize();
    FVector PuntoSeguro = GetActorLocation() + (DireccionEscape * 600.0f);

    ControladorIA->MoveToLocation(PuntoSeguro);
}

// Ataque básico (por si el hijo no lo define)
void AEnemyRanged::AtacarJugador()
{
    Super::AtacarJugador();
    FVector Origen = GetActorLocation() + (GetActorForwardVector() * 100.0f);
    EjecutarDisparo(Origen, GetActorRotation(), 1000.0f, 0.0f);
}

// --- GESTIÓN DEL OBJECT POOL (NO TOCAR) ---
void AEnemyRanged::InicializarPiscina()
{
    if (!ClaseProyectil) return;

    for (int32 i = 0; i < this->TamanoPiscina; i++)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.Instigator = GetInstigator();

        AProyectilBase* P = GetWorld()->SpawnActor<AProyectilBase>(ClaseProyectil, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
        if (P)
        {
            P->DesactivarProyectil();
            PiscinaProyectiles.Add(P);
        }
    }
}

AProyectilBase* AEnemyRanged::ObtenerProyectilDisponible()
{
    for (AProyectilBase* P : PiscinaProyectiles)
    {
        if (P && P->IsHidden()) return P;
    }
    return nullptr;
}

void AEnemyRanged::EjecutarDisparo(FVector Origen, FRotator Rotacion, float Velocidad, float Gravedad)
{
    AProyectilBase* P = ObtenerProyectilDisponible();
    if (P)
    {
        P->SetActorLocationAndRotation(Origen, Rotacion);
        P->Disparar(Rotacion.Vector(), Velocidad, Gravedad);
    }
}