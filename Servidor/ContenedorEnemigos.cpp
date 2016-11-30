#include "ContenedorEnemigos.h"
#include "ContenedorBalas.h"


/* TIPO DE ENEMIGOS
 * 1)camina izquierda buscando pegarte
 * 2)dispara
 * 3)parado boludiando.. (camina x ahi.. salta a las plataformas) (tira bombas juego original)
 * 4)parado boludiando.. ve a player y camina a la derecha rajando (cagon)
 * 5)soldados jefe1
 * 6)jefe1
 */

ContenedorEnemigos::ContenedorEnemigos() {
	ultimoId = 4;
}

void ContenedorEnemigos::cargarEnemigosDelNivel(int nivel, int ALTO_VENTANA){
	listaEnemigos.clear();
	ultimoId = 4;

	if (nivel == 1) {
		nuevoEnemigo(750, ALTO_VENTANA - 100, 2);
		nuevoEnemigo(800, ALTO_VENTANA - 100, 3);
		/*nuevoEnemigo(1000, ALTO_VENTANA - 100, 2);
		nuevoEnemigo(1200, ALTO_VENTANA - 100, 3);
		nuevoEnemigo(1400, ALTO_VENTANA - 100, 3);
		nuevoEnemigo(1650, ALTO_VENTANA - 100, 2);
		nuevoEnemigo(1800, ALTO_VENTANA - 100, 3);
		nuevoEnemigo(1850, ALTO_VENTANA - 100, 3);
		nuevoEnemigo(2000, ALTO_VENTANA - 100, 1);
		nuevoEnemigo(2250, ALTO_VENTANA - 100, 4);
		nuevoEnemigo(2500, ALTO_VENTANA - 100, 2);
		nuevoEnemigo(2700, ALTO_VENTANA - 100, 1);
		nuevoEnemigo(3000, ALTO_VENTANA - 100, 4);
		nuevoEnemigo(3300, ALTO_VENTANA - 100, 1);
		nuevoEnemigo(3700, ALTO_VENTANA - 100, 4);
		nuevoEnemigo(4000, ALTO_VENTANA - 100, 2);
		nuevoEnemigo(4050, ALTO_VENTANA - 100, 3);
		nuevoEnemigo(4200, ALTO_VENTANA - 100, 2);
		nuevoEnemigo(4400, ALTO_VENTANA - 100, 3);
		nuevoEnemigo(4600, ALTO_VENTANA - 100, 3);
		nuevoEnemigo(4650, ALTO_VENTANA - 100, 2);
		nuevoEnemigo(4800, ALTO_VENTANA - 100, 3);
		nuevoEnemigo(4950, ALTO_VENTANA - 100, 3);
		nuevoEnemigo(5000, ALTO_VENTANA - 100, 1);
		nuevoEnemigo(5250, ALTO_VENTANA - 100, 4);
		nuevoEnemigo(5300, ALTO_VENTANA - 100, 2);
		nuevoEnemigo(5400, ALTO_VENTANA - 100, 1);
		nuevoEnemigo(5600, ALTO_VENTANA - 100, 4);
		nuevoEnemigo(5800, ALTO_VENTANA - 100, 1);
		nuevoEnemigo(6000, ALTO_VENTANA - 100, 4);
		nuevoEnemigo(6200, ALTO_VENTANA - 100, 2);
		nuevoEnemigo(6250, ALTO_VENTANA - 100, 3);
		nuevoEnemigo(6400, ALTO_VENTANA - 100, 2);
		nuevoEnemigo(6500, ALTO_VENTANA - 100, 3);
		nuevoEnemigo(6700, ALTO_VENTANA - 100, 3);
		nuevoEnemigo(6850, ALTO_VENTANA - 100, 2);
		nuevoEnemigo(7000, ALTO_VENTANA - 100, 3);
		nuevoEnemigo(7150, ALTO_VENTANA - 100, 3);
		nuevoEnemigo(7200, ALTO_VENTANA - 100, 1);
		nuevoEnemigo(7350, ALTO_VENTANA - 100, 4);
		nuevoEnemigo(7400, ALTO_VENTANA - 100, 2);
		nuevoEnemigo(7500, ALTO_VENTANA - 100, 1);
		nuevoEnemigo(7600, ALTO_VENTANA - 100, 4);
		nuevoEnemigo(7700, ALTO_VENTANA - 100, 1);
		nuevoEnemigo(7800, ALTO_VENTANA - 100, 1);
		nuevoEnemigo(7900, ALTO_VENTANA - 100, 4);
		nuevoEnemigo(8000, ALTO_VENTANA - 100, 2);*/
	} else if (nivel == 2) {
		nuevoEnemigo(750, ALTO_VENTANA - 100, 2);
		nuevoEnemigo(800, ALTO_VENTANA - 100, 3);
		/*nuevoEnemigo(1000, ALTO_VENTANA - 100, 2);
		nuevoEnemigo(1200, ALTO_VENTANA - 100, 3);
		nuevoEnemigo(1400, ALTO_VENTANA - 100, 3);
		nuevoEnemigo(1650, ALTO_VENTANA - 100, 2);
		nuevoEnemigo(1800, ALTO_VENTANA - 100, 3);
		nuevoEnemigo(1850, ALTO_VENTANA - 100, 3);
		nuevoEnemigo(2000, ALTO_VENTANA - 100, 1);
		nuevoEnemigo(2250, ALTO_VENTANA - 100, 4);
		nuevoEnemigo(2500, ALTO_VENTANA - 100, 2);
		nuevoEnemigo(2700, ALTO_VENTANA - 100, 1);
		nuevoEnemigo(3000, ALTO_VENTANA - 100, 4);
		nuevoEnemigo(3300, ALTO_VENTANA - 100, 1);
		nuevoEnemigo(3700, ALTO_VENTANA - 100, 4);
		nuevoEnemigo(4000, ALTO_VENTANA - 100, 2);
		nuevoEnemigo(4050, ALTO_VENTANA - 100, 3);
		nuevoEnemigo(4200, ALTO_VENTANA - 100, 2);
		nuevoEnemigo(4400, ALTO_VENTANA - 100, 3);
		nuevoEnemigo(4600, ALTO_VENTANA - 100, 3);
		nuevoEnemigo(4650, ALTO_VENTANA - 100, 2);
		nuevoEnemigo(4800, ALTO_VENTANA - 100, 3);
		nuevoEnemigo(4950, ALTO_VENTANA - 100, 3);
		nuevoEnemigo(5000, ALTO_VENTANA - 100, 1);
		nuevoEnemigo(5250, ALTO_VENTANA - 100, 4);
		nuevoEnemigo(5300, ALTO_VENTANA - 100, 2);
		nuevoEnemigo(5400, ALTO_VENTANA - 100, 1);
		nuevoEnemigo(5600, ALTO_VENTANA - 100, 4);
		nuevoEnemigo(5800, ALTO_VENTANA - 100, 1);
		nuevoEnemigo(6000, ALTO_VENTANA - 100, 4);
		nuevoEnemigo(6200, ALTO_VENTANA - 100, 2);
		nuevoEnemigo(6250, ALTO_VENTANA - 100, 3);
		nuevoEnemigo(6400, ALTO_VENTANA - 100, 2);
		nuevoEnemigo(6500, ALTO_VENTANA - 100, 3);
		nuevoEnemigo(6700, ALTO_VENTANA - 100, 3);
		nuevoEnemigo(6850, ALTO_VENTANA - 100, 2);
		nuevoEnemigo(7000, ALTO_VENTANA - 100, 3);
		nuevoEnemigo(7150, ALTO_VENTANA - 100, 3);
		nuevoEnemigo(7200, ALTO_VENTANA - 100, 1);
		nuevoEnemigo(7350, ALTO_VENTANA - 100, 4);
		nuevoEnemigo(7400, ALTO_VENTANA - 100, 2);
		nuevoEnemigo(7500, ALTO_VENTANA - 100, 1);
		nuevoEnemigo(7600, ALTO_VENTANA - 100, 4);
		nuevoEnemigo(7700, ALTO_VENTANA - 100, 1);
		nuevoEnemigo(7800, ALTO_VENTANA - 100, 1);
		nuevoEnemigo(7900, ALTO_VENTANA - 100, 4);
		nuevoEnemigo(8000, ALTO_VENTANA - 100, 2);*/
	} else if (nivel == 3) {
		nuevoEnemigo(750, ALTO_VENTANA - 100, 2);
		nuevoEnemigo(800, ALTO_VENTANA - 100, 3);
		/*nuevoEnemigo(1000, ALTO_VENTANA - 100, 2);
		nuevoEnemigo(1200, ALTO_VENTANA - 100, 3);
		nuevoEnemigo(1400, ALTO_VENTANA - 100, 3);
		nuevoEnemigo(1650, ALTO_VENTANA - 100, 2);
		nuevoEnemigo(1800, ALTO_VENTANA - 100, 3);
		nuevoEnemigo(1850, ALTO_VENTANA - 100, 3);
		nuevoEnemigo(2000, ALTO_VENTANA - 100, 1);
		nuevoEnemigo(2250, ALTO_VENTANA - 100, 4);
		nuevoEnemigo(2500, ALTO_VENTANA - 100, 2);
		nuevoEnemigo(2700, ALTO_VENTANA - 100, 1);
		nuevoEnemigo(3000, ALTO_VENTANA - 100, 4);
		nuevoEnemigo(3300, ALTO_VENTANA - 100, 1);
		nuevoEnemigo(3700, ALTO_VENTANA - 100, 4);
		nuevoEnemigo(4000, ALTO_VENTANA - 100, 2);
		nuevoEnemigo(4050, ALTO_VENTANA - 100, 3);
		nuevoEnemigo(4200, ALTO_VENTANA - 100, 2);
		nuevoEnemigo(4400, ALTO_VENTANA - 100, 3);
		nuevoEnemigo(4600, ALTO_VENTANA - 100, 3);
		nuevoEnemigo(4650, ALTO_VENTANA - 100, 2);
		nuevoEnemigo(4800, ALTO_VENTANA - 100, 3);
		nuevoEnemigo(4950, ALTO_VENTANA - 100, 3);
		nuevoEnemigo(5000, ALTO_VENTANA - 100, 1);
		nuevoEnemigo(5250, ALTO_VENTANA - 100, 4);
		nuevoEnemigo(5300, ALTO_VENTANA - 100, 2);
		nuevoEnemigo(5400, ALTO_VENTANA - 100, 1);
		nuevoEnemigo(5600, ALTO_VENTANA - 100, 4);
		nuevoEnemigo(5800, ALTO_VENTANA - 100, 1);
		nuevoEnemigo(6000, ALTO_VENTANA - 100, 4);
		nuevoEnemigo(6200, ALTO_VENTANA - 100, 2);
		nuevoEnemigo(6250, ALTO_VENTANA - 100, 3);
		nuevoEnemigo(6400, ALTO_VENTANA - 100, 2);
		nuevoEnemigo(6500, ALTO_VENTANA - 100, 3);
		nuevoEnemigo(6700, ALTO_VENTANA - 100, 3);
		nuevoEnemigo(6850, ALTO_VENTANA - 100, 2);
		nuevoEnemigo(7000, ALTO_VENTANA - 100, 3);
		nuevoEnemigo(7150, ALTO_VENTANA - 100, 3);
		nuevoEnemigo(7200, ALTO_VENTANA - 100, 1);
		nuevoEnemigo(7350, ALTO_VENTANA - 100, 4);
		nuevoEnemigo(7400, ALTO_VENTANA - 100, 2);
		nuevoEnemigo(7500, ALTO_VENTANA - 100, 1);
		nuevoEnemigo(7600, ALTO_VENTANA - 100, 4);
		nuevoEnemigo(7700, ALTO_VENTANA - 100, 1);
		nuevoEnemigo(7800, ALTO_VENTANA - 100, 1);
		nuevoEnemigo(7900, ALTO_VENTANA - 100, 4);
		nuevoEnemigo(8000, ALTO_VENTANA - 100, 2);*/
	}
}

