#ifndef CONTENEDORENEMIGOS_H_
#define CONTENEDORENEMIGOS_H_

#include <list>
#include "DibujableServerEnemigo.h"
#include "ContenedorBalas.h"

class ContenedorEnemigos {
	private:
		//list<DibujableServerEnemigo> listaEnemigos;
		int ultimoId;
		int posicionJefe;
	public:
		list<DibujableServerEnemigo> listaEnemigos;
		ContenedorEnemigos();
		void nuevoEnemigo(int posX, int posY, int tipoEnemigo);
		void buscarActivos(int camaraX, list<DibujableServerEnemigo> *listaEnemigosActivos, list<DibujableServerEnemigo>* listaEnemigosDeBaja, ContenedorBalas *contenedorBalas);
		bool detectarColision(int camaraX, list<DibujableServerEnemigo>* listaEnemigosActivos, list<DibujableServerEnemigo>* listaEnemigosDeBaja, int xBala, int yBala);
		bool matarEnemigos(int camaraX, list<DibujableServerEnemigo> listaEnemigosDisparados);

		void iniciarJefe(int camaraX);

		virtual ~ContenedorEnemigos();
};

#endif /* CONTENEDORENEMIGOS_H_ */
