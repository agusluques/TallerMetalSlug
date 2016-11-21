#include "ContenedorBalas.h"
#include <list>

ContenedorBalas::ContenedorBalas() {
	ultimoId = 0;
}

void ContenedorBalas::nuevaBala(int posX, int posY, int idUsuario, int direccionDisparo, int tipoDeDisparo){ //5 maquina
	if(ultimoId > 500) ultimoId = 0; //para no usar numeros muy grandes...

	bala nuevaBala(posX,posY,idUsuario, ultimoId += 1,direccionDisparo, tipoDeDisparo);

	listaDeBalas.push_back(nuevaBala);
}

void ContenedorBalas::buscarActivas(int camaraX, list<bala> *listaBalasActivas, list<bala>* listaBalasDeBaja){
	for (list<bala>::iterator it = listaDeBalas.begin(); it != listaDeBalas.end(); ++it) {
		if(it->estaVisible(camaraX)){
			it->mover();
			listaBalasActivas->push_back((*it));
		}else{
			listaBalasDeBaja->push_back((*it));
			it = listaDeBalas.erase(it);
			it--;
		}
	}
}

bool hayColision(int x1, int y1, int x2, int y2){
	//estaria bueno q cada uno tenga su ancho y alto.. y si hay una escala, se aumenta..
	int w1 = 30; int h1 = 40;
	int w2 = 30; int h2 = 40;

	if(x2 + w2 < x1) return false;
	if(x2 > x1 + w1) return false;

	if(y2 + h2 < y1) return false;
	if(y2 > y1 + h1) return false;

	return true;
}

void ContenedorBalas::detectarColisiones(list<bala> *listaBalasDeBaja, list<DibujableServerEnemigo> *listaEnemigosActivos, list<DibujableServerEnemigo> *listaEnemigosDeBaja){
	for (list<DibujableServerEnemigo>::iterator itEnemigos = listaEnemigosActivos->begin(); itEnemigos != listaEnemigosActivos->end(); ++itEnemigos) {
		if(itEnemigos->estaVivo){
			for (list<bala>::iterator itBalas = listaDeBalas.begin(); itBalas != listaDeBalas.end(); ++itBalas) {
				if(itBalas->usr != 5){ //no comparo enemigo contra enemigo
					if(hayColision(itEnemigos->x, itEnemigos->y, itBalas->x, itBalas->y)){
						listaEnemigosDeBaja->push_back((*itEnemigos));
						itEnemigos = listaEnemigosActivos->erase(itEnemigos);
						itEnemigos--;

						listaBalasDeBaja->push_back((*itBalas));
						itBalas = listaDeBalas.erase(itBalas);
						itBalas--;
					}
				}
			}
		}
	}
}

ContenedorBalas::~ContenedorBalas() {
	// TODO Auto-generated destructor stub
}