void ContenedorEnemigos::nuevoEnemigo(int posX, int posY, int tipoEnemigo) {
	DibujableServerEnemigo nuevo;
	nuevo.setId(ultimoId += 1); //ver q id le seteo...
	//char* spriteId = parseXMLPj();
	nuevo.setTipoEnemigo(tipoEnemigo);
	nuevo.setX(posX);
	nuevo.setY(posY);
	nuevo.setSpX(-1);
	nuevo.setSpY(5);
	nuevo.setFlip('I');
	nuevo.setVida(2); //energia enemiga
	nuevo.setPunto(5);

	//soldados jefe1
	if(tipoEnemigo == 5) {
		nuevo.estaEnElPiso = false;
		nuevo.setFlip('D');

	}
	if(tipoEnemigo == 6) {
		nuevo.setFlip('D');

	}

	listaEnemigos.push_back(nuevo);
}

void ContenedorEnemigos::buscarActivos(int camaraX, list<DibujableServerEnemigo>* listaEnemigosActivos, list<DibujableServerEnemigo>* listaEnemigosDeBaja, ContenedorBalas *contenedorBalas) {
	for (list<DibujableServerEnemigo>::iterator it = listaEnemigos.begin(); it != listaEnemigos.end(); ++it) {
		if(it->x > camaraX + 800) break; //usar anchoventana.. esta ordenada por posicion la lista..
		if(it->estaVisible(camaraX)){
			int direccion, tipoBala;
			if(it->mover(camaraX, &listaEnemigos, &direccion, &tipoBala)){
				if(tipoBala == 5) //doble bala del tanque
					contenedorBalas->nuevaBala(it->x + 300, it->y, 5, direccion, tipoBala, it->tipoEnemigo); //5 = maquina
				if(tipoBala == 7) //doble bala helicoptero
					contenedorBalas->nuevaBala(it->x + 200, it->y, 5, 7, tipoBala, it->tipoEnemigo); //5 = maquina
				contenedorBalas->nuevaBala(it->x-5, it->y, 5, direccion, tipoBala, it->tipoEnemigo); //5 = maquina
			}
			listaEnemigosActivos->push_back((*it));
			//cout << "HAY ENEMIGOS ACTIVOS!" << endl;
		}else if(it->x < camaraX - 80){ //80 -> tam sprite
			//cout << "BORRO!" << endl;
			//se paso no lo veo mas.. lo elimino de la lista..
			listaEnemigosDeBaja->push_back((*it));
			it = listaEnemigos.erase(it);
			it--;
		}
	}
}

