#include "ReliquiaDecorator.h"

void UReliquiaDecorator::InicializarDecorador(IAtributosCombateInterface* Envoltura)
{
	ObjetoEnvuelto = Envoltura;
}

float UReliquiaDecorator::CalcularDanioRecibido(float DanioEntrante)
{
	if (ObjetoEnvuelto)
	{
		return ObjetoEnvuelto->CalcularDanioRecibido(DanioEntrante);
	}

	return DanioEntrante;
}