/*
 * Escenario.cpp
 *
 *  Created on: 2/11/2016
 *      Author: pablo
 */

#include "Escenario.h"

using namespace std;

pthread_mutex_t mutexListaEscenario = PTHREAD_MUTEX_INITIALIZER;

Escenario::Escenario() {

	crearPlataformas();

}

void Escenario::crearPlataformas(){

	int idPlataforma = 0;

	Plataforma nueva;
	nueva.setId(idPlataforma);
	nueva.setX(380);
	nueva.setY(360);
	nueva.setLongY(20);
	nueva.setSuelo(350);
	nueva.setLong(440);

	plataformas.push_back(nueva);
	idPlataforma++;

	nueva.setId(idPlataforma);
	nueva.setX(1050);
	nueva.setLong(204);

	plataformas.push_back(nueva);
	idPlataforma++;

     // 3 - id2
    nueva.setId(idPlataforma);
	nueva.setX(1804);
	nueva.setLong(148);

	plataformas.push_back(nueva);
	idPlataforma++;

   //4
    nueva.setId(idPlataforma);
	nueva.setX(2758);
	nueva.setLong(234);

	plataformas.push_back(nueva);
	idPlataforma++;

	//5
    nueva.setId(idPlataforma);
	nueva.setX(3315);
	nueva.setLong(115);

	plataformas.push_back(nueva);
	idPlataforma++;

	//6
    nueva.setId(idPlataforma);
	nueva.setX(3960);
	nueva.setLong(245);

	plataformas.push_back(nueva);

	//plataformas boss
	idPlataforma++;
    nueva.setId(idPlataforma);
	nueva.setX(8010);
	nueva.setLong(158);
	plataformas.push_back(nueva);

	idPlataforma++;
    nueva.setId(idPlataforma);
	nueva.setX(8700);
	nueva.setLong(140);
	plataformas.push_back(nueva);


}

bool Escenario::verificarPlataforma(int spriteX, int spriteY){


	bool estaApoyado = false;

	for (list<Plataforma>::iterator it = plataformas.begin(); it != plataformas.end(); ++it) {
		if (((spriteX >= it->getX()) && (spriteX <= it->getX()+22) && (spriteY < HPLAT_NIVEL1)) || (spriteX <= it->getLong() && spriteX > (it->getLong()-22) && (spriteY < HPLAT_NIVEL1))){
		estaApoyado = true;
		}

	}

	return estaApoyado;

}


bool Escenario::salirPlataforma (int spriteX, int spriteY){

	bool saleDePlataforma = true;

	for (list<Plataforma>::iterator it = plataformas.begin(); it != plataformas.end(); ++it) {

	   if (spriteX > it->getX() && spriteX < it->getLong()){
		 saleDePlataforma = false;
	   }

	}

	return saleDePlataforma;

}

bool Escenario::colisionPlataforma (int cordX, int cordY, bool camIzq, bool camDer){
	bool tocaPlataforma = false;
	for (list<Plataforma>::iterator it = plataformas.begin(); it != plataformas.end(); ++it) {
			if (cordX >= it->getX()-10 && cordX <= it->getLong()+10 && (cordY >= it->getSuelo() || cordY >= it->getSuelo()-30)){
				tocaPlataforma = true;
			}
			if (!tocaPlataforma && (camIzq||camDer)){
				if(cordX+60 >= it->getX() && cordX-58 <= it->getLong()){
					tocaPlataforma =true;
				}
			}
	}

	return tocaPlataforma;
}

bool Escenario::colisionPlataformaBalas (int xBala, int yBala){
	for (list<Plataforma>::iterator itPlat = plataformas.begin(); itPlat != plataformas.end(); ++itPlat){
		if (((xBala >= itPlat->getX()-20) && (xBala <= itPlat->getLong()+80) &&  (yBala > HPLAT_NIVEL2) && (yBala < 455) )){ // || (spriteX <= it->getLong() && spriteX > (it->getLong()-22) && (spriteY < HPLAT_NIVEL1))){
		  return true;
		}
	}
	return false;
}

Escenario::~Escenario() {
	// TODO Auto-generated destructor stub
}