void ContenedorEnemigos::sumarPuntaje(int usrKillAll, list<DibujableServerAdicional*> *listaScores, int modoJuego){
	if(modoJuego == 2){
		for (list<DibujableServerAdicional*>::iterator itScore = listaScores->begin(); itScore != listaScores->end(); ++itScore){
			(*itScore)->aumentar(10);
		}
	} else if(modoJuego == 1){
		for (list<DibujableServerAdicional*>::iterator itScore = listaScores->begin(); itScore != listaScores->end(); ++itScore){
			if((*itScore)->id == usrKillAll){
				(*itScore)->aumentar(10);
			}
		}
	} else if(modoJuego == 3){
		if((usrKillAll == 1) || (usrKillAll == 2)){
			for (list<DibujableServerAdicional*>::iterator itScore = listaScores->begin(); itScore != listaScores->end(); ++itScore){
				if( ((*itScore)->id == 1) || ((*itScore)->id == 2) ){
					(*itScore)->aumentar(10);
				}
			}
		} else if((usrKillAll == 3) || (usrKillAll == 4)){
			for (list<DibujableServerAdicional*>::iterator itScore = listaScores->begin(); itScore != listaScores->end(); ++itScore){
				if( ((*itScore)->id == 3) || ((*itScore)->id == 4) ){
					(*itScore)->aumentar(10);
				}
			}
		}
	}
}

