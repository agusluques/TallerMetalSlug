#include "ContenedorBalas.h"
#include <list>

ContenedorBalas::ContenedorBalas() {
	ultimoId = 0;
}

void ContenedorBalas::nuevaBala(int posX, int posY, int idUsuario, int direccionDisparo, int tipoDeArma, int tipoEnemigo){ //5 maquina
	if(ultimoId > 500) ultimoId = 0; //para no usar numeros muy grandes...
	bala nuevaBala(posX,posY,idUsuario, ultimoId += 1,direccionDisparo, tipoDeArma, tipoEnemigo);

	listaDeBalas.push_back(nuevaBala);
}

void ContenedorBalas::buscarActivas(int camaraX, list<bala> *listaBalasActivas, list<bala>* listaBalasDeBaja){
	for (list<bala>::iterator it = listaDeBalas.begin(); it != listaDeBalas.end(); ++it) {
		if(it->estaVisible(camaraX) && it->visible){
			it->mover();
			listaBalasActivas->push_back((*it));
		}else{
			listaBalasDeBaja->push_back((*it));
			it = listaDeBalas.erase(it);
			it--;
		}
	}
}


bool hayColisionConUser(int xTipo, int yTipo, int xBala, int yBala, int tipoEnemigo){
	int w1 = 30; int h1 = 40;
	int w2 = 5; int h2 = 5;
	int escala = 66;

	//cout << "Y BALA: " << yBala << endl;
	//cout << "Y PERS: " << yTipo << endl;

	if(xBala + w2 < xTipo) return false;
	if(xBala > xTipo + w1) return false;

	if(yBala < yTipo) return false;
	if(yBala > yTipo + 70) return false;

	//if(yBala + h2 < yTipo) return false;
	//if(yBala > yTipo + h1) return false;


	return true;
}


