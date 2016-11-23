#ifndef CONTENEDORBONUS_H_
#define CONTENEDORBONUS_H_

#include <list>
#include <iostream>
#include "Bonus.h"
#include "DibujableServer.h"

class ContenedorBonus{
	private:
		int ultimoId;
	public:
		list<Bonus> listaBonus;
		ContenedorBonus();
		void nuevoBonus(int posX, int posY, int tipoBonus);
		void buscarActivos(int camaraX, list<Bonus>* listaBonusActivos, list<Bonus>* listaBonusDeBaja);
		void borrarBonus(int id, list<Bonus>* listaBonusDeBaja, list<Bonus>* listaBonusActivos);
		bool detectarColision(list<Bonus>* listaBonusDeBaja, list<Bonus>* listaBonusActivos, list<DibujableServer>* listaPersonajes);
		void aumentarId();
		virtual ~ContenedorBonus();
};

#endif /* CONTENEDORBONUS_H_ */