void ContenedorEnemigos::killAll(list<DibujableServerEnemigo>* listaEnemigosActivos, list<DibujableServerEnemigo>* listaEnemigosDeBaja, int usrKillAll, list<DibujableServerAdicional*> *listaScores, int modoJuego){
	for(list<DibujableServerEnemigo>::iterator it = listaEnemigosActivos->begin(); it != listaEnemigosActivos->end(); ++it){
		//listaEnemigosDeBaja->push_back((*it));
		sumarPuntaje(usrKillAll, listaScores, modoJuego);
		int idEnemigo = it->id;
		it->matar();
		//it = listaEnemigosActivos->erase(it);
		//it--;

		bool encontrado = false;
		for(list<DibujableServerEnemigo>::iterator i = listaEnemigos.begin(); i != listaEnemigos.end(); ++i){
			if(i->id == idEnemigo){
				encontrado = true;
				i->matar();
				//i = listaEnemigos.erase(i);
				//i--;
			}
			if(encontrado){
				break;
			}
		}
	}
}

void ContenedorEnemigos::killAll(list<DibujableServerEnemigo>* listaEnemigosActivos, list<DibujableServerEnemigo>* listaEnemigosDeBaja){
	for(list<DibujableServerEnemigo>::iterator it = listaEnemigosActivos->begin(); it != listaEnemigosActivos->end(); ++it){
		int idEnemigo = it->id;
		it->matar();

		bool encontrado = false;
		for(list<DibujableServerEnemigo>::iterator i = listaEnemigos.begin(); i != listaEnemigos.end(); ++i){
			if(i->id == idEnemigo){
				encontrado = true;
				i->matar();
			}
			if(encontrado){
				break;
			}
		}
	}
}

