#include "ContenedorEnemigos.h"
#include "ContenedorBalas.h"

ContenedorEnemigos::ContenedorEnemigos() {
	ultimoId = 4;
}

/* TIPO DE ENEMIGOS
 * 1)camina izquierda buscando pegarte
 * 2)dispara
 * 3)parado boludiando.. (camina x ahi.. salta a las plataformas) (tira bombas juego original)
 * 4)parado boludiando.. ve a player y camina a la derecha rajando (cagon)
 * 5)soldados jefe1
 * 6)jefe1
 */

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
			if(it->mover(camaraX, &listaEnemigos)){
				contenedorBalas->nuevaBala(it->x-5, it->y, 5, 4, 0); //5 = maquina
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

void ContenedorEnemigos::matarEnemigos(int camaraX, list<DibujableServerEnemigo> listaEnemigosDisparados){
	for (list<DibujableServerEnemigo>::iterator it = listaEnemigos.begin(); it != listaEnemigos.end(); ++it) {
		for (list<DibujableServerEnemigo>::iterator it2 = listaEnemigosDisparados.begin(); it2 != listaEnemigosDisparados.end(); ++it2) {
			if(it2->id == it->id){
				it->matar();
			}
		}
		if(it->x > camaraX + 850) break;
	}
}

void ContenedorEnemigos::iniciarJefe(int camaraX){
	posicionJefe = camaraX + 800;

	DibujableServerEnemigo nuevo;
	nuevo.setId(ultimoId += 1); //ver q id le seteo...
	//char* spriteId = parseXMLPj();
	nuevo.setTipoEnemigo(6);
	nuevo.setX(posicionJefe); //+ ANCHO VENTANA/2
	nuevo.setY(100);
	nuevo.setSpX(-1);
	nuevo.setSpY(5);

	nuevo.ultimoId = ultimoId;

	nuevo.setFlip('D');

	listaEnemigos.push_back(nuevo);
}

ContenedorEnemigos::~ContenedorEnemigos() {
}
