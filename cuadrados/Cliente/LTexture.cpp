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

void LTexture::render(SDL_Renderer *window, SDL_Texture *mTexture, int ancho, int alto){
	SDL_Rect renderQuad;

	renderQuad.x = xcord;
	renderQuad.y = ycord;

	//esto entra por el xml
	renderQuad.w = ancho;
	renderQuad.h = alto;
	//renderQuad.w = player[ spX ][0].w;
	//renderQuad.h = player[ spX ][0].h;

	SDL_RenderCopyEx( window, mTexture, &player[spX][spY], &renderQuad, 0, NULL, flipType );
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
	/*cuerpo[0][0].x=8; cuerpo[0][0].y=443;
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
	cuerpo[5][0].w=35; cuerpo[5][0].h=50;*/

	//CORRIENDO
	player[0][0].x=0; player[0][0].y=50;
	player[0][0].w=30; player[0][0].h=40;

	player[1][0].x=40; player[1][0].y=50;
	player[1][0].w=30; player[1][0].h=40;

	player[2][0].x=80; player[2][0].y=50;
	player[2][0].w=30; player[2][0].h=40;

	player[3][0].x=120; player[3][0].y=50;
	player[3][0].w=30; player[3][0].h=40;

	player[4][0].x=160; player[4][0].y=50;
	player[4][0].w=30; player[4][0].h=40;

	player[5][0].x=200; player[5][0].y=50;
	player[5][0].w=30; player[5][0].h=40;

	//PARADO
	player[0][1].x=0; player[0][1].y=0;
	player[0][1].w=30; player[0][1].h=40;

	// sprite gris cuando se cierra la ventana.. ver cuando se mata el cliente q onda..
	//momia
	player[1][1].x = 0; player[1][1].y = 100;
	player[1][1].w = 30; player[1][1].h = 40;
	//zombi
	player[2][1].x = 40; player[2][1].y = 100;
	player[2][1].w = 30; player[2][1].h = 40;
}
