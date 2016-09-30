#include "Grafica.h"
#include <SDL2/SDL.h>

Grafica::Grafica() {
	gWindow = NULL;
	gScreenSurface = NULL;
	gCurrentSurface = NULL;
	gFondoSurface = NULL;
}

Grafica::~Grafica() {
	// TODO Auto-generated destructor stub
}

//DIMENSIONES PANTALLA
const int SCREEN_WIDTH = 200;
const int SCREEN_HEIGHT = 200;

using namespace std;

bool Grafica::hayColision(SDL_Rect *a , SDL_Rect *b) {
	if(b->x + b->w < a->x)	return 0;
	if(b->x > a->x + a->w)	return 0;

	if(b->y + b->h < a->y)	return 0;
	if(b->y > a->y + a->h)	return 0;

	return 1;
}

//INICIAR SDL
bool Grafica::init() {
	bool resultado = true;

	//INICIO SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		cout << "No se puede iniciar SDL. Error: "<< SDL_GetError() << endl;
		resultado = false;
	}
	else {
		//CREO WINDOW
		gWindow = SDL_CreateWindow( "PROBANDO",
				SDL_WINDOWPOS_UNDEFINED,
				SDL_WINDOWPOS_UNDEFINED,
				SCREEN_WIDTH,
				SCREEN_HEIGHT,
				SDL_WINDOW_SHOWN );
		if( gWindow == NULL ) {
			cout << "No se puede crear la ventana. Error : " << SDL_GetError() << endl;
			resultado = false;
		}
		else {
			//CARGO SURFACE DE LA VENTANA
			gScreenSurface = SDL_GetWindowSurface(gWindow);
		}
	}

	return resultado;
}

//CERRAR SDL Y LIBERAR SURFACE
void Grafica::close() {
	//LIBERO SURFACE
	SDL_FreeSurface(gScreenSurface);

	//LIBERO VENTANA
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	//CIERRO SDL
	SDL_Quit();
}

//CARGAR UNA IMAGEN
SDL_Surface* Grafica::loadSurface( std::string path ) {
	//CARGAR IMAGEN
	SDL_Surface* loadedSurface = SDL_LoadBMP( path.c_str() );
	if( loadedSurface == NULL ) {
		cout << "No se pudo cargar la imagen " << path.c_str() << "SDL Error: " << SDL_GetError() << endl;
	}

	return loadedSurface;
}

SDL_Window* Grafica::getWindow(){
	return gWindow;
}

SDL_Surface* Grafica::getSurface(){
	return gCurrentSurface;
}

void Grafica::setSurface(SDL_Surface* surface){
	gCurrentSurface = surface;
}

void Grafica::mensajeUp(int idObjeto){
	Dibujable aux = buscarDibujable(idObjeto);
	if (aux.posicionY() > 0){
		aux.moverY(-6);
		actualizarDibujable(aux);
	}
}

void Grafica::mensajeDown(int idObjeto){
	Dibujable aux = buscarDibujable(idObjeto);
	if (aux.posicionY() <= SCREEN_HEIGHT - 50){
		aux.moverY(6);
		actualizarDibujable(aux);
	}
}

void Grafica::mensajeLeft(int idObjeto){
	Dibujable aux = buscarDibujable(idObjeto);
	if (aux.posicionX() > 0){
		aux.moverX(-6);
		actualizarDibujable(aux);
	}
}

void Grafica::mensajeRight(int idObjeto){
	Dibujable aux = buscarDibujable(idObjeto);
	if (aux.posicionX() <= SCREEN_WIDTH - 50){
		aux.moverX(6);
		actualizarDibujable(aux);
	}
}

void Grafica::mostrar(){
	SDL_UpdateWindowSurface(gWindow);
}

void Grafica::mostrarDibujables(){
	for (list<Dibujable>::iterator i = listaDibujable.begin(); i != listaDibujable.end(); ++i) {
		SDL_BlitSurface( (*i).getSprite(), NULL, gScreenSurface, (*i).getPosicion());
	}

	SDL_UpdateWindowSurface(gWindow);
}

void Grafica::nuevoDibujable(Dibujable nuevo) {
	listaDibujable.push_back(nuevo);
}

Dibujable Grafica::buscarDibujable(int id){
	for (list<Dibujable>::iterator it = listaDibujable.begin(); it != listaDibujable.end(); ++it) {
		if ((*it).getId() == id ){
			return (*it);
		}
	}
	return Dibujable();
}

void Grafica::actualizarDibujable(Dibujable nuevo) {
	for (list<Dibujable>::iterator it = listaDibujable.begin(); it != listaDibujable.end(); ++it) {
		if ((*it).getId() == nuevo.getId() ){
			memcpy(&(*it),&nuevo,sizeof(Dibujable));
		}
	}
}

void Grafica::borrarDibujable(int id) {
	for (list<Dibujable>::iterator it = listaDibujable.begin(); it != listaDibujable.end(); ++it) {
		if ((*it).getId() == id ){
			it = listaDibujable.erase(it);
			it--;
		}
	}
}
