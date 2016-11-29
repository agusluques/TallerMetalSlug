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
		void cargarEnemigosDelNivel(int nivel, int ALTO_VENTANA);
		void nuevoEnemigo(int posX, int posY, int tipoEnemigo);
		void buscarActivos(int camaraX, list<DibujableServerEnemigo> *listaEnemigosActivos, list<DibujableServerEnemigo>* listaEnemigosDeBaja, ContenedorBalas *contenedorBalas);
		bool detectarColision(int camaraX, list<DibujableServerEnemigo>* listaEnemigosActivos, list<DibujableServerEnemigo>* listaEnemigosDeBaja, int xBala, int yBala);
		bool matarEnemigos(int camaraX, list<DibujableServerEnemigo> listaEnemigosDisparados);
		void sumarPuntaje(int usrKillAll, list<DibujableServerAdicional*> *listaScores);
		void killAll(list<DibujableServerEnemigo>* listaEnemigosActivos, list<DibujableServerEnemigo>* listaEnemigosDeBaja, int usrKillAll, list<DibujableServerAdicional*> *listaScores);
		void killAll(list<DibujableServerEnemigo>* listaEnemigosActivos, list<DibujableServerEnemigo>* listaEnemigosDeBaja);
		void iniciarJefe(int camaraX);

		virtual ~ContenedorEnemigos();
};

#endif /* CONTENEDORENEMIGOS_H_ */
