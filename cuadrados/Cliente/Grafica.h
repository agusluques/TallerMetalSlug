#include <iostream>
#include <SDL2/SDL.h>
#include <string>
#include "Dibujable.h"
#include <list>

#ifndef GRAFICA_H_
#define GRAFICA_H_

class Grafica {
private:
	//VENTANA
	SDL_Window* gWindow;
	//SURFACE DE LA VENTANA
	SDL_Surface* gScreenSurface;
	//SURFACE ACTUAL
	SDL_Surface* gCurrentSurface;
	//SURFACE FONDO
	SDL_Surface* gFondoSurface;

	list<Dibujable> listaDibujable;
public:
	Grafica();
	virtual ~Grafica();
	bool init();
	void close();
	void mostrar();
	bool hayColision(SDL_Rect *a , SDL_Rect *b);
	SDL_Surface* loadSurface( std::string path );
	SDL_Window* getWindow();
	SDL_Surface* getSurface();
	void setSurface(SDL_Surface* surface);
	void mensajeUp(int idObjeto);
	void mensajeDown(int idObjeto);
	void mensajeLeft(int idObjeto);
	void mensajeRight(int idObjeto);

	void mostrarDibujables();
	Dibujable buscarDibujable(int id);
	void nuevoDibujable(Dibujable nuevo);
	void actualizarDibujable(Dibujable nuevo);
	void borrarDibujable(int id);
};

#endif /* SERVIDOR_H_ */
