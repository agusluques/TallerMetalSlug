#include "ContenedorEnemigos.h"

ContenedorEnemigos::ContenedorEnemigos() {
	ultimoId = 4;
}

/* TIPO DE ENEMIGOS
 * 1)camina izquierda buscando pegarte
 * 2)dispara
 * 3)parado boludiando.. (camina x ahi.. salta a las plataformas) (tira bombas juego original)
 * 4)parado boludiando.. ve a player y camina a la derecha rajando (cagon)
 * 5)soldados jefe1
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

void ContenedorEnemigos::buscarActivos(int camaraX, list<DibujableServerEnemigo>* listaEnemigosActivos, list<DibujableServerEnemigo>* listaEnemigosDeBaja) {
	for (list<DibujableServerEnemigo>::iterator it = listaEnemigos.begin(); it != listaEnemigos.end(); ++it) {
		if(it->estaVisible(camaraX)){
			//it->saltar();
			it->mover(camaraX);
			listaEnemigosActivos->push_back((*it));
			//cout << "HAY ENEMIGOS ACTIVOS!" << endl;
		}else if(it->x < camaraX - 80){ //80 -> tam sprite
			//cout << "BORRO!" << endl;
			//se paso no lo veo mas.. lo elimino de la lista..
			listaEnemigosDeBaja->push_back((*it));
			it = listaEnemigosActivos->erase(it);
			it--;
		}
	}
}

bool ContenedorEnemigos::detectarColision(int camaraX, list<DibujableServerEnemigo>* listaEnemigosActivos, list<DibujableServerEnemigo>* listaEnemigosDeBaja, int xBala, int yBala){
	bool huboColision = false;
	for(list<DibujableServerEnemigo>::iterator it = listaEnemigos.begin(); it != listaEnemigos.end(); ++it){
		if(it->estaVisible(camaraX)){
			cout << "X Enemigo: " << it->getX() << endl;
			cout << "Y Enemigo: " << it->getY() << endl;
			cout << "X Bala: " << xBala << endl;
			cout << "Y Bala: " << yBala << endl;
			if(((it->getX() - xBala) <= 10) && (it->getY() == yBala)){
				cout << "COLISIOOOOOOOOOOOOOOOOOOOOOON!" << endl;
				listaEnemigosDeBaja->push_back((*it));
				it = listaEnemigosActivos->erase(it);
				it--;
				huboColision = true;
			}
		}
	}
	return huboColision;
}
		

ContenedorEnemigos::~ContenedorEnemigos() {
}
