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

	renderQuad.x = xcord;
	renderQuad.y = ycord;
	renderQuad.w = cuerpo[ spX ][0].w;
	renderQuad.h = cuerpo[ spX ][0].h;

	SDL_RenderCopyEx( window, mTexture, &cuerpo[ spX ][0], &renderQuad, 0, NULL, flipType );
//	SDL_RenderCopy( window, mTexture, &cuerpo[ spX ][0], &renderQuad );
}

void LTexture::actualizar(int x, int y, int spx, int spy){
	if(this->xcord > x)	flipType = SDL_FLIP_HORIZONTAL; else flipType = SDL_FLIP_NONE;
	this->xcord = x;
	this->ycord = y;
	this-> spX = spx;
	this-> spY = spy;
}

void LTexture::setX(int movimiento){
	this->xcord = movimiento;
}

void LTexture::setY(int movimiento){
	this->ycord = movimiento;
}

void LTexture::setSpX(int movimiento){
	this->spX = movimiento;
}

void LTexture::setSpY(int movimiento){
	this->spY = movimiento;
}

void LTexture::setId(int id){
	this->id = id;
}

int LTexture::getId(){
	return this->id;
}

void LTexture::inicializarTexture(){
	cuerpo[0][0].x=8; cuerpo[0][0].y=443;
	cuerpo[0][0].w=35; cuerpo[0][0].h=50;

	cuerpo[1][0].x=57; cuerpo[1][0].y=441;
	cuerpo[1][0].w=35; cuerpo[1][0].h=50;

	cuerpo[2][0].x=98; cuerpo[2][0].y=441;
	cuerpo[2][0].w=35; cuerpo[2][0].h=50;

	cuerpo[3][0].x=131; cuerpo[3][0].y=441;
	cuerpo[3][0].w=35; cuerpo[3][0].h=50;

	cuerpo[4][0].x=163; cuerpo[4][0].y=441;
	cuerpo[4][0].w=35; cuerpo[4][0].h=50;

	cuerpo[5][0].x=200; cuerpo[5][0].y=441;
	cuerpo[5][0].w=35; cuerpo[5][0].h=50;
}
