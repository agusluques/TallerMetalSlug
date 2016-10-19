#include <iostream>
#include <string>
#include "Dibujable.h"
#include <list>
#include <cstring>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "LTexture.h"
#include <sstream>

#ifndef GRAFICA_H_
#define GRAFICA_H_

//The dimensions of the level
//const int LEVEL_WIDTH = 2451;
//const int LEVEL_HEIGHT = 300;

//Screen dimension constants
//const int SCREEN_WIDTH = 310;
//const int SCREEN_HEIGHT = 230;

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

	list<LTexture> listaDibujable;


	//lo de agus
	SDL_Rect camera; // = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

    SDL_Rect renderQuad;
    SDL_Rect renderQuad2;

	SDL_Window* windowARenderizar;

	LTexture spriteFondo;
	LTexture spriteFondo2;
	SDL_Texture *fondoText;


	int anchoVentana;
	int altoVentana;

	//fondo camara
	int centro;
	int centro2;

public:
	int numeroCliente;
	SDL_Renderer* window;

	Grafica();
	virtual ~Grafica();
	bool init(int ancho, int alto);
	void close();
	void mostrar();
	bool hayColision(SDL_Rect *a , SDL_Rect *b);
	SDL_Surface* loadSurface( std::string path );
	SDL_Window* getWindow();
	SDL_Surface* getSurface();
	void setSurface(SDL_Surface* surface);

	void actualizar(int idObjeto,int x,int y, int spx, int spy);

	void mostrarDibujables(SDL_Rect camera, float constCamera);
	LTexture buscarDibujable(int id);
	void nuevoDibujable(char* sprite, int idObjeto, int posX, int posY, int spx, int spy);
	void actualizarDibujable(LTexture nuevo);
	void borrarDibujable(int id);
	void avanzarCamara (int avance);

	void clear();

	//lo de agus
	bool inicializarVentana(int ancho, int alto);
	bool inicializarFondo(char *path);
	bool inicializarPersonaje(char* path, int ancho, int alto);

	void setIdCliente(int numCliente){
		this->numeroCliente = numCliente;
	}

};

#endif /* SERVIDOR_H_ */
