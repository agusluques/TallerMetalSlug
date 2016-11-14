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
	nueva.setX(397);
	nueva.setY(360);
	nueva.setLong(436);

	plataformas.push_back(nueva);
	idPlataforma++;

	nueva.setId(idPlataforma);
	nueva.setX(1050);
	nueva.setLong(219);

	plataformas.push_back(nueva);
	idPlataforma++;

     // 3 - id2
    nueva.setId(idPlataforma);
	nueva.setX(1808);
	nueva.setLong(142);

	plataformas.push_back(nueva);
	idPlataforma++;

   //4
    nueva.setId(idPlataforma);
	nueva.setX(2758);
	nueva.setLong(230);

	plataformas.push_back(nueva);
	idPlataforma++;

	//5
    nueva.setId(idPlataforma);
	nueva.setX(3320);
	nueva.setLong(110);

	plataformas.push_back(nueva);
	idPlataforma++;

	//6
    nueva.setId(idPlataforma);
	nueva.setX(3960);
	nueva.setLong(255);

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
		if (spriteX > it->getX() && spriteX < it->getLong() && (spriteY < HPLAT_NIVEL1)){
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

Escenario::~Escenario() {
	// TODO Auto-generated destructor stub
}

