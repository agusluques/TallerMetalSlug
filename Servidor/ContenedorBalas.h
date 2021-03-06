#ifndef CONTENEDORBALAS_H_
#define CONTENEDORBALAS_H_

#include <iostream>
#include <cstring>
#include <string>
#include <ctime>
#include <list>
#include "bala.h"
#include "DibujableServerEnemigo.h"
#include "DibujableServerAdicional.h"
#include "DibujableServer.h"

using namespace std;

class ContenedorBalas {
public:
	list<bala> listaDeBalas;

	int ultimoId;

	ContenedorBalas();
	void nuevaBala(int posX, int posY, int idUsuario, int direccionDisparo, int tipoDeArma, int tipoEnemigo);
	void buscarActivas(int camaraX, list<bala> *listaEnemigosActivos, list<bala>* listaEnemigosDeBaja);
	bool agregarDano (DibujableServer *usuario, int balaEnemiga);
	void detectarColisiones(list<bala> *listaBalasDeBaja, list<DibujableServerEnemigo> *listaEnemigosActivos, list<DibujableServerEnemigo> *listaEnemigosDeBaja, list<DibujableServerAdicional*> *listaScores, list<DibujableServer*> *listaUsuarios, int modoJuego);
	void detectarColisionPlataforma(Escenario *escenario);
	void acumuloPuntajes(int puntos, int idJugador, list<DibujableServerAdicional*> *listaScores, int modoJuego);
	virtual ~ContenedorBalas();
};

#endif /* CONTENEDORBALAS_H_ */
