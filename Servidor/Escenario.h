/*
 * Escenario.h
 *
 *  Created on: 2/11/2016
 *      Author: pablo
 */

#ifndef SERVIDOR_ESCENARIO_H_
#define SERVIDOR_ESCENARIO_H_

#include <list>
#include "Plataforma.h"
#include <iostream>
#include <pthread.h>

using namespace std;

static int HPLAT_NIVEL1 = 380;
static int HPLAT_NIVEL2 = 240;


class Escenario {

	list<Plataforma> plataformas;

public:

	Plataforma plataforma1;

	Escenario();
	void crearPlataformas();
	bool verificarPlataforma(int spriteX, int spriteY);
	int plataformaActual(int spriteX, int spriteY);
	bool salirPlataforma (int spriteX, int spriteY);
	virtual ~Escenario();
};

#endif /* SERVIDOR_ESCENARIO_H_ */
