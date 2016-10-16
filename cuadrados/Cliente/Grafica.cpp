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
	centro = 180;
	centro2=0;

	mTexture = NULL;
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
bool Grafica::init(int ancho, int alto, int numeroCliente) {
	bool exito = true;

	exito = inicializarVentana(ancho , alto, numeroCliente);
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

void Grafica::actualizar(int idObjeto,int x,int y, int spx, int spy){
	/* usar avance de la lista */
	LTexture aux = buscarDibujable(idObjeto);
	aux.actualizar( x,  y, spx, spy);
	actualizarDibujable(aux);
}

void Grafica::mostrarDibujables(){
	SDL_RenderClear( window );

	renderQuad.x = 0 ;
    renderQuad.y = 150 ;
	renderQuad.w = 2000;
	renderQuad.h = 400;

	renderQuad2.x = 30;
	renderQuad2.y = 472;
    renderQuad2.w = 2000;
    renderQuad2.h = 700;

    SDL_RenderCopy (window, fondoText, &renderQuad2, &renderQuad );



    renderQuad.x = centro2 ;
    renderQuad.y = 150 ;
    renderQuad.w = 2000;
    renderQuad.h = 400;

    renderQuad2.x = centro;
    renderQuad2.y = 230;
    renderQuad2.w = 2000;
    renderQuad2.h = 200;



    SDL_RenderCopy (window, fondoText, &renderQuad2 , &renderQuad);


    for (list<LTexture>::iterator i = listaDibujable.begin(); i != listaDibujable.end(); ++i) {
		(*i).render(window, mTexture, anchoVentana/12, altoVentana/10);//si cambian el ancho y alto de 
	}																   //la ventana, el tam de los tipos
																	   //se ajusta con el /10
	//manera fea de mostrar primero al cliente
	list<LTexture>::iterator i = listaDibujable.begin();
	advance(i,numeroCliente - 1);
	(*i).render(window, mTexture, anchoVentana/12, altoVentana/10);

	SDL_RenderPresent( window );
}

void Grafica::nuevoDibujable(int idObjeto, int posX, int posY, int spx, int spy) {
	//if (strcmp(spriteId,"fondo") == 0){
	//nuevo.inicializarTexture("Clark.png");
	//}else

	LTexture nuevo;
	nuevo.setId(idObjeto);
	nuevo.setX(posX);
	nuevo.setY(posY);
	nuevo.setSpX(spx);
	nuevo.setSpY(spy);
	
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
bool Grafica::inicializarVentana(int ancho, int alto, int numeroCliente){
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
	char paath= 'a';
    char *path = &paath;
	inicializarFondo(path);

	string result;
	stringstream sstm;
	sstm << "Clark" << numeroCliente << ".png";
	result = sstm.str();
	cout << "Clark Imagen: " << result << endl;

	loadFromFile(result);
	//loadFromFile("Clarkcopia.png");

	return exito;

}

bool Grafica::inicializarFondo(char *path){
	bool exito = true;


    gFondoSurface = IMG_Load( "fondo-capas.png");

    if( gFondoSurface == NULL ) {
    	cout << "No se pudo cargar la imagen " << "fondo-capas.png" << "SDL Error: " << SDL_GetError() << endl;
    }

    fondoText = SDL_CreateTextureFromSurface(window, gFondoSurface);
    SDL_FreeSurface(gFondoSurface);



	return exito;
}

bool Grafica::inicializarPersonaje(char* path, int ancho, int alto){
	bool exito = true;



	return exito;
}

void Grafica::avanzarCamara (int posicion){

  centro = posicion - 150;
  if (centro < 180)
	  centro= 180;
  renderQuad2.x=centro;

  centro2= centro - posicion;
  renderQuad.x= centro2;
  SDL_RenderCopy (window, fondoText, &renderQuad2, &renderQuad );
  SDL_RenderPresent( window );


}


