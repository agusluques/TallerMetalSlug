#ifndef CONTENEDORBONUS_H_
#define CONTENEDORBONUS_H_

#include <list>
#include <iostream>
#include "Bonus.h"

class ContenedorBonus{
	private:
		int ultimoId;
	public:
		list<Bonus> listaBonus;
		ContenedorBonus();
		void nuevoBonus(int posX, int posY, int tipoBonus);
		void buscarActivos(int camaraX, list<Bonus>* listaBonusActivos, list<Bonus>* listaBonusDeBaja);
		bool detectarColision(int camaraX, list<Bonus>* listaBonusActivos, list<Bonus>* listaBonusDeBaja, int xPersonaje, int yPersonaje);
		void aumentarId();
		virtual ~ContenedorBonus();
};

#endif /* CONTENEDORBONUS_H_ */