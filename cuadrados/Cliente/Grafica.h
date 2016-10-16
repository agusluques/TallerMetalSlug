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
	SDL_Renderer* window;
	LTexture spriteFondo;
	SDL_Texture *fondoText;
	SDL_Texture *mTexture;

	//Image dimensions
	int mWidth;
	int mHeight;

	int anchoVentana;
	int altoVentana;

	//fondo camara
	int centro;
	int centro2;

public:
	int numeroCliente;

	Grafica();
	virtual ~Grafica();
	bool init(int ancho, int alto, int numeroCliente);
	void close();
	void mostrar();
	bool hayColision(SDL_Rect *a , SDL_Rect *b);
	SDL_Surface* loadSurface( std::string path );
	SDL_Window* getWindow();
	SDL_Surface* getSurface();
	void setSurface(SDL_Surface* surface);

	void actualizar(int idObjeto,int x,int y, int spx, int spy);

	void mostrarDibujables();
	LTexture buscarDibujable(int id);
	void nuevoDibujable(int idObjeto, int posX, int posY, int spx, int spy);
	void actualizarDibujable(LTexture nuevo);
	void borrarDibujable(int id);
	void avanzarCamara (int avance);


	void clear();

	//lo de agus
	bool inicializarVentana(int ancho, int alto, int numeroCliente);
	bool inicializarFondo(char *path);
	bool inicializarPersonaje(char* path, int ancho, int alto);

	void free() {
		//Free texture if it exists
		if( mTexture != NULL ) {
			SDL_DestroyTexture( mTexture );
			mTexture = NULL;
			mWidth = 0;
			mHeight = 0;
		}
	}

	bool loadFromFile( std::string path ) {
		free();

		//SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
		SDL_Surface* loadedSurface = IMG_Load( "Clarkcopia.png" );

		if( loadedSurface == NULL ) {
			printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
		}
		else {
			//this->mTexture = SDL_CreateTextureFromSurface( window, loadedSurface );
			this->mTexture = IMG_LoadTexture(window, "Clarkcopia.png");
			if( this->mTexture == NULL ) {
				printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
			}
			else {
				//mWidth = loadedSurface->w;
				//mHeight = loadedSurface->h;
			}
			//SDL_FreeSurface( loadedSurface );
		}

		return (mTexture != NULL);
	}

	void setIdCliente(int numCliente){
		this->numeroCliente = numCliente;
	}

};

#endif /* SERVIDOR_H_ */
