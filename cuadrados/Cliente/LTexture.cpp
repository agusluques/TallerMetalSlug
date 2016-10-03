#include "LTexture.h"

LTexture::LTexture() {
	id = 0;
	xcord = 0;
	ycord = 0;
	spX = 0;
	spY = 0;

	flipType = SDL_FLIP_NONE;
}

LTexture::~LTexture() {
	//Deallocate
	//free();
}

void LTexture::render(SDL_Renderer *window, SDL_Texture *mTexture){
	SDL_Rect renderQuad;

	//duplico para las pieras..
	renderQuad.x = xcord;
	renderQuad.y = ycord + 23;
	renderQuad.w = pierna[ spX ][0].w;
	renderQuad.h = pierna[ spX ][0].h;

	SDL_RenderCopyEx( window, mTexture, &pierna[ spX ][0], &renderQuad, 0, NULL, flipType );
//	SDL_RenderCopy( window, mTexture, &pierna[ spX ][0], &renderQuad );

	renderQuad.x = xcord;
	renderQuad.y = ycord;
	renderQuad.w = torzo[ spX ][0].w;
	renderQuad.h = torzo[ spX ][0].h;

	SDL_RenderCopyEx( window, mTexture, &torzo[ spX ][0], &renderQuad, 0, NULL, flipType );
//	SDL_RenderCopy( window, mTexture, &torzo[ spX ][0], &renderQuad );
}

void LTexture::moverX(int movimiento){
	this->xcord += movimiento;
	this-> spX++;
	if (spX == 6) spX=0;
}

void LTexture::moverDerecha(int movimiento){
	flipType = SDL_FLIP_NONE;
	this->xcord += movimiento;
	this-> spX++;
	if (spX == 6) spX=0;
}

void LTexture::moverIzquierda(int movimiento){
	flipType = SDL_FLIP_HORIZONTAL;
	this->xcord -= movimiento;
	this-> spX++;
	if (spX == 6) spX=0;
}

void LTexture::moverY(int movimiento){
	this->ycord += movimiento;
	this-> spX++;
	if (spX == 6) spX=0;
}

void LTexture::moverArriba(int movimiento){
	this->ycord -= movimiento;
	this-> spX++;
	if (spX == 6) spX=0;
}

void LTexture::moverAbajo(int movimiento){
	this->ycord += movimiento;
	this-> spX++;
	if (spX == 6) spX=0;
}


void LTexture::setX(int movimiento){
	this->xcord = movimiento;
}

void LTexture::setY(int movimiento){
	this->ycord = movimiento;
}

void LTexture::setId(int id){
	this->id = id;
}

int LTexture::getId(){
	return this->id;
}

void LTexture::inicializarTexture(){
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
