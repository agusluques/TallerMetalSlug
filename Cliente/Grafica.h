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
	SDL_Surface* fondo1;
	SDL_Surface* fondo2;
	SDL_Surface* fondo3;

	SDL_Texture *spriteFondo1;
	SDL_Texture *spriteFondo2;
	SDL_Texture *spriteFondo3;

	list<LTexture> listaDibujable;

	//lo de agus
    SDL_Rect renderQuad;
    SDL_Rect renderQuad2;

	SDL_Window* windowARenderizar;

	int anchoVentana;
	int altoVentana;
    int count;
    int cameraSet;

public:
	SDL_Rect camera; // = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_Rect camera1;
    SDL_Rect camera2;
    SDL_Rect camera3;
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

	void actualizar(int idObjeto,int x,int y, int spx, int spy, bool avanzar, char flip);

	void mostrarDibujables();
	LTexture buscarDibujable(int id);
	void nuevoDibujable(char* sprite, int idObjeto, int posX, int posY, int spx, int spy, char flip);
	void actualizarDibujable(LTexture nuevo);
	void borrarDibujable(int id);
	void avanzarCamara (int posicionX);

	void clear();

	//lo de agus
	bool inicializarVentana(int ancho, int alto);
	bool inicializarFondo(char *path);
	bool inicializarPersonaje(char* path, int ancho, int alto);

	void setIdCliente(int numCliente){
		this->numeroCliente = numCliente;
	}

	bool empiezaDeNuevo();

	void setXCamara(int xCamara, int camSet){
		camera.x = xCamara;
		camera3.x = camera.x;
		camera2.x = (((2000-camera3.w)*camera3.x)/(4000-camera3.w));
		camera1.x = (((1000-camera3.w)*camera3.x)/(4000-camera3.w));

		cameraSet = camSet;
	}

};

#endif /* SERVIDOR_H_ */
