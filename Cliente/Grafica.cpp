#include "Grafica.h"
#include <iostream>
#include <cstring>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

Grafica::Grafica() {
	gWindow = NULL;
	gScreenSurface = NULL;
	gCurrentSurface = NULL;
	gFondoSurface = NULL;

	windowARenderizar = NULL;
	window = NULL;

	camera.x = 179;
	camera.y = 204;
	camera.w = ((800*241)/600);
	camera.h = 241;

	/*renderQuad.x= 179;
	renderQuad.y= 204;
	renderQuad.w= ((800*241)/600); //
	renderQuad.h= 241; //446 446-204 = 241*/

	cameraSet = 0;
}

Grafica::~Grafica() {
	// TODO Auto-generated destructor stub
}

using namespace std;

bool Grafica::hayColision(SDL_Rect *a , SDL_Rect *b) {
	if(b->x + b->w < a->x)	return 0;
	if(b->x > a->x + a->w)	return 0;

	if(b->y + b->h < a->y)	return 0;
	if(b->y > a->y + a->h)	return 0;

	return 1;
}

//INICIAR SDL
bool Grafica::init(int ancho, int alto) {
	bool exito = true;

	exito = inicializarVentana(ancho , alto);
	if (!exito){
		cerr<<"Cerrando todo"<<endl;
	}
	anchoVentana = ancho;
	altoVentana = alto;
	return exito;
}

void Grafica::clear() {
	close();
	listaDibujable.clear();
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

void Grafica::actualizar(int idObjeto,int x,int y, int spx, int spy, bool avanzar, char flip){
	/* usar avance de la lista */
	LTexture aux = buscarDibujable(idObjeto);
	aux.actualizar( x,  y, spx, spy, flip);

	if (avanzar)
	   this->avanzarCamara(x);

	actualizarDibujable(aux);
}

void Grafica::mostrarDibujables(){

	SDL_RenderClear( window );

	SDL_RenderCopy (window, spriteFondo2, &camera, &renderQuad2);

	SDL_RenderCopy (window, spriteFondo, &camera, NULL);

    for (list<LTexture>::iterator i = listaDibujable.begin(); i != listaDibujable.end(); ++i) {
		(*i).render(window,(*i).texture, &camera, altoVentana/12);
	}
	list<LTexture>::iterator i = listaDibujable.begin();
	advance(i,numeroCliente - 1);
	(*i).render(window, (*i).texture, &camera, altoVentana/12);

	SDL_RenderPresent( window );
}

void Grafica::nuevoDibujable(char* sprite, int idObjeto, int posX, int posY, int spx, int spy, char flip) {
	//if (strcmp(spriteId,"fondo") == 0){
	//nuevo.inicializarTexture("Clark.png");
	//}else

	LTexture nuevo;
	nuevo.setId(idObjeto);
	nuevo.setX(posX);
	nuevo.setY(posY);
	nuevo.setSpX(spx);
	nuevo.setSpY(spy);
	nuevo.setFlip(flip);

	nuevo.inicializarTexture(window, sprite); //pasar nombre como parametro
	
	listaDibujable.push_back(nuevo);
}

LTexture Grafica::buscarDibujable(int id){
	for (list<LTexture>::iterator it = listaDibujable.begin(); it != listaDibujable.end(); ++it) {
		if ((*it).getId() == id ){
			return (*it);
		}
	}
	return LTexture();
}

void Grafica::actualizarDibujable(LTexture nuevo) {
	for (list<LTexture>::iterator it = listaDibujable.begin(); it != listaDibujable.end(); ++it) {
		if ((*it).getId() == nuevo.getId() ){
			memcpy(&(*it),&nuevo,sizeof(LTexture));
		}
	}
}

void Grafica::borrarDibujable(int id) {
	for (list<LTexture>::iterator it = listaDibujable.begin(); it != listaDibujable.end(); ++it) {
		if ((*it).getId() == id ){
			it = listaDibujable.erase(it);
			it--;
		}
	}
}

//LO DE AGUS
bool Grafica::inicializarVentana(int ancho, int alto){
	bool exito = true;
	window = NULL;
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		exito = false;
		cout << "NO SE PUDO INICIALIZAR SDL: "<< SDL_GetError() <<endl;
	}
	else
	{
		//Create window
		windowARenderizar = SDL_CreateWindow( "METAL SLUG", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ancho, alto, SDL_WINDOW_SHOWN );
		if( windowARenderizar == NULL )
		{
			exito = false;
			cout<< "NO SE PUDO INICIALIZAR LA VENTANA A RENDERIZAR: "<< SDL_GetError() <<endl;
		}else{
			window = SDL_CreateRenderer( windowARenderizar, -1, SDL_RENDERER_ACCELERATED );
			if( window == NULL )
			{
				cout<<"NO SE PUDO CREAR LA VENTANA RENDERIZADA: "<< SDL_GetError() <<endl;
				exito = false;
			}
			else{
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					cout<< "NO SE PUDO INICIALIZAR SOPORTE PNG: "<< IMG_GetError() <<endl;
					exito = false;
				}
			}
		}
	}
	
	
	//loadFromFile("Clarkcopia.png");

	return exito;

}

bool Grafica::inicializarFondo(char *path){
	//Loading success flag
	bool success = true;

	gCurrentSurface = IMG_Load( "fondo1.png");

	if( gCurrentSurface == NULL ) {
		    	cout << "No se pudo cargar la imagen " << "fondo1.png" << "SDL Error: " << SDL_GetError() << endl;
		    }

	spriteFondo = SDL_CreateTextureFromSurface(window, gCurrentSurface);
	SDL_FreeSurface(gCurrentSurface);

	renderQuad.x= 179;
	renderQuad.y= 204;
	renderQuad.w= ((800*241)/600); //
	renderQuad.h= 241; //446 446-204 = 241

	//Load background texture
	gFondoSurface = IMG_Load( "fondo2.png");

	if( gFondoSurface == NULL ) {
	    cout << "No se pudo cargar la imagen " << "fondo2.png" << "SDL Error: " << SDL_GetError() << endl;
	}

	spriteFondo2 = SDL_CreateTextureFromSurface(window, gFondoSurface);
	SDL_FreeSurface(gFondoSurface);

	renderQuad2.x = 0;
	renderQuad2.y = 0;
	renderQuad2.w = 1585;
	renderQuad2.h = 580;

	return success;
}

bool Grafica::inicializarPersonaje(char* path, int ancho, int alto){
	bool exito = true;



	return exito;
}

void Grafica::avanzarCamara (int posicionX){

	if (posicionX > cameraSet)
	   cameraSet = posicionX;

	camera.x = cameraSet - 400;

	if (camera.x < 0)
	  camera.x = 0;

}


