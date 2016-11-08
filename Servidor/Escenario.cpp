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
	nueva.setX(399);
	nueva.setY(360);
	nueva.setLong(450);

	plataforma1 = nueva;

	plataformas.push_back(nueva);

	idPlataforma++;




}

bool Escenario::verificarPlataforma(int spriteX, int spriteY){


	bool estaApoyado = false;
	//int totalPlataforma = plataforma1.getLong();

	for (list<Plataforma>::iterator it = plataformas.begin(); it != plataformas.end(); ++it) {
		if (spriteX > it->getX() && spriteX < it->getLong() && (spriteY < HPLAT_NIVEL1 || spriteY < HPLAT_NIVEL2)){

	    //if (spriteX > plataforma1.getX() && spriteX < totalPlataforma && (spriteY < HPLAT_NIVEL1 || spriteY < HPLAT_NIVEL2)){
		cout << "entro a la plataformaaaaaaa" << endl;
		estaApoyado = true;
		}
	}

	return estaApoyado;

}

int Escenario::plataformaActual(int spriteX, int spriteY){
	int basePlataforma = 0;

	//int a = pthread_mutex_trylock(&mutexListaEscenario);
	for (list<Plataforma>::iterator it = plataformas.begin(); it != plataformas.end(); ++it)
		if (spriteX > it->getX() && spriteX < it->getLong() && (spriteY < HPLAT_NIVEL1 || spriteY < HPLAT_NIVEL2)){
			cout << "estoy en la plataforma: " << it->getId() << endl;
			cout << " imprime x : " << it->getX() << endl;
			cout << " imprime y : " << it->getY() << endl;
			basePlataforma = it->getY();
			cout << "devuelvo un: " << basePlataforma <<endl;
			break;
		}
	//pthread_mutex_unlock (&mutexListaEscenario);

	return basePlataforma;
}

bool Escenario::salirPlataforma (int spriteX, int spriteY){

	bool saleDePlataforma = false;
	//int totalPlataforma = plataforma1.getLong();

	for (list<Plataforma>::iterator it = plataformas.begin(); it != plataformas.end(); ++it) {

	   if (spriteX < it->getX() || spriteX > it->getLong()){
		 cout << "salee de la plataforma" << endl;
		 saleDePlataforma = true;
	   }

	}

	return saleDePlataforma;

}

Escenario::~Escenario() {
	// TODO Auto-generated destructor stub
}

