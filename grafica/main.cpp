#include <iostream>
#include <cstring>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "rapidxml.hpp"
#include "rapidxml_print.hpp"
#include "rapidxml_utils.hpp"
#include "Texture.hpp"

using namespace rapidxml;
using namespace std;
//The dimensions of the level
const int LEVEL_WIDTH = 2451;
const int LEVEL_HEIGHT = 300;

//Screen dimension constants
const int SCREEN_WIDTH = 310;
const int SCREEN_HEIGHT = 230;

//RESOLUCION
int ANCHO;
int ALTO;

SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

SDL_Window* windowARenderizar;
SDL_Renderer* window;
SDL_Rect torzo[6][1];
SDL_Rect pierna[6][1];
LTexture spritePersonaje;
LTexture spriteFondo;
int xcord = 100;
int ycord = 100;
int spX=0;
int spY=0;


bool inicializarVentana(int ancho, int alto){
	bool exito = true;
	window = NULL;
	ANCHO =ancho;
	ALTO = alto;
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
				spritePersonaje.setWindow(window);
				spriteFondo.setWindow(window);

				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					cout<< "NO SE PUDO INICIALIZAR SOPORTE PNG: "<< IMG_GetError() <<endl;
					exito = false;
				}
			}
		}
	}

	return exito;

}


bool inicializarFondo(char* path){
	bool exito = true;

	if (!spriteFondo.loadFromFile(path)){
		cout<< "NO SE PUDO CARGAR EL FONDO"<<endl;
		exito = false;
	}


	return exito;
}

bool inicializarPersonaje(char* path, int ancho, int alto){
	bool exito = true;

	if( !spritePersonaje.loadFromFile( path ) )
	{
		cout<< "NO SE PUDO CARGAR EL SPRITE"<<endl;
		exito = false;
	}
	else
	{

	torzo[0][0].x=12; torzo[0][0].y=401;
    torzo[0][0].w=35; torzo[0][0].h=35;

    torzo[1][0].x=62; torzo[1][0].y=400;
    torzo[1][0].w=35; torzo[1][0].h=35;

    torzo[2][0].x=100; torzo[2][0].y=400;
    torzo[2][0].w=32; torzo[2][0].h=35;

    torzo[3][0].x=136; torzo[3][0].y=400;
    torzo[3][0].w=35; torzo[3][0].h=35;

    torzo[4][0].x=171; torzo[4][0].y=400;
    torzo[4][0].w=32; torzo[4][0].h=35;

    torzo[5][0].x=204; torzo[5][0].y=400;
    torzo[5][0].w=35; torzo[5][0].h=35;

    pierna[0][0].x=15; pierna[0][0].y=436;
	pierna[0][0].w=21; pierna[0][0].h=22;

    pierna[1][0].x=59; pierna[1][0].y=436;
	pierna[1][0].w=25; pierna[1][0].h=18;

    pierna[2][0].x=103; pierna[2][0].y=436;
	pierna[2][0].w=21; pierna[2][0].h=20;

    pierna[3][0].x=135; pierna[3][0].y=436;
	pierna[3][0].w=21; pierna[3][0].h=20;

    pierna[4][0].x=166; pierna[4][0].y=436;
	pierna[4][0].w=28; pierna[4][0].h=18;

	pierna[5][0].x=204; pierna[5][0].y=436;
	pierna[5][0].w=21; pierna[5][0].h=20;
	}

	return exito;
	
}

int cantidadDeHijos(xml_node<> *nodo){
	int c = 0;
	for (xml_node<> *hijo = nodo->first_node(); hijo != NULL; hijo = hijo->next_sibling()){
		c++;
	}

	return c;
}

bool parsearXML(const char* direccion){
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
	}

	return exito;
}

int main(int argc, char const *argv[])
{
	bool exito = true;

	if (argc < 2){
		exito = parsearXML("ej.xml");
	}else{
		exito = parsearXML(argv[1]); 
	}

	if (!exito){
		cerr<<"Cerrando todo"<<endl;
	}
	
	bool salir = false;

	SDL_Event evento;


	while( !salir ){
		//Handle events on queue
		while( SDL_PollEvent( &evento ) != 0 ){
			//User requests quit
			if( evento.type == SDL_QUIT ){
				salir = true;
			} else if (evento.type == SDL_KEYDOWN){
				 switch( evento.key.keysym.sym ){
				 	case SDLK_RIGHT: 
				 		xcord +=3;
				 		spX++;
				 		if (spX == 6) spX=0;
				 		if(xcord >= (camera.x + ANCHO*3/4)){
				 			camera.x += 10;
				 		}
				 		break;
				 }

			}

		SDL_RenderClear( window );
		spriteFondo.render(0, 0 , &camera, 1);
		spritePersonaje.render( xcord, ycord, &torzo[ spX ][0] ,0);
		spritePersonaje.render( xcord, ycord+23, &pierna[spX][0] ,0);
		SDL_RenderPresent( window );	
		}
	}

	return 0;
}