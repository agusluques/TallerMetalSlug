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

bool ContenedorBonus::detectarColision(int camaraX, list<Bonus>* listaBonusActivos, list<Bonus>* listaBonusDeBaja, int xPersonaje, int yPersonaje){
	bool huboColision = false;
	for(list<Bonus>::iterator it = listaBonus.begin(); it != listaBonus.end(); ++it){
		if(it->estaVisible(camaraX)){
			if(((it->getPosX() - xPersonaje) <= 10) && (it->getPosY() == yPersonaje)){
				cout << "COLISIOOOOOOOOOOOOOOOOOOOOOON!" << endl;
				listaBonusDeBaja->push_back((*it));
				it = listaBonusActivos->erase(it);
				it--;
				huboColision = true;
			}
		}
	}
	return huboColision;
}
		

ContenedorBonus::~ContenedorBonus() {
}
