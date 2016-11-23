#include "ContenedorBonus.h"

ContenedorBonus::ContenedorBonus() {
	ultimoId = 0;
}

void ContenedorBonus::aumentarId(){
	this->ultimoId++;
}

void ContenedorBonus::nuevoBonus(int posX, int posY, int tipoBonus) {
	this->aumentarId();
	Bonus nuevo(posX, posY, tipoBonus, ultimoId);


	listaBonus.push_back(nuevo);
}

void ContenedorBonus::buscarActivos(int camaraX, list<Bonus>* listaBonusActivos, list<Bonus>* listaBonusDeBaja) {
	for (list<Bonus>::iterator it = listaBonus.begin(); it != listaBonus.end(); ++it) {
		if(it->estaVisible(camaraX)){
			//it->mover(camaraX);
			listaBonusActivos->push_back((*it));
			//cout << "HAY ENEMIGOS ACTIVOS!" << endl;
		}else if(it->getPosX() < camaraX - 80){ //80 -> tam sprite
			//cout << "BORRO!" << endl;
			//se paso no lo veo mas.. lo elimino de la lista..
			listaBonusDeBaja->push_back((*it));
			it = listaBonusActivos->erase(it);
			it--;
		}
	}
}

void ContenedorBonus::borrarBonus(int id, list<Bonus>* listaBonusDeBaja, list<Bonus>* listaBonusActivos){

	for(list<Bonus>::iterator it = listaBonus.begin(); it != listaBonus.end(); ++it){

		if((*it).getId() == id){
			listaBonusDeBaja->push_back((*it));
			it = listaBonusActivos->erase(it);
			it--;
		}

	}
}

bool ContenedorBonus::detectarColision(list<Bonus>* listaBonusDeBaja, list<Bonus>* listaBonusActivos, list<DibujableServer>* listaPersonajes){
	bool huboColision = false;

	for(list<Bonus>::iterator it = listaBonusActivos->begin(); it != listaBonusActivos->end(); ++it){
		for(list<DibujableServer>::iterator itPjs = listaPersonajes->begin(); itPjs != listaPersonajes->end(); ++itPjs){
			if((it->getPosX() - itPjs->x) <= 40){

				//cout << "Colision con Bonus" << endl;
				huboColision = true;

				borrarBonus(it->getId(), listaBonusDeBaja, listaBonusActivos);

				break;

			}
		}
		if(huboColision){
			break;
		}
	}
	return huboColision;
}
		

ContenedorBonus::~ContenedorBonus() {
}
