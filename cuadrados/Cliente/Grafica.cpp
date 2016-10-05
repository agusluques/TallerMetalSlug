#include "Grafica.h"
#include <iostream>
#include <cstring>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "rapidxml.hpp"
#include "rapidxml_print.hpp"
#include "rapidxml_utils.hpp"

Grafica::Grafica() {
	gWindow = NULL;
	gScreenSurface = NULL;
	gCurrentSurface = NULL;
	gFondoSurface = NULL;

	windowARenderizar = NULL;
	window = NULL;

	mTexture = NULL;
}

Grafica::~Grafica() {
	// TODO Auto-generated destructor stub
}

using namespace rapidxml;
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
	bool exito;

	//aca se recibirian los datos ya parseados
	exito = parsearXML("ej.xml");
	if (!exito){
		cerr<<"Cerrando todo"<<endl;
	}

	return exito;
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

void Grafica::actualizar(int idObjeto,int x,int y){
	LTexture aux = buscarDibujable(idObjeto);
	aux.actualizar( x,  y);
	actualizarDibujable(aux);
}

void Grafica::mostrarDibujables(){
	SDL_RenderClear( window );

	for (list<LTexture>::iterator i = listaDibujable.begin(); i != listaDibujable.end(); ++i) {
		(*i).render(window, mTexture);
	}

	SDL_RenderPresent( window );
}

void Grafica::nuevoDibujable(int idObjeto, int posX, int posY) {
	//if (strcmp(spriteId,"fondo") == 0){
	//nuevo.inicializarTexture("Clark.png");
	//}else

	LTexture nuevo;
	nuevo.setId(idObjeto);
	nuevo.setX(posX);
	nuevo.setY(posY);
	nuevo.inicializarTexture(); //pasar nombre como parametro

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
		windowARenderizar = SDL_CreateWindow( "METAL SLUG", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ancho, alto, SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE );
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

	loadFromFile("Clark.png");

	return exito;

}

bool Grafica::inicializarFondo(char* path){
	bool exito = true;

	/*if (!spriteFondo.loadFromFile(path)){
		cout<< "NO SE PUDO CARGAR EL FONDO"<<endl;
		exito = false;
	}*/

	return exito;
}

bool Grafica::inicializarPersonaje(char* path, int ancho, int alto){
	bool exito = true;

	return exito;
}

int Grafica::cantidadDeHijos(xml_node<> *nodo){
	int c = 0;
	for (xml_node<> *hijo = nodo->first_node(); hijo != NULL; hijo = hijo->next_sibling()){
		c++;
	}

	return c;
}

bool Grafica::parsearXML(const char* direccion){
	bool exito = true;

	file<> xmlFile(direccion);
	xml_document<> doc;    // character type defaults to char
	doc.parse<0>(xmlFile.data());    // 0 means default parse flags

	xml_node<> *ventana = doc.first_node("ventana");
	xml_node<> *ancho = ventana->first_node("ancho");
	xml_node<> *alto = ventana->first_node("alto");

	exito = inicializarVentana(atoi(ancho->value()), atoi(alto->value()));
	if(!exito) cout<<"Fallo en crear ventana"<<endl;

	xml_node<> *capas = doc.first_node("capas");
	xml_node<> *capa1 = capas->first_node("capa");
	xml_node<> *fondoXML = capa1->first_node("imagen_fondo");

	exito = inicializarFondo(fondoXML->value());
	if(!exito) cout<<"Fallo en crear fondo"<<endl;

	xml_node<> *sprites = doc.first_node("sprites");
	xml_node<> *sprite = sprites->first_node();
	xml_node<> *parado = NULL;
	bool encontrado = false;
	while (!encontrado && sprite != NULL){
		if (strcmp(sprite->first_node("id")->value(),"personaje-parado") == 0 ){
			parado = sprite;
			encontrado = true;
		}else{
			sprite = sprite->next_sibling();
		}
	}
	if (encontrado){
		exito = inicializarPersonaje(parado->first_node("path")->value(), atoi(parado->first_node("ancho")->value()),
				atoi(parado->first_node("alto")->value()));
		exito = true;
	}

	return exito;
}

