#include "Grafica.h"
#include <iostream>
#include <cstring>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

Grafica::Grafica() {
	gWindow = NULL;
	gScreenSurface = NULL;
	fondo1 = NULL;
	fondo2 = NULL;
	fondo3 = NULL;

	windowARenderizar = NULL;
	window = NULL;

	count = 0;

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

//CERRAR SDL Y LIBERAR SURFACE
void Grafica::close() {
	//LIBERO
	SDL_DestroyWindow(windowARenderizar);
	windowARenderizar = NULL;

	SDL_DestroyRenderer(window);
	window = NULL;

	SDL_DestroyTexture(spriteFondo1);
	SDL_DestroyTexture(spriteFondo2);
	SDL_DestroyTexture(spriteFondo3);
	spriteFondo1 = NULL;
	spriteFondo2 = NULL;
	spriteFondo3 = NULL;

	fondo1 = NULL;
	fondo2 = NULL;
	fondo3 = NULL;

	listaDibujable.clear();

	anchoVentana = 0;
	altoVentana = 0;
	count = 0;
	cameraSet = 0;

	camera1.x = 0;
	camera2.x = 0;
	camera3.x = 0;

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
	return fondo3;
}

void Grafica::setSurface(SDL_Surface* surface){
	fondo3 = surface;
}

void Grafica::actualizar(int idObjeto,int x,int y, int spx, int spy, bool avanzar, char flip){
	/* usar avance de la lista */
	LTexture aux = buscarDibujable(idObjeto);
	aux.actualizar( x,  y, spx, spy, flip);

	//if (avanzar)
	//	this->avanzarCamara(x);

	actualizarDibujable(aux);
}

void Grafica::mostrarDibujables(){

	SDL_RenderClear(window);

	SDL_RenderCopy (window, spriteFondo1, &camera1, NULL);
	SDL_RenderCopy (window, spriteFondo2, &camera2, NULL);
	SDL_RenderCopy (window, spriteFondo3, &camera3, NULL);

	for (list<LTexture>::iterator i = listaDibujable.begin(); i != listaDibujable.end(); ++i) {
		(*i).render(window,(*i).texture, xCamara, altoVentana/12);
	}
	list<LTexture>::iterator i = listaDibujable.begin();
	advance(i,numeroCliente - 1);
	(*i).render(window, (*i).texture, xCamara, altoVentana/12);

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

bool Grafica::inicializarFondo(char *path1, char* path2, char* path3){
	//Loading success flag
	bool success = true;

	fondo1 = IMG_Load( path1);

	if( fondo1 == NULL ) {
		cout << "No se pudo cargar la imagen " << "fondo1.png" << "SDL Error: " << SDL_GetError() << endl;
	}

	spriteFondo1 = SDL_CreateTextureFromSurface(window, fondo1);
	SDL_FreeSurface(fondo1);

	camera1.x = 0;
	camera1.y = 0;
	camera1.w = ((anchoVentana*270)/altoVentana);
	camera1.h = 270;


	//Load background texture
	fondo2 = IMG_Load( path2);

	if( fondo2 == NULL ) {
		cout << "No se pudo cargar la imagen " << "fondo2.png" << "SDL Error: " << SDL_GetError() << endl;
	}

	spriteFondo2 = SDL_CreateTextureFromSurface(window, fondo2);
	SDL_FreeSurface(fondo2);

	camera2.x = 0;
	camera2.y = 0;
	camera2.w = ((anchoVentana*270)/altoVentana);
	camera2.h = 270;


	fondo3 = IMG_Load( path3);

	if( fondo3 == NULL ) {
		cout << "No se pudo cargar la imagen " << "fondo3.png" << "SDL Error: " << SDL_GetError() << endl;
	}

	spriteFondo3 = SDL_CreateTextureFromSurface(window, fondo3);
	SDL_FreeSurface(fondo3);

	camera3.x = 0;
	camera3.y = 0;
	camera3.w = ((anchoVentana*270)/altoVentana);
	camera3.h = 270;


	return success;
}

bool Grafica::inicializarPersonaje(char* path, int ancho, int alto){
	bool exito = true;



	return exito;
}

void Grafica::setXCamara(int xCamara){
	if(xCamara == 0){
		camera3.x = 0;
		camera2.x = 0;
		camera1.x = 0;
	}


	if (xCamara < 0)
		xCamara = 0;

	this->xCamara = xCamara;

	if(camera3.x < (4000-camera3.w))
		camera3.x = xCamara*camera3.w/anchoVentana;

	if(camera2.x < (2000-camera2.w))
		camera2.x = (((2000-camera3.w)*camera3.x)/(4000-camera3.w));

	if(camera1.x < (1000-camera1.w))
		camera1.x = (((1000-camera3.w)*camera3.x)/(4000-camera3.w));

}

/*void Grafica::avanzarCamara (int posicionX){
	if (posicionX > cameraSet)
		cameraSet = posicionX;

	camera.x = cameraSet - anchoVentana/2;
	//cout <<"camara.x: " << camera.x << endl;

	if (camera.x < 0)
		camera.x = 0;

	//AVANZAR DE LAS CAMARAS..
	if(camera3.x < (4000-camera3.w))
		camera3.x = camera.x;

	if(camera2.x < (2000-camera2.w))
		camera2.x = (((2000-camera3.w)*camera3.x)/(4000-camera3.w));

	if(camera1.x < (1000-camera1.w))
		camera1.x = (((1000-camera3.w)*camera3.x)/(4000-camera3.w));

}*/

bool Grafica::empiezaDeNuevo () {
	if (camera3.x > 3910){ //3910 un cuarto de pantalla final masomenos..
		int xMin = 9999;
		for (list<LTexture>::iterator i = listaDibujable.begin(); i != listaDibujable.end(); ++i) {
			if (i->xcord < xMin) xMin = i->xcord;
		}
		if (xMin > 7800) {
			//camera.x = 0;
			camera1.x = 0;
			camera2.x = 0;
			camera3.x = 0;
			cameraSet = 0;
			return true;
		}
	}else
		return false;
}
