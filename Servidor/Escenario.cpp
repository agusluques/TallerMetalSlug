/*
 * Escenario.cpp
 *
 *  Created on: 2/11/2016
 *      Author: pablo
 */

#include "Escenario.h"

using namespace std;

Escenario::Escenario() {

	crearPlataformas();

}

void Escenario::crearPlataformas(){

	Plataforma nueva;
	nueva.setX(399);
	nueva.setY(360);
	nueva.setLong(450);

	plataforma1 = nueva;

	plataformas.push_back(nueva);


}

bool Escenario::verificarPlataforma(int spriteX, int spriteY){


	bool estaApoyado = false;
	int totalPlataforma = plataforma1.getX() + plataforma1.getLong();


	if (spriteX > plataforma1.getX() && spriteX < totalPlataforma && spriteY < 380){
		cout << "entro a la plataformaaaaaaa" << endl;
		estaApoyado = true;
	}

	return estaApoyado;


}

bool Escenario::salirPlataforma (int spriteX, int spriteY){

	bool saleDePlataforma = false;
	int totalPlataforma = plataforma1.getX() + plataforma1.getLong();

	if (spriteX < plataforma1.getX() || spriteX > totalPlataforma){
		cout << "salee de la plataforma" << endl;
		saleDePlataforma = true;

	}

	return saleDePlataforma;

}

Escenario::~Escenario() {
	// TODO Auto-generated destructor stub
}

