#ifndef CONTENEDORBALAS_H_
#define CONTENEDORBALAS_H_

#include <iostream>
#include <cstring>
#include <string>
#include <ctime>
#include <list>
#include "bala.h"
#include "DibujableServerEnemigo.h"

using namespace std;

class ContenedorBalas {
public:
	list<bala> listaDeBalas;

	int ultimoId;

	ContenedorBalas();
	void nuevaBala(int posX, int posY, int idUsuario, int direccionDisparo, int tipoDeDisparo);
	void buscarActivas(int camaraX, list<bala> *listaEnemigosActivos, list<bala>* listaEnemigosDeBaja);
	void detectarColisiones(list<bala> *listaBalasDeBaja, list<DibujableServerEnemigo> *listaEnemigosActivos, list<DibujableServerEnemigo> *listaEnemigosDeBaja);

	virtual ~ContenedorBalas();
};

#endif /* CONTENEDORBALAS_H_ */