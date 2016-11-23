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

bool ContenedorBonus::detectarColision(list<Bonus>* listaBonusDeBaja, list<Bonus>* listaBonusActivos, list<DibujableServer>* listaPersonajes){
	bool huboColision = false;
	if(!listaBonusActivos->empty()){
		list<Bonus>::iterator it = listaBonusActivos->begin();
		while((it != listaBonusActivos->end()) && (!huboColision)){

			list<DibujableServer>::iterator itPjs = listaPersonajes->begin();
			while((itPjs != listaPersonajes->end()) && (!huboColision)){

				//cout << "X Bonus: " << it->getPosX() << endl;
				//cout << "X Pj   : " << itPjs->x << endl;

				if((it->getPosX() - itPjs->x) <= 40){
				//if(((it->getPosX() - itPjs->x) <= 10) || ((itPjs->x - it->getPosX()) <= 10) && (it->getPosY() == itPjs->y)){
					cout << "ENTRO????" << endl;
					//listaBonusDeBaja->push_back((*it));
					//it = listaBonusActivos->erase(it);
					//it--;
					huboColision = true;
				}
			}
		}
	}
	return huboColision;
}
		

ContenedorBonus::~ContenedorBonus() {
}
