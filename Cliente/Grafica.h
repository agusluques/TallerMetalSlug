#include <iostream>
#include <string>
#include "Dibujable.h"
#include <list>
#include <cstring>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "LTexture.h"
#include "TextureBalas.h"
#include "TextureBonus.h"
#include "TextureEnergia.h"
#include "LTextureEnemigo.h"
#include "TextureScore.h"
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

	SDL_Texture *soldadosTexture;
	SDL_Texture *energiaTexture;

	SDL_Texture *puntaje;
	SDL_Rect puntajeRect;

	list<LTexture> listaDibujable;
	list<TextureEnergia> listaDibujableEnergia;
	list<TextureScore> listaDibujableScore;
	list<TextureBalas> listaDibujableBalas;
	list<TextureBonus> listaDibujableBonus;
	list<LTextureEnemigo> listaDibujableEnemigos;

	//lo de agus
    SDL_Rect renderQuad;
    SDL_Rect renderQuad2;

	SDL_Window* windowARenderizar;

	int anchoVentana;
	int altoVentana;
    int count;
    int cameraSet;
    int posicionEnergia;

public:
	SDL_Rect camera1;
    SDL_Rect camera2;
    SDL_Rect camera3;
	int numeroCliente;
	SDL_Renderer* window;

	int xCamara;

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

	void actualizar(int idObjeto,int x,int y, int spx, int spy, bool avanzar, char flip, int tipo);
	void quitarEnemigo(int idObjeto);
	void agregarEnergia (int id, int spY, string imagen);

	void mostrarDibujables();
	LTexture buscarDibujable(int id);
	void nuevoDibujable(char* sprite, int idObjeto, int posX, int posY, int spx, int spy, char flip);
	void actualizarDibujable(LTexture nuevo);
	bool actualizarDibujable(int idObjeto,int x,int y, int spx, int spy, char flip);
	bool actualizarDibujableEnemigos(int idObjeto,int x,int y, int spx, int spy, char flip);
	void borrarDibujable(int id);
	void avanzarCamara (int posicionX);

	void clear();

	bool inicializarVentana(int ancho, int alto);
	bool inicializarFondo(char *path1, char* path2, char* path3);
	bool inicializarPersonaje(char* path, int ancho, int alto);

	void inicializarBalas(char* sprite);
	void agregarBala(int x, int y, int cont, bool dirBala, int tipoDisp);
	void borrarBala(int cont);

	void actualizarBalas(int id, int x, int y, int dirBala, int tipoDisp, int spY);
	bool modificarBalas(int id, int x, int y, int dirBala, int tipoDisp, int spY);
	void quitarBalas(int id);

	void inicializarBonus(char* sprite);
	void agregarBonus(int x, int y, int cont, int tipoBonus);
	void borrarBonus(int cont);

	void actualizarBonus(int id, int x, int y, int tipoBonus);
	bool modificarBonus(int id, int x, int y, int tipoBonus);
	void quitarBonus(int id);

	void agregarScores(int id); //inicializa de cero los scores;
	void mostrarScores(int score);
	void actualizarScore(int id, int score);

	void setIdCliente(int numCliente){
		this->numeroCliente = numCliente;
	}

	bool empiezaDeNuevo();

	void setXCamara(int xCamara);

};

#endif /* GRAFICA_H_ */
