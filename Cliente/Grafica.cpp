#include "Grafica.h"
#include "TextureEnergia.h"
#include <iostream>
#include <cstring>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

Grafica::Grafica() {
	gWindow = NULL;
	gScreenSurface = NULL;
	fondo1 = NULL;
	fondo2 = NULL;
	fondo3 = NULL;

	soldadosTexture = NULL;
	energiaTexture = NULL;
	puntaje = NULL;

	windowARenderizar = NULL;
	window = NULL;

	count = 0;
	posicionEnergia = -150;

	cameraSet = 0;
	muerto = false;
	esFondoScore = false;
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
	liberar();
	//LIBERO
	SDL_DestroyWindow(windowARenderizar);
	windowARenderizar = NULL;

	SDL_DestroyRenderer(window);
	window = NULL;

	SDL_DestroyTexture(spriteFondo1);
	SDL_DestroyTexture(spriteFondoScore);
	SDL_DestroyTexture(spriteFondo2);
	SDL_DestroyTexture(spriteFondo3);
	SDL_DestroyTexture(soldadosTexture);
	SDL_DestroyTexture(energiaTexture);
	SDL_DestroyTexture(puntaje);
	spriteFondo1 = NULL;
	spriteFondoScore = NULL;
	spriteFondo2 = NULL;
	spriteFondo3 = NULL;
	soldadosTexture = NULL;
	energiaTexture = NULL;
	puntaje = NULL;

	fondo1 = NULL;
	fondo2 = NULL;
	fondo3 = NULL;

	for (list<LTexture*>::iterator it = listaDibujable.begin(); it != listaDibujable.end(); ++it){
		(*it)->free();
		delete (*it);
	}
	listaDibujable.clear();
	for (list<LTextureEnemigo*>::iterator it2 = listaDibujableEnemigos.begin(); it2 != listaDibujableEnemigos.end(); ++it2){
		(*it2)->free();
		delete (*it2);
	}
	listaDibujableEnemigos.clear();
	for (list<TextureBalas*>::iterator it3 = listaDibujableBalas.begin(); it3 != listaDibujableBalas.end(); ++it3){
		(*it3)->free();
		delete (*it3);
	}
	listaDibujableBalas.clear();
	for (list<TextureBonus*>::iterator it4 = listaDibujableBonus.begin(); it4 != listaDibujableBonus.end(); ++it4){
		(*it4)->free();
		delete (*it4);
	}
	listaDibujableBonus.clear();
	for (list<TextureEnergia*>::iterator it5 = listaDibujableEnergia.begin(); it5 != listaDibujableEnergia.end(); ++it5){
		(*it5)->free();
		delete (*it5);
	}
	listaDibujableEnergia.clear();
	for (list<TextureScore*>::iterator it6 = listaDibujableScore.begin(); it6 != listaDibujableScore.end(); ++it6){
		(*it6)->free();
		delete (*it6);
	}
	listaDibujableScore.clear();

	posicionEnergia = -150;

	anchoVentana = 0;
	altoVentana = 0;
	count = 0;
	cameraSet = 0;

	camera1.x = 0;
	camera2.x = 0;
	camera3.x = 0;


	//CIERRO IMG
	IMG_Quit();
	//CIERRO TTF
	TTF_Quit();
	//CIERRO SDL
	SDL_Quit();
}