bool ContenedorEnemigos::matarEnemigos(int camaraX, list<DibujableServerEnemigo> listaEnemigosDisparados){
	bool esJefe = false;
	for (list<DibujableServerEnemigo>::iterator it = listaEnemigos.begin(); it != listaEnemigos.end(); ++it) {
		for (list<DibujableServerEnemigo>::iterator it2 = listaEnemigosDisparados.begin(); it2 != listaEnemigosDisparados.end(); ++it2) {
			if(it2->id == it->id){
				//cout << "ENTRA CON: " << it2->bala << endl;
				it->bala = it2->bala;
				esJefe = it->matar();
			}
		}
		if(it->x > camaraX + 850) break;
	}
	return esJefe;
}

void ContenedorEnemigos::iniciarJefe(int camaraX, int nivelActual){
	DibujableServerEnemigo nuevo;
	nuevo.setId(ultimoId += 1); //ver q id le seteo...
	//char* spriteId = parseXMLPj();

	if(nivelActual == 1){
		nuevo.setTipoEnemigo(6);
		nuevo.setY(100);
		nuevo.setVida(10);
	}
	else if(nivelActual == 2) {
		nuevo.setTipoEnemigo(7);
		nuevo.setY(500);
		nuevo.setVida(20);
	}
	else {
		nuevo.setTipoEnemigo(8);
		nuevo.setY(100);
		nuevo.setVida(30);
	}

	nuevo.setX(camaraX + 800);
	nuevo.setSpX(-1);
	nuevo.setSpY(5);
	nuevo.setPunto(150);

	nuevo.ultimoId = ultimoId;

	nuevo.setFlip('D');

	listaEnemigos.push_back(nuevo);
}

ContenedorEnemigos::~ContenedorEnemigos() {
	listaEnemigos.clear();
}
