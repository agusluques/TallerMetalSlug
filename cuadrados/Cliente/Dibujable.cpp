#include "Dibujable.h"

Dibujable::Dibujable() {
	posicion.x = 0;
	posicion.y = 0;
	sprite = NULL;
}

void Dibujable::moverX(int movimiento){
	posicion.x = posicion.x + movimiento;
}

void Dibujable::moverY(int movimiento){
	posicion.y = posicion.y + movimiento;
}

void Dibujable::setX(int movimiento){
	posicion.x = movimiento;
}

void Dibujable::setY(int movimiento){
	posicion.y = movimiento;
}

void Dibujable::setId(int id){
	this->id = id;
}

int Dibujable::getId(){
	return id;
}

void Dibujable::setSprite(string rutaImg){
	sprite = SDL_LoadBMP(rutaImg.c_str());
	if ( sprite == NULL ) {
		printf("No pude cargar gráfico: %s\n", SDL_GetError());
		exit(1);
	}
}

int Dibujable::posicionX(){
	return posicion.x;
}

int Dibujable::posicionY(){
	return posicion.y;
}

SDL_Rect* Dibujable::getPosicion(){
	return &posicion;
}

SDL_Surface* Dibujable::getSprite(){
	return sprite;
}

Dibujable::~Dibujable() {
	// TODO Auto-generated destructor stub
}