void Grafica::liberar(){
	SDL_DestroyTexture(spriteFondo1);
	SDL_DestroyTexture(spriteFondo2);
	SDL_DestroyTexture(spriteFondo3);
	SDL_DestroyTexture(puntaje);
	spriteFondo1 = NULL;
	spriteFondo2 = NULL;
	spriteFondo3 = NULL;
	fondo1 = NULL;
	fondo2 = NULL;
	fondo3 = NULL;


	SDL_DestroyTexture(soldadosTexture);
	soldadosTexture = NULL;

	for (list<LTextureEnemigo*>::iterator it2 = listaDibujableEnemigos.begin(); it2 != listaDibujableEnemigos.end(); ++it2){
		(*it2)->free();
		delete (*it2);
	}
	listaDibujableEnemigos.clear();
	for (list<TextureBalas*>::iterator it3 = listaDibujableBalas.begin(); it3 != listaDibujableBalas.end(); ++it3){
		(*it3)->free();
		delete (*it3);
	}
	listaDibujableBalas.clear();
	for (list<TextureBonus*>::iterator it4 = listaDibujableBonus.begin(); it4 != listaDibujableBonus.end(); ++it4){
		(*it4)->free();
		delete (*it4);
	}

	listaDibujableBonus.clear();
	listaDibujableBalas.clear();
	listaDibujableEnemigos.clear();

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

void Grafica::actualizar(int idObjeto,int x,int y, int spx, int spy, bool avanzar, char flip, int tipo){
	//busco si esta en jugadores
	if(!actualizarDibujable(idObjeto,x,y,spx,spy,flip)){
		//no esta en jugadores lo busco en enemigos
		if(!actualizarDibujableEnemigos(idObjeto,x,y,spx,spy,flip)){
			//no lo encontro en enemigos entonces lo creo xq es un nuevo enemigo..
			LTextureEnemigo* nuevo = new LTextureEnemigo;
			nuevo->setId(idObjeto);
			nuevo->setX(x);
			nuevo->setY(y);
			nuevo->setSpX(spx);
			nuevo->setSpY(spy);
			nuevo->setFlip(flip);
			nuevo->tipo = tipo;
			char nombre[] = "soldado.png";
			nuevo->inicializarTexture(window, nombre); //pasar nombre como parametro

			listaDibujableEnemigos.push_back(nuevo);
		}
	}
}

void Grafica::quitarEnemigo(int idObjeto){
	for (list<LTextureEnemigo*>::iterator it = listaDibujableEnemigos.begin(); it != listaDibujableEnemigos.end(); ++it) {
		if((*it)->id == idObjeto){
			(*it)->free();
			delete (*it);
			it = listaDibujableEnemigos.erase(it);
			it--;
		}
	}
}

void Grafica::actualizarBalas(int id, int x, int y, int dirBala, int tipoDisp, int spY){
	if(!modificarBalas(id, x, y, dirBala, tipoDisp, spY)){
		//cout<<"CREA BALA NUEVAAAAAAA"<<endl;
		TextureBalas* nueva = new TextureBalas;
		nueva->setId(id);
		nueva->setX(x);
		nueva->setY(y);
		nueva->setSpX(dirBala);
		nueva->setTipoDisparo(tipoDisp);
		nueva->setSpY(spY);

		char pathBalas[] = "img/balas/balas.png";
		nueva->inicializarTexture(window, pathBalas);

		listaDibujableBalas.push_back(nueva);
	}

}

bool Grafica::modificarBalas(int id, int x, int y, int dirBala, int tipoDisp, int spY){
	bool actualizo = false;

	for (list<TextureBalas*>::iterator it = listaDibujableBalas.begin(); it != listaDibujableBalas.end(); ++it) {
		if ( (*it)->id == id ){
			(*it)->actualizar( x, y, dirBala, tipoDisp, spY);
			actualizo = true;
		}
	}

	return actualizo;
}

void Grafica::quitarBalas(int id){
	for (list<TextureBalas*>::iterator it = listaDibujableBalas.begin(); it != listaDibujableBalas.end(); ++it) {
		if((*it)->id == id){
			(*it)->free();
			delete (*it);
			it = listaDibujableBalas.erase(it);
			it--;
		}
	}
}

void Grafica::agregarEnergia(int id, int spY, string imagen){
	bool esta = false;

	for (list<TextureEnergia*>::iterator it = listaDibujableEnergia.begin(); it != listaDibujableEnergia.end(); ++it) {
		if((*it)->id == id){
			char nombre[14];
			strcpy(nombre, imagen.c_str());
			(*it)->inicializarTexture(window, nombre);
			esta = true;
			break;
		}
	}

	if(!esta){
		TextureEnergia* nuevo = new TextureEnergia;
		posicionEnergia +=150;
		nuevo->setId(id);
		nuevo->setPos(posicionEnergia);

		char nombre[14];
		strcpy(nombre, imagen.c_str());

		nuevo->inicializarTexture(window, nombre);

		listaDibujableEnergia.push_back(nuevo);
	}
}

void Grafica::agregarScores (int id){
	bool existe = false;

	for (list<TextureScore*>::iterator it = listaDibujableScore.begin(); it != listaDibujableScore.end(); ++it) {
		if((*it)->id == id){
			existe = true;
			break;
		}
	}

	if(!existe){
		TextureScore* nuevo = new TextureScore;
		nuevo->setId(id);
		nuevo->setX(posicionEnergia+20);
		nuevo->setY(35);

		if (id==1){
			SDL_Color color1 = {85,170,230,255}; //celeste
			nuevo->setColor(color1);
		}
		if( id==2){
			SDL_Color color2 = {0,0,255,255}; //azul
			nuevo->setColor(color2);
		}
		if (id ==3){
			SDL_Color color3 = {255,255,255,255}; //blanco
			nuevo->setColor(color3);
		}
		if (id == 4){
			SDL_Color color4 = {0,0,0,255}; //negro
			nuevo->setColor(color4);
		}

		string aMostrar = "0";

		nuevo->inicializarTexture(window, aMostrar);

		listaDibujableScore.push_back(nuevo);
	}
}

void Grafica::actualizarScore(int id, int score){
	for (list<TextureScore*>::iterator it = listaDibujableScore.begin(); it != listaDibujableScore.end(); ++it) {
		if((*it)->id == id){
			(*it)->setAumentable(score);
			string numero = "";
			numero = static_cast<ostringstream*>(&(ostringstream() << score))->str();
			(*it)->inicializarTexture(window,numero);
			//(*it)->renderScore(window);
		}
	}
}


void Grafica::morir(){
	this->muerto = true;
}
void Grafica::actualizarVida(int id,int vida){
	for (list<TextureEnergia*>::iterator it = listaDibujableEnergia.begin(); it != listaDibujableEnergia.end(); ++it) {
		if((*it)->id == id){
			if (vida == 10) morir();
			(*it)->setSpY(vida);
			(*it)->renderEnergia(window, altoVentana/20);
		}
	}
}

bool Grafica::estaMuerto(){
	return this->muerto;
}



void Grafica::setearFondoScore(bool estado){
	esFondoScore = estado;
}

void Grafica::agregarBonus(int x, int y, int cont, int tipoBonus){
	//Busco en la lista con el ID de bonus si esta actualizo sino agrego
	if(listaDibujableBonus.empty()){
		TextureBonus* nuevo = new TextureBonus;
		nuevo->setX(x);
		nuevo->setY(y);
		nuevo->setId(cont);
		nuevo->setTipo(tipoBonus);
		string pathBonus = "img/bonus/bonus.png";
		nuevo->inicializarTexture(window, &pathBonus[0]);

		listaDibujableBonus.push_back(nuevo);
	} else {
		bool encontrado = false;
		for(list<TextureBonus*>::iterator j = listaDibujableBonus.begin(); j!= listaDibujableBonus.end(); ++j){
			if((**j).id == cont){
				encontrado = true;
				(**j).xcord = x;
				(**j).ycord = y;
			}
		}
		if(!encontrado){
			TextureBonus* nuevo = new TextureBonus;
			nuevo->setX(x);
			nuevo->setY(y);
			nuevo->setId(cont);
			nuevo->setTipo(tipoBonus);
			string pathBonus = "img/bonus/bonus.png";
			nuevo->inicializarTexture(window, &pathBonus[0]);

			listaDibujableBonus.push_back(nuevo);
		}
	}
}

void Grafica::borrarBonus(int cont){
	for(list<TextureBonus*>::iterator j = listaDibujableBonus.begin(); j!= listaDibujableBonus.end(); ++j){
		if((**j).id == cont){
			(*j)->free();
			delete (*j);
			j = listaDibujableBonus.erase(j);
			j--;
		}
	}
}

void Grafica::actualizarBonus(int id, int x, int y, int tipoBonus){
	if(!modificarBonus(id, x, y, tipoBonus)){
		//cout << "Creando nuevo bonus" << endl;
		TextureBonus* nueva = new TextureBonus;
		nueva->setId(id);
		nueva->setX(x);
		nueva->setY(y);
		nueva->setTipo(tipoBonus);

		char pathBonus[] = "img/bonus/bonus.png";
		nueva->inicializarTexture(window, pathBonus);

		listaDibujableBonus.push_back(nueva);
	}

}

bool Grafica::modificarBonus(int id, int x, int y, int tipoBonus){
	bool actualizo = false;

	for (list<TextureBonus*>::iterator it = listaDibujableBonus.begin(); it != listaDibujableBonus.end(); ++it) {
		//cout << "IT->ID: " << it->id << endl;
		//cout << "ID: " << id << endl;
		if ( (*it)->id == id ){
			(*it)->actualizar( x, y);
			actualizo = true;
		}
	}
	return actualizo;
}

void Grafica::quitarBonus(int id){
	for (list<TextureBonus*>::iterator it = listaDibujableBonus.begin(); it != listaDibujableBonus.end(); ++it) {
		if((*it)->id == id){
			(*it)->free();
			delete (*it);
			it = listaDibujableBonus.erase(it);
			it--;
		}
	}
}

void Grafica::agregarBala(int x, int y, int cont, bool dirBala, int tipoDisp){
	//Busco en la lista con el ID de bala si esta actualizo sino agrego
	if(listaDibujableBalas.empty()){
		TextureBalas* nuevo = new TextureBalas;
		nuevo->setX(x);
		nuevo->setY(y);
		nuevo->setTipoArma(2);
		nuevo->setId(cont);
		nuevo->setTipoDisparo(tipoDisp);
		if(dirBala == true){
			nuevo->setFlip('D');
		} else {
			nuevo->setFlip('I'); 
		}
		string pathBalas = "img/balas/balas.png";
		nuevo->inicializarTexture(window, &pathBalas[0]);

		listaDibujableBalas.push_back(nuevo);
	} else {
		bool encontrado = false;
		for(list<TextureBalas*>::iterator j = listaDibujableBalas.begin(); j!= listaDibujableBalas.end(); ++j){
			if((**j).id == cont){
				encontrado = true;
				(**j).xcord = x;
				(**j).ycord = y;
			}
		}
		if(!encontrado){
			TextureBalas* nuevo = new TextureBalas;
			nuevo->setX(x);
			nuevo->setY(y);
			//cout << "YYYY: " << y << endl;
			nuevo->setTipoArma(2);
			nuevo->setId(cont);
			nuevo->setTipoDisparo(tipoDisp);
			if(dirBala == true){
				nuevo->setFlip('D');
			} else {
				nuevo->setFlip('I'); 
			}
			string pathBalas = "img/balas/balas.png";
			nuevo->inicializarTexture(window, &pathBalas[0]);

			listaDibujableBalas.push_back(nuevo);
		} else {

		}
	}
}

void Grafica::borrarBala(int cont){
	for(list<TextureBalas*>::iterator j = listaDibujableBalas.begin(); j!= listaDibujableBalas.end(); ++j){
		if((**j).id == cont){
			(*j)->free();
			delete (*j);
			j = listaDibujableBalas.erase(j);
			j--;
		}
	}
}

struct Comparator {
    bool operator()(const TextureScore* o1, const TextureScore* o2) const {
        return o1->aumentable > o2->aumentable;
    }
};

void Grafica::mostrarDibujables(){
	//bool finFondo1, finFondo2, finFondo3;
	SDL_RenderClear(window);

	if(!esFondoScore){
		SDL_RenderCopy (window, spriteFondo1, &camera1, NULL);
		SDL_RenderCopy (window, spriteFondo2, &camera2, NULL);
		SDL_RenderCopy (window, spriteFondo3, &camera3, NULL);
	}else{

		listaDibujableScore.sort(Comparator());


		font = TTF_OpenFont("OpenSans-ExtraBold.ttf",50);
		SDL_Color color = {249, 249, 16, 1};
		SDL_Surface* superficie = TTF_RenderText_Solid(font, "SCORES",color);
		puntaje = SDL_CreateTextureFromSurface( window, superficie);
		puntajeRect.x = 270;
		puntajeRect.y = 90;
		SDL_QueryTexture(puntaje, NULL, NULL, &puntajeRect.w, &puntajeRect.h);
		SDL_RenderCopy(window, puntaje, NULL, &puntajeRect);
		SDL_FreeSurface(superficie);
		SDL_DestroyTexture(puntaje);
		int y = 130;
		for (list<TextureScore*>::iterator it = listaDibujableScore.begin(); it != listaDibujableScore.end(); ++it) {
			std::stringstream ss, ss2;
			ss << (*it)->id;
			ss2 << (*it)->aumentable;
			string str = ss.str();
			string str2 = ss2.str();
			string texto = "Jugador " + str + " : " + str2 + " puntos";
			const char * c = texto.c_str();
			SDL_Color color = {255, 255, 255, 1};
			SDL_Surface* superficie = TTF_RenderText_Solid(font, c,color);
			puntaje = SDL_CreateTextureFromSurface( window, superficie);
			puntajeRect.x = 50;
			puntajeRect.y = y + 70;
			y = y+70;
			SDL_QueryTexture(puntaje, NULL, NULL, &puntajeRect.w, &puntajeRect.h);
			SDL_RenderCopy(window, puntaje, NULL, &puntajeRect);
			SDL_FreeSurface(superficie);
			SDL_DestroyTexture(puntaje);

		}
		TTF_CloseFont(font);
	}
	

	for (list<TextureEnergia*>::iterator it = listaDibujableEnergia.begin(); it != listaDibujableEnergia.end(); ++it) {
		(*it)->renderEnergia(window, altoVentana/20);
	}

	for (list<TextureScore*>::iterator it = listaDibujableScore.begin(); it != listaDibujableScore.end(); ++it) {
		(*it)->renderScore(window);
	}

	for (list<LTextureEnemigo*>::iterator it = listaDibujableEnemigos.begin(); it != listaDibujableEnemigos.end(); ++it) {
		(*it)->render(window, soldadosTexture, xCamara, altoVentana/10);
	}

	for(list<TextureBalas*>::iterator j = listaDibujableBalas.begin(); j!= listaDibujableBalas.end(); ++j){
		(**j).render(window,(**j).texture, xCamara, altoVentana/48);
	}

	for (list<TextureBonus*>::iterator it2 = listaDibujableBonus.begin(); it2 != listaDibujableBonus.end(); ++it2)
	{
		(**it2).render(window, (**it2).texture, xCamara, altoVentana/15);
	}
	for (list<LTexture*>::iterator i = listaDibujable.begin(); i != listaDibujable.end(); ++i) {
		(**i).render(window,(**i).texture, xCamara, altoVentana/12);
	}

	list<LTexture*>::iterator i = listaDibujable.begin();
	advance(i,numeroCliente - 1);
	(**i).render(window, (**i).texture, xCamara, altoVentana/12);

	//SDL_RenderCopy(window, puntaje, NULL, &puntajeRect);

	SDL_RenderPresent( window );
}

void Grafica::nuevoDibujable(char* sprite, int idObjeto, int posX, int posY, int spx, int spy, char flip) {
	//if (strcmp(spriteId,"fondo") == 0){
	//nuevo.inicializarTexture("Clark.png");
	//}else

	LTexture* nuevo = new LTexture;
	nuevo->setId(idObjeto);
	nuevo->setX(posX);
	nuevo->setY(posY);
	nuevo->setSpX(spx);
	nuevo->setSpY(spy);
	nuevo->setFlip(flip);

	nuevo->inicializarTexture(window, sprite); //pasar nombre como parametro

	listaDibujable.push_back(nuevo);
}

LTexture* Grafica::buscarDibujable(int id){
	for (list<LTexture*>::iterator it = listaDibujable.begin(); it != listaDibujable.end(); ++it) {
		if ((**it).getId() == id ){
			return (*it);
		}
	}
	return new LTexture();
}

void Grafica::actualizarDibujable(LTexture nuevo) {
	for (list<LTexture*>::iterator it = listaDibujable.begin(); it != listaDibujable.end(); ++it) {
		if ((**it).getId() == nuevo.getId() ){
			memcpy(&(**it),&nuevo,sizeof(LTexture));
		}
	}
}

bool Grafica::actualizarDibujable(int idObjeto,int x,int y, int spx, int spy, char flip){
	bool actualizo = false;

	for (list<LTexture*>::iterator it = listaDibujable.begin(); it != listaDibujable.end(); ++it) {
		if ( (*it)->getId() == idObjeto ){
			(*it)->actualizar( x, y, spx, spy, flip);
			actualizo = true;
		}
	}

	return actualizo;
}

bool Grafica::actualizarDibujableEnemigos(int idObjeto,int x,int y, int spx, int spy, char flip){
	bool actualizo = false;

	for (list<LTextureEnemigo*>::iterator it = listaDibujableEnemigos.begin(); it != listaDibujableEnemigos.end(); ++it) {
		if ( (*it)->getId() == idObjeto ){
			(*it)->actualizar( x, y, spx, spy, flip);
			actualizo = true;
		}
	}

	return actualizo;
}

void Grafica::borrarDibujable(int id) {
	for (list<LTexture*>::iterator it = listaDibujable.begin(); it != listaDibujable.end(); ++it) {
		if ((**it).getId() == id ){
			(*it)->free();
			delete (*it);
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
		TTF_Init();
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
					exito = false; //
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


	SDL_Surface* loadedSurface = IMG_Load("soldado.png");
	soldadosTexture = SDL_CreateTextureFromSurface( window, loadedSurface );
	SDL_FreeSurface(loadedSurface);

	return success;
}

bool Grafica::inicializarPersonaje(char* path, int ancho, int alto){
	bool exito = true;



	return exito;
}

void Grafica::setXCamara(int camaraX){
	if(xCamara == 0){
		camera3.x = 0;
		camera2.x = 0;
		camera1.x = 0;
	}


	if (xCamara < 0)
		xCamara = 0;

	this->xCamara = camaraX;

	//if(camera3.x < (4000-camera3.w))
	camera3.x = xCamara*camera3.w/anchoVentana;

	//if(camera2.x < (2000-camera2.w))
	camera2.x = (((2000-camera3.w)*camera3.x)/(4000-camera3.w));

	//if(camera1.x < (1000-camera1.w))
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

	return false;
}
/* caso empezaria de nuevo

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

 */