bool hayColision(int xTipo, int yTipo, int xBala, int yBala, int tipoEnemigo, int tipoBala){
	int w1 = 30; int h1 = 40;
	int w2 = 5; int h2 = 5;
	int escala = 60; //altoVentana/10

	switch(tipoBala){
	case 0:
		w2 = 6 + escala;
		h2 = 6 + escala;
		break;

	case 1: //H
		w2 = 25 + escala;
		h2 = 5 + escala;
		break;

	case 2: //R
		w2 = 50 + escala;
		h2 = 8 + escala;

		break;

	case 3: //S
		w2 = 100 + escala;
		h2 = 64 + escala;

		break;

	}

	switch(tipoEnemigo){
	/* 1)camina izquierda buscando pegarte
	 * 2)dispara
	 * 3)parado boludiando.. (camina x ahi.. salta a las plataformas) (tira bombas juego original)
	 * 4)parado boludiando.. ve a player y camina a la derecha rajando (cagon)
	 * 5)soldados jefe1
	 * 6)jefe1
	 * 7)jefe2
	 * 8)jefe3
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

	case 7:
		w1 = 100 + escala + 160;
		h1 = 50 + escala + 90;

		break;

	case 8:
		w1 = 50 + escala + 60;
		h1 = 80 + escala + 80;

		break;

	}

	if(xBala + w2 < xTipo) return false;
	if(xBala > xTipo + w1) return false;

	if(yBala + h2 < yTipo) return false;
	if(yBala > yTipo + h1) return false;


	return true;
}

bool ContenedorBalas::agregarDano (DibujableServer *usuario, int balaEnemiga){

	if(balaEnemiga == 5){ //soldado jefe quita 2
		usuario->aumentarDano(2);
	}
	usuario->aumentarDano(1);

	if(usuario->vida == 10){
		return true;
	}

	return false;

}

void ContenedorBalas::acumuloPuntajes(int puntos, int idJugador, list<DibujableServerAdicional*> *listaScores, int modoJuego){

	if(modoJuego == 2){
		for (list<DibujableServerAdicional*>::iterator itScore = listaScores->begin(); itScore != listaScores->end(); ++itScore){
				(*itScore)->aumentar(puntos);
		}
	} else if(modoJuego == 1){
		for (list<DibujableServerAdicional*>::iterator itScore = listaScores->begin(); itScore != listaScores->end(); ++itScore){
			if((*itScore)->id == idJugador){
				(*itScore)->aumentar(puntos);
			}
		}
	} else if(modoJuego == 3){
		if((idJugador == 1) || (idJugador == 2)){
			for (list<DibujableServerAdicional*>::iterator itScore = listaScores->begin(); itScore != listaScores->end(); ++itScore){
				if( ((*itScore)->id == 1) || ((*itScore)->id == 2) ){
					(*itScore)->aumentar(puntos);
				}
			}
		} else if((idJugador == 3) || (idJugador == 4)){
			for (list<DibujableServerAdicional*>::iterator itScore = listaScores->begin(); itScore != listaScores->end(); ++itScore){
				if( ((*itScore)->id == 3) || ((*itScore)->id == 4) ){
					(*itScore)->aumentar(puntos);
				}
			}
		}
	}
}

void ContenedorBalas::detectarColisionPlataforma (Escenario *escenario){

	for (list<bala>::iterator itBalas = listaDeBalas.begin(); itBalas != listaDeBalas.end(); ++itBalas) {
		if(itBalas->usr != 5){
			if( escenario->colisionPlataformaBalas(itBalas->x,itBalas->y)){
				itBalas->visible = false;
			}
		}
	}

}

void ContenedorBalas::detectarColisiones(list<bala> *listaBalasDeBaja, list<DibujableServerEnemigo> *listaEnemigosActivos, list<DibujableServerEnemigo> *listaEnemigosDeBaja, list<DibujableServerAdicional*> *listaScores, list<DibujableServer*> *listaUsuarios, int modoJuego){
	for (list<DibujableServerEnemigo>::iterator itEnemigos = listaEnemigosActivos->begin(); itEnemigos != listaEnemigosActivos->end(); ++itEnemigos) {
		if(itEnemigos->estaVivo){
			for (list<bala>::iterator itBalas = listaDeBalas.begin(); itBalas != listaDeBalas.end(); ++itBalas) {
				if(itBalas->usr != 5){ //no comparo enemigo contra enemigo
					if(hayColision(itEnemigos->x, itEnemigos->y, itBalas->x, itBalas->y, itEnemigos->tipoEnemigo, itBalas->tipoBala)){

						//itEnemigos->quitarEnergia(1);

						//if(itEnemigos->getVida()==0){
						itEnemigos->bala = itBalas->tipoBala;
						//cout << "TIPO BALA: " << itBalas->tipoBala << endl;
						//cout << "ASIGNO   : " << itEnemigos->bala << endl;
						listaEnemigosDeBaja->push_back((*itEnemigos));
						acumuloPuntajes(itEnemigos->getPunto(), itBalas->usr, listaScores, modoJuego);
						itEnemigos = listaEnemigosActivos->erase(itEnemigos);
						itEnemigos--;
						//}

						itBalas->visible = false;
						//listaBalasDeBaja->push_back((*itBalas));
						//itBalas = listaDeBalas.erase(itBalas);
						//itBalas--;
					}

				}
			}
		}
	} //balas enemigas
	for(list<DibujableServer*>::iterator itUser = listaUsuarios->begin(); itUser != listaUsuarios->end(); ++itUser){
		if((*itUser)->estaOnline && (*itUser)->estaVivo){
			for (list<bala>::iterator itBalas = listaDeBalas.begin(); itBalas != listaDeBalas.end(); ++itBalas) {
				if(itBalas->usr == 5){
					bool murio;
					if(hayColisionConUser((*itUser)->x,(*itUser)->y,itBalas->x,itBalas->y,itBalas->tipoEnemigo)){
						murio = agregarDano((*itUser), itBalas->tipoEnemigo);
						listaBalasDeBaja->push_back((*itBalas));
						itBalas = listaDeBalas.erase(itBalas);
						itBalas--;
					}
					if(murio) (*itUser)->matar();
				}
			}
		}
	}
}

ContenedorBalas::~ContenedorBalas() {
	listaDeBalas.clear();
}

