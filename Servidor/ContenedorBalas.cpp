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

bool hayColision(int xTipo, int yTipo, int xBala, int yBala, int tipoEnemigo){
	/*
	bool colisiono = false;

	int ancho1 = 35; int alto1 = 40;
	int ancho2 = 5; int alto2 = 40;

	cout << "yBala : " << yBala << endl;
	if( yTipo != 500)
	   cout << yTipo << endl;

	if (direccion <= 3){ //x bala positiva
	  if((xBala <= xTipo + ancho1)&&(xBala >= xTipo - ancho2)&&(yBala <= yTipo + alto1)&&(yBala >= yTipo - alto2)){
		  colisiono = true;
		  return colisiono;
	  }
    }else{
    	return false;

    }

	return colisiono;

	*/

	//estaria bueno q cada uno tenga su ancho y alto.. y si hay una escala, se aumenta..
	int w2 = 5; int h2 = 5;
	int w1 = 30; int h1 = 40;

	int escala = 60; //altoVentana/10

	switch(tipoEnemigo){
	/* 1)camina izquierda buscando pegarte
	 * 2)dispara
	 * 3)parado boludiando.. (camina x ahi.. salta a las plataformas) (tira bombas juego original)
	 * 4)parado boludiando.. ve a player y camina a la derecha rajando (cagon)
	 * 5)soldados jefe1
	 * 6)jefe1
	 */
	case 1:
		w1 = 10 + escala;
		h1 = 30 + escala;

		break;
	case 2:
		w1 = 10 + escala;
		h1 = 40 + escala;
		xTipo += 40;

		break;
	case 3:
		w1 = 15 + escala;
		h1 = 40 + escala;

		break;
	case 4:
		w1 = 20 + escala;
		h1 = 40 + escala;

		break;
	case 5:
		w1 = 10 + escala;
		h1 = 40 + escala;
		xTipo += 20 + escala;

		break;
	case 6:
		w1 = 170 + escala + 120;
		h1 = 90 + escala + 100;

		break;

	}

	if(xBala + w2 < xTipo) return false;
	if(xBala > xTipo + w1) return false;

	if(yBala + h2 < yTipo) return false;
	if(yBala > yTipo + h1) return false;


	return true;
}

void ContenedorBalas::acumuloPuntajes(int tipoDeBala, int idJugador, list<DibujableServerAdicional> *listaScores ){
     for (list<DibujableServerAdicional>::iterator itScore = listaScores->begin(); itScore != listaScores->end(); ++itScore){
    	 if(itScore->id == idJugador){
    		 itScore->aumentar(10);
    	 }
     }
}

void ContenedorBalas::detectarColisiones(list<bala> *listaBalasDeBaja, list<DibujableServerEnemigo> *listaEnemigosActivos, list<DibujableServerEnemigo> *listaEnemigosDeBaja, list<DibujableServerAdicional> *listaScores){
	for (list<DibujableServerEnemigo>::iterator itEnemigos = listaEnemigosActivos->begin(); itEnemigos != listaEnemigosActivos->end(); ++itEnemigos) {
		if(itEnemigos->estaVivo){
			for (list<bala>::iterator itBalas = listaDeBalas.begin(); itBalas != listaDeBalas.end(); ++itBalas) {
				if(itBalas->usr != 5){ //no comparo enemigo contra enemigo
					if(hayColision(itEnemigos->x, itEnemigos->y, itBalas->x, itBalas->y, itEnemigos->tipoEnemigo)){
						listaEnemigosDeBaja->push_back((*itEnemigos));
                        acumuloPuntajes(itBalas->tipoBala, itBalas->id, listaScores);
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
	listaDeBalas.clear();
}

