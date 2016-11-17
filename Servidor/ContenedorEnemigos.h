#ifndef CONTENEDORENEMIGOS_H_
#define CONTENEDORENEMIGOS_H_

#include <list>
#include "DibujableServerEnemigo.h"

class ContenedorEnemigos {
	private:
		//list<DibujableServerEnemigo> listaEnemigos;
		int ultimoId;
	public:
		list<DibujableServerEnemigo> listaEnemigos;
		ContenedorEnemigos();
		void nuevoEnemigo(int posX, int posY, int tipoEnemigo);
		void buscarActivos(int camaraX, list<DibujableServerEnemigo> *listaEnemigosActivos, list<DibujableServerEnemigo>* listaEnemigosDeBaja);
		bool detectarColision(int camaraX, list<DibujableServerEnemigo>* listaEnemigosActivos, list<DibujableServerEnemigo>* listaEnemigosDeBaja, int xBala, int yBala);
		virtual ~ContenedorEnemigos();
};

#endif /* CONTENEDORENEMIGOS_H_ */
